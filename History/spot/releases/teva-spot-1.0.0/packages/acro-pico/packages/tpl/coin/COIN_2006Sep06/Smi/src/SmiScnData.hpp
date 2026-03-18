// Copyright (C) 2003, International Business Machines
// Corporation and others.  All Rights Reserved.

#ifndef SmiScnData_HPP
#define SmiScnData_HPP

#if defined(_MSC_VER)
// Turn off compiler warning about long names
#  pragma warning(disable:4786)
#endif

#include "OsiSolverInterface.hpp"
#include "CoinPackedVector.hpp"
#include "CoinMpsIO.hpp"
#include "SmiCoreCombineRule.hpp"
#include "SmiLinearData.hpp"

#include <map>
#include <vector>

using namespace std;


typedef int SmiCoreIndex;
typedef int SmiScenarioIndex;
typedef int SmiStageIndex;

class SmiCoreData;

class SmiNodeData : public SmiLinearData
{
public:
	typedef map<int,CoinPackedVector *> SmiRowMap;
	void setCoreNode();
	CoinPackedVector * getRow(int i) { 
		SmiRowMap::iterator r=rowMap.find(i); 
		if (r!=rowMap.end()) return r->second; 
		else return NULL;}
	inline SmiCoreData * getCore() { return core_;}
	inline int getStage() { return stg_;}
	inline  int getNumElements(){ return nels_; }

	inline void setCoreCombineRule(SmiCoreCombineRule *r){combineRule_=r;}
	inline SmiCoreCombineRule * getCoreCombineRule() { return combineRule_;}

	void copyRowLower(double * drlo);
	void copyRowUpper(double * drup);
	void copyColLower(double * dclo);
	void copyColUpper(double * dcup);
	void copyObjective(double * dobj);

	CoinPackedVector * combineWithCoreRow(CoinPackedVector *cr, CoinPackedVector *nr);

	SmiNodeData(SmiStageIndex stg, SmiCoreData *core,
				 const CoinPackedMatrix *const matrix,
				 CoinPackedVector *dclo, 
				 CoinPackedVector *dcup,
				 CoinPackedVector *dobj,
				 CoinPackedVector *drlo, 
				 CoinPackedVector *drup);
	~SmiNodeData();

protected:
	void combineWithCoreDoubleArray(double *d_out, const CoinPackedVector &cpv, int o);

private:
	SmiStageIndex stg_;
	SmiRowMap rowMap;
	int nels_;
	SmiCoreData *core_;
	bool isCoreNode_;
	SmiCoreCombineRule *combineRule_;
};


class SmiCoreData
{
public:
	inline int getNumCols(){ return ncol_;}
	inline int getNumRows(){ return nrow_;}
	inline int getNumStages(){ return nstag_;}
	inline int getNumCols(SmiStageIndex t){ return nColInStage_[t];}
	inline int getNumRows(SmiStageIndex t){ return nRowInStage_[t];}
	inline int getColStart(SmiStageIndex t){ return stageColPtr_[t];}
	inline int getRowStart(SmiStageIndex t){ return stageRowPtr_[t];}
	inline int getColStage(int i){ return colStage_[i];}
	inline int getRowStage(int i){ return rowStage_[i];}
	inline int getRowInternalIndex(int i){ return rowEx2In_[i];}
	inline int getColInternalIndex(int i){ return colEx2In_[i];}
	inline int getRowExternalIndex(int i){ return rowIn2Ex_[i];}
	inline int getColExternalIndex(int i){ return colIn2Ex_[i];}
	inline CoinPackedVector * getMatrixRow(SmiStageIndex t, int i){ return nodes_[t]->getRow(i);}
	inline const CoinPackedVector &getRowLower(SmiStageIndex t){return nodes_[t]->getRowLower();}
	inline const CoinPackedVector &getRowUpper(SmiStageIndex t){return nodes_[t]->getRowUpper();}
	inline const CoinPackedVector &getColLower(SmiStageIndex t){return nodes_[t]->getColLower();}
	inline const CoinPackedVector &getColUpper(SmiStageIndex t){return nodes_[t]->getColUpper();}
	inline const CoinPackedVector &getObjCoefficients(SmiStageIndex t){return nodes_[t]->getObjective();}

	void copyRowLower(double * drlo,SmiStageIndex t );
	void copyRowUpper(double * drup,SmiStageIndex t);
	void copyColLower(double * dclo,SmiStageIndex t);
	void copyColUpper(double * dcup,SmiStageIndex t);
	void copyObjective(double * dobj,SmiStageIndex t);

	inline SmiNodeData * getNode(SmiStageIndex t){return nodes_[t];}
	SmiCoreData(OsiSolverInterface *osi, int nstag, int *cstag, int *rstag);
	SmiCoreData(CoinMpsIO *cMps, int nstag, int *cstag, int *rstag);
	~SmiCoreData();

private:
	void gutsOfConstructor(int nrow,int ncol,int nstag,
							   int *cstag,int *rstag,
							   CoinPackedMatrix *matrix,
							   CoinPackedVector *dclo,
							   CoinPackedVector *dcup,
							   CoinPackedVector *dobj,
							   CoinPackedVector *drlo,
							   CoinPackedVector *drup);
private:
	int nrow_;
	int ncol_;
	SmiStageIndex nstag_;
	int *nColInStage_;
	int *nRowInStage_;
	int *stageColPtr_;
	int *stageRowPtr_;
	int *colStage_;
	int *rowStage_;
	int *colEx2In_;
	int *rowEx2In_;
	int *colIn2Ex_;
	int *rowIn2Ex_;
	double **cdrlo_; 
	double **cdrup_;
	double **cdobj_;
	double **cdclo_; 
	double **cdcup_;
	vector<SmiNodeData*> nodes_;
};

#endif //#define SmiScnData_HPP
