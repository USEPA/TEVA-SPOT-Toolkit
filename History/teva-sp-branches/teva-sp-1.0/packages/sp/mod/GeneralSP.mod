#
# GeneralSP.mod
#
# AMPL model for sensor placement that uses general impact factors and cost measures.
# Adapted from Dynamic.mod
#
# This MILP model for sensor placement in water networks generalizes the p-median formulation in
# Dynamic.mod to allow for an arbitrary specification of the objective function as well as 
# constraints.  This model is only meaningful if either the objective is to minimize total cost (e.g.
# number of sensors, or if the model is constrained by total cost.
#
# This model allows for the following performance goals:
#	cost
#	ec
#	mc
#	nfd
#	ns
#	pe
#	pk
#	td
#	vc
# Note: we want to minimize all of these goals!
#
# This model allows for the computation of a variety of performance measures for these goals:
#	mean performance
#	var (value-at-risk)
#	cvar (conditional value-at-risk)
#	worst performance
#

##
## Objective information
##
#
# The set of allowable performance goals
#
set goals := {"cost", "ec", "mc", "nfd", "ns", "pe", "pk", "td", "vc"};
#
# Goal measures
# 
set measures := {"mean", "var", "cvar", "worst", "total"};
#
# The optimization objective's goal
#
param objectiveGoal symbolic in goals;
#
# The optimization objective's measure
#
param objectiveMeasure symbolic in measures default "mean";
#
# The goals used for constraints
#
set goalConstraints within goals cross measures diff {(objectiveGoal, objectiveMeasure)};
#
# Active goals
#
set ActiveGoals := ((setof {(g,m) in goalConstraints} g) union {objectiveGoal}) diff {"ns","cost"};
#
# The active goal/measure pairs
#
set ActiveGMPair := goalConstraints union {(objectiveGoal,objectiveMeasure)};
#
# CHECK ERRORS
#
check {(g,m) in ActiveGMPair}: ((m = "total") && (g in {"ns","cost"})) || 
((m != "total") && !(g in {"ns","cost"}));

##
## The water network elements. These are locations in the network where we can
## put sensors.  For now just number them consecutively.  
## Network elements are consecutive integers.  These are the node indices;
##
#
# Number of network locations
#
param numNetworkLocations, integer, > 0;
#
# A dummy location where that all attacks impact
#
param dummyLocation := -1;
#
# Places where sensors can be placed.
#
set NetworkLocations := 1 .. numNetworkLocations;
#
# Predetermined fixed sensor locations
#
set FixedSensorLocations within NetworkLocations default {};
#
# Predetermined locations where sensors cannot be placed
#
set InfeasibleLocations within NetworkLocations default {};
#
# The set of feasible sensor locations
#
set FeasibleLocations := NetworkLocations diff InfeasibleLocations;

#
# FEASIBILITY CHECKS
#
#check: FixedSensorLocations intersect InfeasibleLocations within {};


##
## Constraint information
##
#
# Bounds
#
param GoalBound {goalConstraints};
#
# The confidence interval, e.g. consider the worst 5% of cases
#
param gamma > 0.0 <= 1.0 default 0.05;


##
## Attack scenarios are defined by an attack location (for us, this will always be a node)
## and a time of day.  However, this model doesn't need to know these, so we'll
## just label attacks consecutively
##
#
# Number of attacks
#
param numAttacks {ActiveGoals} integer, > 0;
#
# The set of attack IDs
#
set Attacks {g in ActiveGoals} := 1 .. numAttacks[g];
#
# Attack weights.  You can think of this as a probability that the
# (single-location) attack occurs at this location and time
#
param AttackWeight {g in ActiveGoals, a in Attacks[g]} >= 0.0 <= 1.0 default 1.0/numAttacks[g];
#
# Attack location information (used only for printing sensible outputs (e.g. debugging))
#
param AttackLoc  {g in ActiveGoals, Attacks[g]} default 0;
#
# Attack time information (used only for printing sensible outputs (e.g. debugging))
#
param AttackTime {g in ActiveGoals, Attacks[g]} default 0;

##
## Revised location information using attack information
##
#
# Locations that could actually see the impact of each attack
# Remember that the dummyLocation is touched by all attacks.
#
set AttackTouchedLocations {g in ActiveGoals, a in Attacks[g]} within NetworkLocations union {dummyLocation};
#
# The IDs of aggregated network location classes.
# NOTE: these are independent of the attack scenario, to facilitate 
# data compression.
#
set SuperLocations;
#
# The set of network locations that correspond to each super location 
#
set SuperLocationMembers {SuperLocations} within NetworkLocations union {dummyLocation};
#
# For each goal and attack, the set of SuperLocation IDs that define that attack
#
set AttackSuperLocations {g in ActiveGoals, a in Attacks[g]} within SuperLocations;
#
# TODO: add checks here?
#

