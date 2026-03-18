//
// impact2Dynamic.cpp
// author: Jon Berry
// creation: 5/2005
//
// precondition: impact file(s) created
// postcondition: ampl .dat file(s) created
// usage: impact2Dynamic -imf <impactFile> -n <#nodes>");
//	[-awf attackWeightsFile] [-amf amplOutputFile] 
//      [-b budget] [-slt superlocation_threshold] 
//      [-nf nodeCostFile]
// example:
// genattack 128 > attack.128
// impact2Dynamic -imf ~/waterData/exp1viz/exp1viz.0.mc.impact -n 36  \\
//                -awf attack.128 -amf exp1viz.0.mc.dat -b 3 -slt 0.0

#include <iostream>
#include <fstream>
#include <map>
#include <list>
#include <vector>
#include <string>
#include "Result.h"

using namespace std;

#define STRMAX 512
#define max(a,b) ((a) > (b))? a : b;

char emsg[STRMAX];
                                                  
extern  "C" {
#include <string.h>
}

class Attack {
public:
	Attack(int v, int m, int e, float ss, int r) : 
				vertex(v), minute(m), end_minute(e), 
				strength(ss), rank(r), prob(0.0) {}
	int vertex;
	int minute, end_minute;
	float strength;
	int rank;
	double prob;
};

class SuperLocation {
public:
	SuperLocation(int idd, vector<int> memb)  : 
		id(idd), members(memb)
	{
	}
	int id;
	vector<int> members;
};

class OneRunData {
public:
	OneRunData(int av, int sm, Attack *a, vector<RData*> res)  : 
		attack_vertex(av), start_minute(sm), attack_data(a),
		result(res)
	{
	}
	int attack_vertex;
	int start_minute;
	double current_consump;
	Attack *attack_data;
	vector<RData*> result;
};


static list<Attack*>    attacks;
static list<OneRunData> resultList;
static list<OneRunData> superLocResults;
map< vector<int>, int>  superLocIDs;
map< int, vector<int> > superLocs;
map< string, int>       vmap;
map< int, int>          costMap;
	
void addResult(int attack_vertex, int start_minute, Attack*atckdata,
	       vector<RData*> result)
{
	resultList.push_back(OneRunData(attack_vertex,start_minute,
				        atckdata, result));
}

void processAttackWeights(char *afile, int n, int numattacks)
{
	ifstream inp(afile);
	list<double> attackprobs;
	double next;
	while (inp >> next) {
		attackprobs.push_back(next);
	}
	int num_probs = attackprobs.size();
	if (num_probs != numattacks) {
		cerr << "baseline attack file needs " << numattacks
			<< " values"<<endl;
		//exit(1);
	}
	double *atckprobs = new double[num_probs];
	copy(attackprobs.begin(), attackprobs.end(), atckprobs);
	double total =0.0;
	for (int i=0; i<=num_probs; i++) {
		total += atckprobs[i];
	}
	if ((total <= (.99)) || (total >= (1.01))) {
		cerr << "baseline attack file: attack probs. must sum to 1, "
		     << "actual sum: " << total << endl;
	}
	inp.close();
	list<Attack*>::iterator atck;
	total = 0.0;
	int aindex=0;
	for (atck=attacks.begin(); atck!=attacks.end(); atck++) {
		Attack *a = *atck;
		double aprob = atckprobs[aindex++];
		a->prob = aprob;
	}
	delete [] atckprobs;
}

void processNodeCosts(char *cfile, int n)
{
	if (!cfile) {
		for (int i=1; i<=n; i++)
			costMap[i] = 1;
		return;
	}
	ifstream inp(cfile);
	string id;
	int cost;
	while (inp >> id >> cost) {
		int index = vmap[id];
		costMap[index] = cost;
	}
	inp.close();
}

