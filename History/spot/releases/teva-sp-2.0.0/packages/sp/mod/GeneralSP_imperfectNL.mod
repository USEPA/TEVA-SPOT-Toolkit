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

############################################################################
# dummySuperLocation
#	This parameter has a different purpose than the dummyLocation.
#	It is included to avoid confusion over this new purpose.
#       In order to compute witness probabilities, we must look at the
#       second super location in each witness list.  If there is only
#	one super location in the witness list, then this dummy super
#	location serves as the "second location."  To be clear, the
#       last non-dummy super location in the witness list of an attack
#       will still contain the dummyLocation.
############################################################################
param dummySuperLocation := -1;

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

set AugmentedSuperLocations {g in ActiveGoals, a in Attacks[g]} := 
		AttackSuperLocations[g,a] union {dummySuperLocation};
#
# TODO: add checks here?
#
## Environment types are distinguished by any properties that effect the ability to sense an attack.
## For example, sensors located near a swamp might share an environment type, those that are
## in areas that are particularly cold or hot, etc.  The false positive and false negative
## rates for a sensor depend upon the environment where it will operate

## The number of environment types determines the number of indices for the count variables.
## as of right now.  So it's not really a parameter one can change.  We're making it a
## parameter in case we choose to encode the tuple later or find another way to allow
## this generalization.

param numEnvironmentTypes default 3;

check numEnvironmentTypes = 3;

set EnvironmentTypeNames := 1..numEnvironmentTypes;

param EnvironmentTypes {NetworkLocations};

# False positives per day
param FalsePositiveRate{EnvironmentTypeNames};
param FalseNegativeProbability{EnvironmentTypeNames};

param DetectionProbability{e in EnvironmentTypeNames} := 
				1 - FalseNegativeProbability[e];

#  Count the environment types in each superlocation

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


##############################################################################
# PrevSuperLocations
#
#	For every attack and super location witness, store the set of
#       previous super location witnesses.
##############################################################################

set PrevSuperLocations {g in ActiveGoals, a in Attacks[g], 
			L in AttackSuperLocations[g,a]} :=
        setof {sl in AttackSuperLocations[g,a]:
	       GoalMeasurement[g,a,WorstSuperLocationMember[g,a,sl]]<
	       GoalMeasurement[g,a,WorstSuperLocationMember[g,a,L]]} sl;

##########################################################################
## ProbabilityOfPlaying
##		For each possible super location witness L of an attack,
##		store the probability that all sensors in all super 
##		locations that precede L in the witness list will fail to
##		detect, giving L a chance to be the witness;  Note that 
##		if L is the first witness in the list, this probability
##		will be 1.0.
##########################################################################

var ProbabilityOfPlaying{g in ActiveGoals, a in Attacks[g], 
			     L in AttackSuperLocations[g,a]};

##########################################################################
## ProbabilityOfSuccess
##		For each possible super location witness L of an attack,
var ProbabilityOfSuccess{g in ActiveGoals, a in Attacks[g], 
			     L in AttackSuperLocations[g,a]};



##
## Decision variables
##
#
# s[loc] = 1 if there's a sensor at location loc, 0 otherwise.
#

## Within each superlocation, count the number of deployed sensors
## of each environment type

var s {FeasibleLocations} binary;
#
# WitnessProbability[AttackSuperLocations] = the probability that
# a touched location within this superLocation witnesses the attack.
# The sensor must be selected (placed) and all sensors placed in
# locations that would see this attack earlier must fail to detect it.
#
var WitnessProbability{g in ActiveGoals, a in Attacks[g], AttackSuperLocations[g,a]} >= 0 <= 1; #
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
## detects the attack.  This is a probability distribution.  The dummy absorbs any probability left
## by the real sensors.
##
subject to pickFirstSensor {g in ActiveGoals, a in Attacks[g]}:
        sum {L in AttackSuperLocations[g,a]} WitnessProbability[g,a,L] = 1;

##
## A sensor on a real location can't be the first tripped if that location isn't selected
##
subject to onlyPickReal {g in ActiveGoals, a in Attacks[g], L in AttackSuperLocations[g,a] : dummyLocation not in SuperLocationMembers[L]}:
        WitnessProbability[g,a,L] <= sum {l in SuperLocationMembers[L]} s[l];