##
## Goal information
##
#
# The goal values used in either the objective or constraints.
#
param GoalMeasurement {g in ActiveGoals, a in Attacks[g], AttackTouchedLocations[g,a]};
#
# The cost information used to compute the 'cost' goal
#
param PlacementCost {FeasibleLocations} default 0.0;
#
# ID of the element of a superlocation set that has the worst value
#
param WorstSuperLocationMember {g in ActiveGoals, a in Attacks[g], L in SuperLocations} in SuperLocationMembers[L];
#
# Some data checks
#
#check {g in ActiveGoals, a in Attacks[g], L in AttackSuperLocations[g,a], LL in AttackSuperLocations[g,a]}: (L == LL) or (L != LL) and SuperLocationMembers[L] intersect SuperLocationMembers[LL] within {};

##
## Decision variables
##
#
# s[loc] = 1 if there's a sensor at location loc, 0 otherwise.
#
var s {FeasibleLocations} binary;
#
# FirstSensorForAttack[AttackTouchedLocations] = 1 if and only if this
# touched location is the first sensor hit for this attack (among
# locations chosen to receive sensors, this one is the first hit by
# contamination among all those hit by this attack.  These will be
# binary in practice (as needed) as long as the sensor placement
# decisions are binary.
#
var FirstSensorForAttack{g in ActiveGoals, a in Attacks[g], AttackSuperLocations[g,a]} >= 0 <= 1; #
#
# Goal variable values
#
var GoalValue {ActiveGMPair};
#
# Variables used to compute the worst-case goal measurement
#
var WorstGoalMeasurement {g in ActiveGoals : (g,"worst") in goalConstraints};
#
# For CVaR we need to know both Value-At-Risk, VaR, and the extent to which VaR
# is exceeded in a given attack, y. By definition, VaR <= CVaR and, for small
# Gamma ( < 50% ), VaR >= expected value.  Note, we need these variables for
# each goal/cvar pair.
#
var VaR {g in ActiveGoals : (g,"cvar") in ActiveGMPair};
var y {g in ActiveGoals, a in Attacks[g] : (g,"cvar") in ActiveGMPair} >= 0;

##
## OBJECTIVE
##
minimize Objective:
	GoalValue[objectiveGoal,objectiveMeasure];

##
## Set the fixed locations
##
subject to FixedSensors{l in FixedSensorLocations}:
	s[l] = 1;
	
##
## For each attack, one sensor is tripped first (this could be the dummy, indicating no real sensor
## detects the attack
##
subject to pickFirstSensor {g in ActiveGoals, a in Attacks[g]}:
        sum {L in AttackSuperLocations[g,a]} FirstSensorForAttack[g,a,L] = 1;

##
## A sensor on a real location can't be the first tripped if that location isn't selected
##
subject to onlyPickReal {g in ActiveGoals, a in Attacks[g], L in AttackSuperLocations[g,a] : dummyLocation not in SuperLocationMembers[L]}:
        FirstSensorForAttack[g,a,L] <= sum {l in SuperLocationMembers[L]} s[l];

##
## Mean Goal Value
##
subject to meanGoals {(g,m) in ActiveGMPair : m == "mean"}:
	GoalValue[g,m] = sum{a in Attacks[g]} AttackWeight[g,a] * 
	   			sum{L in AttackSuperLocations[g,a]} 
					GoalMeasurement[g,a,WorstSuperLocationMember[g,a,L]]*FirstSensorForAttack[g,a,L];

##
## Worst Goal Value
##
subject to worstGoals {(g,m) in ActiveGMPair diff {("ns","total"), ("cost","total")}, a in Attacks[g] : m == "worst"}:
	sum{L in AttackSuperLocations[g,a]} GoalMeasurement[g,a,WorstSuperLocationMember[g,a,L]]*FirstSensorForAttack[g,a,L] <= GoalValue[g,m];

##
## total ns Goal Value
##
subject to nsGoal:
	if (("ns","total") in ActiveGMPair)
		then GoalValue["ns","total"] 
		else 0.0
	>= 
	if (("ns","total") in ActiveGMPair)
		then sum{l in FeasibleLocations} s[l]
		else 0.0;

##
## total cost Goal Value
##
subject to costGoal:
	if (("cost","total") in ActiveGMPair)
		then GoalValue["cost","total"]
		else sum{l in FeasibleLocations} PlacementCost[l]
	>= sum{l in FeasibleLocations} PlacementCost[l]*s[l];

##
## CVaR Goal Value
##
subject to cvarGoals {(g,m) in ActiveGMPair : m == "cvar"}:
	GoalValue[g,m] = VaR[g] + (1.0/gamma) * 
			sum{a in Attacks[g]} (AttackWeight[g,a] * y[g,a]);

##
## Ylimit - set y to the max of excess consumption above VaR or 0.
##
subject to yplus {g in ActiveGoals, a in Attacks[g] : (g,"cvar") in ActiveGMPair}:
	y[g,a] >= sum {L in AttackSuperLocations[g,a]} 
    			GoalMeasurement[g,a,WorstSuperLocationMember[g,a,L]]*
			FirstSensorForAttack[g,a,L] - VaR[g];

##
## Constraint limits
##
subject to goalBounds {(g,m) in goalConstraints}:
	GoalValue[g,m] <= GoalBound[g,m];

end;