void findSuperLocations(int n, int numattacks, double threshold)
{
	int next = n+2;	// 1..n: real, n+1: dummy, n+2: superDummy, n+3..:supers
	int superDummy = next;
	double current_consump;
	double initial_consump;
	vector<int> superDummyLocs;
	superDummyLocs.push_back(n+1);
	superLocIDs[superDummyLocs] = next;	// n+2 is the super dummy
	superLocs[next++] = superDummyLocs;

	list<OneRunData>::iterator it = resultList.begin();
	for (; it!=resultList.end(); it++) {
		vector<RDataPtr> newresult;
		OneRunData rd = *it;
		int attack_vertex = rd.attack_vertex;
		int    start_minute =  rd.start_minute;
		Attack *attack_data =  rd.attack_data;
		vector<RDataPtr> res = rd.result;
		int i = 0;
		while (i < int(res.size())) {
			initial_consump = res[i]->consumedBeforeAlarmAt;
			current_consump = initial_consump;
			if (i == int(res.size()-1)) {
				newresult.push_back(new SRData(superDummy, 
						current_consump));
				break;
			}
			int j = i;
			vector<int> thisSL;
			int thisSL_ID;
			while (j<int(res.size()) && 
			       res[j]->consumedBeforeAlarmAt <= 
					initial_consump + threshold) {
				thisSL.push_back(res[j]->getID());
				current_consump = res[j]->consumedBeforeAlarmAt;
				j++;
			}
        		sort(thisSL.begin(), thisSL.end());
        		map<vector<int>,int>::iterator it = 
						superLocIDs.find(thisSL);
        		if (it != superLocIDs.end()) {
				thisSL_ID = superLocIDs[thisSL];
			} else {
				thisSL_ID = next++;
				superLocIDs[thisSL] = thisSL_ID;
			}
			superLocs[thisSL_ID] = thisSL;
			newresult.push_back(new SRData(thisSL_ID, 
						       current_consump));
			i = j;
		}
		superLocResults.push_back(
			OneRunData(attack_vertex, start_minute, attack_data,
				   newresult));
	}
}

void outputFile(string fname, int argc, char *argv[], int n,
		int numattacks, int budget)
{
	ofstream of(fname.c_str());
	of << "#" << endl;
	of << "# AMPL file generated with: ";
	for (int i=0; i<argc; i++) {
		of << argv[i] << " ";
	}
	of << endl << "#" << endl;
	of << "param numTrueNetworkLocations := " << n << ";" << endl;
	of << "param Loc1st := 1;" << endl;

	of << "set SuperLocations := " << endl;
	map<int, vector<int> >::iterator sit = superLocs.begin();
	for (; sit!=superLocs.end(); sit++) {
		int id = (*sit).first;
		of << id << endl;
	}
	of << ";" << endl;

	map<int, vector<int> >::iterator sit2 = superLocs.begin();
	for (; sit2!=superLocs.end(); sit2++) {
		int id = (*sit2).first;
		of << "set SuperLocationMembers[" << id << "] := " ;
		vector<int> members = (*sit2).second;
		for (int i=0; i<int(members.size()); i++)  {
			of << " " << members[i];
		}
		of << ";" << endl;
	}
	of << "param numAttacks := " << numattacks << ";" << endl;
	of << "param Attack1st := 1;" << endl;
	of << "param AttackWeight :=" << endl;
	list<OneRunData>::iterator it = resultList.begin();
	for (; it!=resultList.end(); it++) {
		Attack *a = (*it).attack_data;
		of << a->rank << " " << a->prob << endl;
	}
	of << ";"<< endl;
	of << "param Budget := " << budget << ";" << endl;
	of << "set AttackTouchedLocPair := " << endl;

	for (it=superLocResults.begin(); it!= superLocResults.end(); it++) {
	  OneRunData data = *it;
	  Attack *adata = data.attack_data;
	  vector<RData*> result = data.result;
	  vector<RData*>::iterator it2;
	  vector<RData*>::iterator itend=result.end(); 
	  itend--;
	  of << "(" << adata->rank << ", *) := ";
	  for (it2=result.begin(); it2!=itend;it2++){
		(*it2)->printLoc(of);
		of << ",";
	  }
	  (*it2)->printLoc(of);
          of << endl;
	}
	of << "\n;\nparam ConsumptionBeforeAlarmAt := " << endl;

	for (it = superLocResults.begin(); it!= superLocResults.end(); it++) {
	  OneRunData data = *it;
	  Attack *adata = data.attack_data;
	  vector<RData*> result = data.result;
	  vector<RData*>::iterator it2;
	  for (it2 = result.begin(); it2!=result.end(); it2++) {
	  	of << adata->rank << " ";
		(*it2)->printLoc(of);
	  	of << " ";
		(*it2)->printVC(of);
		of << endl;
	  }
	}
	of << ";" << endl;

	of << "\n;\nparam AttackLoc := " << endl;

	for (it = resultList.begin(); it!= resultList.end(); it++) {
	  OneRunData data = *it;
	  Attack *adata = data.attack_data;
	  of << adata->rank << " " << adata->vertex << endl;
	}
	of << ";" << endl;

	of << "\n;\nparam AttackTime := " << endl;

	for (it = resultList.begin(); it!= resultList.end(); it++) {
	  OneRunData data = *it;
	  Attack *adata = data.attack_data;
	  of << adata->rank << " " << adata->minute << endl;
	}
	of << ";" << endl;

	of << "\n;\nparam LocPlacementCost := " << endl;

	for (int i=1; i<=n; i++) {
	  of << i << " " << costMap[i] << endl;
	}
	of << ";" << endl;
	of.close();
}