##
## Mean Goal Value
##
subject to meanGoals {(g,m) in ActiveGMPair : m == "mean"}:
	GoalValue[g,m] = sum{a in Attacks[g]} AttackWeight[g,a] * 
	   			sum{L in AttackSuperLocations[g,a]} 
					GoalMeasurement[g,a,WorstSuperLocationMember[g,a,L]]*WitnessProbability[g,a,L];

##
## Worst Goal Value
##
subject to worstGoals {(g,m) in ActiveGMPair diff {("ns","total"), ("cost","total")}, a in Attacks[g] : m == "worst"}:
	sum{L in AttackSuperLocations[g,a]} GoalMeasurement[g,a,WorstSuperLocationMember[g,a,L]]*WitnessProbability[g,a,L] <= GoalValue[g,m];

##
## total ns Goal Value
##
subject to nsGoal:
	if (("ns","total") in ActiveGMPair)
		then GoalValue["ns","total"] 
		else 0.0
	= 
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
			WitnessProbability[g,a,L] - VaR[g];

##
## Constraint limits
##
subject to goalBounds {(g,m) in goalConstraints}:
	GoalValue[g,m] <= GoalBound[g,m];

##########################################################################
## compute_probability_of_playing --
##		For each possible super location witness L of an attack,
##		compute the probability that all sensors in all super 
##		locations that precede L in the witness list will fail to
##		detect, giving L a chance to be the witness;  Note that 
##		if L is the first witness in the list, this probability
##		will be 1.0.
##########################################################################
subject to compute_probability_of_playing{g in ActiveGoals, a in Attacks[g],
		  asl in AttackSuperLocations[g,a],
		  predsl in PrevSuperLocations[g,a,asl]: 
		  (card(PrevSuperLocations[g,a,asl])>=1) &&
		  GoalMeasurement[g,a,WorstSuperLocationMember[g,a,predsl]]
		    >= max {osl in PrevSuperLocations[g,a,asl]}
		        GoalMeasurement[g,a,WorstSuperLocationMember[g,a,osl]]}:

		ProbabilityOfPlaying[g,a,asl] = 

			ProbabilityOfPlaying[g,a,predsl]

		*

		( (FalseNegativeProbability[1] ** 
			  sum {l in SuperLocationMembers[predsl]: 
				l != dummyLocation &&
				EnvironmentTypes[l]=1} s[l])
		      *(FalseNegativeProbability[2] ** 
			  sum {l in SuperLocationMembers[predsl]: 
				l != dummyLocation &&
				EnvironmentTypes[l]=2} s[l])
		      *(FalseNegativeProbability[3] ** 
			  sum {l in SuperLocationMembers[predsl]: 
				l != dummyLocation &&
				EnvironmentTypes[l]=3} s[l]));
			
##########################################################################
## compute_witness_probabilities --
##			The witness variables "WitnessProbability" used
##			to be indicator variables.  Now they are 
##			probabilities.  The probability that super location
##			i witnesses attack a is the probability that all
##			possible witnesses in super locations before i 
##			in the witness list fail times the probability that
##			one of the locations in i succeeds.
##
##                      Not so nice since we'd like to iterate over the
##			environment types in general;  here, we explicitly
##			address environment types individually.
##########################################################################
subject to compute_first_witness_probability{g in ActiveGoals, a in Attacks[g],
		  asl in AttackSuperLocations[g,a],
		  psl in PrevSuperLocations[g,a,asl]:
		  (card(PrevSuperLocations[g,a,asl])=1)}:
	WitnessProbability[g,a,psl] <= 
		(1 - ( (FalseNegativeProbability[1] ** 
			  sum {l in SuperLocationMembers[asl]: 
				l != dummyLocation &&
				EnvironmentTypes[l]=1} s[l])
		      *(FalseNegativeProbability[2] ** 
			  sum {l in SuperLocationMembers[asl]: 
				l != dummyLocation &&
				EnvironmentTypes[l]=2} s[l])
		      *(FalseNegativeProbability[3] ** 
			  sum {l in SuperLocationMembers[asl]: 
				l != dummyLocation &&
				EnvironmentTypes[l]=3} s[l])));

subject to compute_witness_probabilities{g in ActiveGoals, a in Attacks[g],
					  asl in AttackSuperLocations[g,a]}:
	ProbabilityOfPlaying[g,a,asl] = 
			1 - sum {pl in PrevSuperLocations[g,a,asl]} 
			    WitnessProbability[g,a,pl];

end;