// ***********************************************************************
// ** oneWQsimulation*****************************************************
// *********************************************************************** 
// ***********************************************************************
int readImpactFile(char *impactFile, int numnodes)
{
	int numattacks=0;
	ifstream imf(impactFile);
	int atcknum, witness, anode, amin, cur_atcknum=-1;
	double damage;
	vector<RData*> result;
	Attack *atck=0;
	while (imf >> atcknum >> anode >> amin >>witness >> damage) {
		if (witness == 0) {
			printf("oops!\n");
		}
		if (atcknum != cur_atcknum) {
			cur_atcknum = atcknum;
			atck = new Attack(anode,amin,-1,-1, atcknum);
			numattacks++;
			if (numattacks != atcknum)
				cout << "error: attack num: " << atcknum
				     << ", count: " << numattacks << endl; 
			attacks.push_back(atck);
		}
		if (witness == -1) {
			result.push_back(new DummyRData(damage,numnodes));
			int source = anode; // (*(result.begin()))->getID();
  			addResult(source, amin, atck, result);//time unknown
			result.clear();
		} else {
			result.push_back(new VRData(witness,damage));
		}
	}
	return numattacks;
}
		
void argUsageMsg()
{
	printf("usage: impact2Dynamic -imf <impactFile> -n <#nodes>");
	printf(" [-awf attackWeightsFile] [-amf amplOutputFile] "
               " [-b budget] [-slt superlocation_threshold] \n"
               " [-nf nodeCostFile]\n");
}

void processArgs(int argc, char *argv[], char *& impactFile, 
	    char*&attackWeightsFile, char*& amplOutputFile, int& budget, 
	    float& superlocation_threshold, char*& nodeCostFile, int& nnodes,
	    int& nattacks)
{
        if(argc > 2) {
                int ai = 1;
                while(ai<argc) {
                        if(strncmp(argv[ai],"-slt",4)==0) {
                                superlocation_threshold = atof(argv[++ai]);
                        } else if(strncmp(argv[ai],"-awf",4)==0) {
                                attackWeightsFile = argv[++ai];
                        } else if(strncmp(argv[ai],"-imf",4)==0) {
                                impactFile = argv[++ai];
                        } else if(strncmp(argv[ai],"-amf",4)==0) {
                                amplOutputFile = argv[++ai];
                        } else if(strncmp(argv[ai],"-n",2)==0) {
                                nnodes = atoi(argv[++ai]);
                        } else if(strncmp(argv[ai],"-b",2)==0) {
                                budget = atoi(argv[++ai]);
                        } else {
				argUsageMsg();
                                exit(9);
                        }
                        ai++;
                }
        }
}

int main(int argc, char **argv)
{
        char *impactFile;
  	char *amplOutputFile = NULL;
  	char *attackWeightsFile = NULL;
  	int budget, nnodes, nattacks;
  	float sl_threshold = 0.0;
  	char *costFile = NULL;

        if(argc <2 || argc >14) {
		argUsageMsg();
                exit(9);
        }
        processArgs(argc, argv, impactFile, attackWeightsFile, 
		    amplOutputFile, budget, sl_threshold, costFile, nnodes,
		    nattacks);
	int numattacks = readImpactFile(impactFile, nnodes);
	cout << numattacks << " attacks " << endl;
	cout << resultList.size() << " resultList.size() " << endl;
  	processAttackWeights(attackWeightsFile, nnodes, numattacks);
  	processNodeCosts(costFile, nnodes);
  	findSuperLocations(nnodes, numattacks, sl_threshold);
  	outputFile(amplOutputFile, argc, argv, nnodes, numattacks, budget);

	return 0;
}
