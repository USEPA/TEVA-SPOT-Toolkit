#include "SmiScnData.hpp"
#include "CoinHelperFunctions.hpp"
#include "OsiSolverInterface.hpp"
#include "CoinPackedMatrix.hpp"

#include <vector>

using namespace std;

SmiCoreData::SmiCoreData(OsiSolverInterface *osi,int nstag,int *cstag,int *rstag)
{
	int nrow = osi->getNumRows();
	int ncol = osi->getNumCols();
	CoinPackedVector *drlo = new CoinPackedVector(nrow,osi->getRowLower());
	CoinPackedVector *drup = new CoinPackedVector(nrow,osi->getRowUpper());
	CoinPackedVector *dclo = new CoinPackedVector(ncol,osi->getColLower()); 
	CoinPackedVector *dcup = new CoinPackedVector(ncol,osi->getColUpper());
	CoinPackedVector *dobj = new CoinPackedVector(ncol,osi->getObjCoefficients());

	CoinPackedMatrix *matrix = new CoinPackedMatrix(*osi->getMatrixByRow());
	matrix->eliminateDuplicates(0.0);

	gutsOfConstructor(nrow,ncol,nstag,cstag,rstag,matrix,dclo,dcup,dobj,drlo,drup);
	
		delete matrix;
		delete drlo;
		delete drup;
		delete dclo;
		delete dcup;
		delete dobj;



}

SmiCoreData::SmiCoreData(CoinMpsIO *osi,int nstag,int *cstag,int *rstag)
{
	int nrow = osi->getNumRows();
	int ncol = osi->getNumCols();
	CoinPackedVector *drlo = new CoinPackedVector(nrow,osi->getRowLower());
	CoinPackedVector *drup = new CoinPackedVector(nrow,osi->getRowUpper());
	CoinPackedVector *dclo = new CoinPackedVector(ncol,osi->getColLower()); 
	CoinPackedVector *dcup = new CoinPackedVector(ncol,osi->getColUpper());
	CoinPackedVector *dobj = new CoinPackedVector(ncol,osi->getObjCoefficients());

	CoinPackedMatrix *matrix = new CoinPackedMatrix(*osi->getMatrixByRow());
	matrix->eliminateDuplicates(0.0);

	gutsOfConstructor(nrow,ncol,nstag,cstag,rstag,matrix,dclo,dcup,dobj,drlo,drup);
	
		delete matrix;
		delete drlo;
		delete drup;
		delete dclo;
		delete dcup;
		delete dobj;



}
void
SmiCoreData::gutsOfConstructor(int nrow,int ncol,int nstag,
							   int *cstag,int *rstag,
							   CoinPackedMatrix *matrix,
							   CoinPackedVector *dclo,
							   CoinPackedVector *dcup,
							   CoinPackedVector *dobj,
							   CoinPackedVector *drlo,
							   CoinPackedVector *drup)
{
	int i;
	nrow_ = nrow;
	ncol_ = ncol;

	// store number stages
	nstag_ = nstag;

	nColInStage_ = new int[nstag_+1];
	nRowInStage_ = new int[nstag_+1];


	colStage_ = new int[ncol_];
	colEx2In_ = new int[ncol_];
	colIn2Ex_ = new int[ncol_];

	rowStage_ = new int[nrow_];
	rowEx2In_ = new int[nrow_];
	rowIn2Ex_ = new int[nrow_];
	
	// store stage maps
	CoinDisjointCopyN(cstag,ncol_,colStage_);
	CoinDisjointCopyN(rstag,nrow_,rowStage_);

	// zero stage counts
	for (i=0;i<nstag_+1;i++)
	{
		nColInStage_[i] = 0;
		nRowInStage_[i] = 0;
	}
		
	// array to point to start of new stage
	stageRowPtr_ = new int[nstag_+1];

	// count rows in each stage
	for (i=0;i<nrow_;i++)
		nRowInStage_[rowStage_[i]]++;

	// set stage pointers
	stageRowPtr_[0] = 0;
	for (i=0;i<nstag_;i++)
		stageRowPtr_[i+1] = stageRowPtr_[i] + nRowInStage_[i];

	// place index into next open position in its stage
	for (i=0;i<nrow_;i++)
	{
		rowEx2In_[i] = stageRowPtr_[rowStage_[i]];
		rowIn2Ex_[rowEx2In_[i]] = i;
		stageRowPtr_[rowStage_[i]]++;
	}

	// reset stage pointers
	stageRowPtr_[0] = 0;
	for (i=0;i<nstag_;i++)
		stageRowPtr_[i+1] = stageRowPtr_[i] + nRowInStage_[i];

			
	// array to point to start of new stage
	stageColPtr_ = new int[nstag_+1];

	// count cols in each stage
	for (i=0;i<ncol_;i++)
		nColInStage_[colStage_[i]]++;

	// set stage pointers
	stageColPtr_[0] = 0;
	for (i=0;i<nstag_;i++)
		stageColPtr_[i+1] = stageColPtr_[i] + nColInStage_[i];

	// place index into next open position in its stage
	for (i=0;i<ncol_;i++)
	{
		colEx2In_[i] = stageColPtr_[colStage_[i]];
		colIn2Ex_[colEx2In_[i]] = i;
		stageColPtr_[colStage_[i]]++;
	}

	// reset stage pointers
	stageColPtr_[0] = 0;
	for (i=0;i<nstag_;i++)
		stageColPtr_[i+1] = stageColPtr_[i] + nColInStage_[i];


	// make nodes

	this->nodes_.reserve(nstag_);

		// TODO: specialize this interface for core nodes


	cdclo_ = new double*[nstag_];
	cdcup_ = new double*[nstag_];
	cdobj_ = new double*[nstag_];
	cdrlo_ = new double*[nstag_];
	cdrup_ = new double*[nstag_];

	for (i=0;i<nstag_;i++)
	{
	
		SmiNodeData *node = new SmiNodeData(i,this,
			matrix,dclo,dcup,dobj,drlo,drup);

		node->setCoreNode();

		nodes_.push_back(node);

		int nrow=this->getNumRows(i);
		int ncol=this->getNumCols(i);
		int irow=this->getRowStart(i);
		int icol=this->getColStart(i);

		cdrlo_[i]=node->getRowLower().denseVector(nrow+irow)+irow;
		cdrup_[i]=node->getRowUpper().denseVector(nrow+irow)+irow;
		cdclo_[i]=node->getColLower().denseVector(ncol+icol)+icol;
		cdcup_[i]=node->getColUpper().denseVector(ncol+icol)+icol;
		cdobj_[i]=node->getObjective().denseVector(ncol+icol)+icol;
		
	}

	

}

void SmiCoreData::copyRowLower(double * d,SmiStageIndex t )
{	
	CoinDisjointCopyN(cdrlo_[t],this->getNumRows(t),d);
}
	
void SmiCoreData::copyRowUpper(double * d,SmiStageIndex t)
{	
	CoinDisjointCopyN(cdrup_[t],this->getNumRows(t),d);
}
void SmiCoreData::copyColLower(double * d,SmiStageIndex t)
{	
	CoinDisjointCopyN(cdclo_[t],this->getNumCols(t),d);
}
void SmiCoreData::copyColUpper(double * d,SmiStageIndex t)
{	
	CoinDisjointCopyN(cdcup_[t],this->getNumCols(t),d);
}
void SmiCoreData::copyObjective(double * d,SmiStageIndex t)
{	
	CoinDisjointCopyN(cdobj_[t],this->getNumCols(t),d);
}

SmiCoreData::~SmiCoreData()
{
	delete nColInStage_;
	delete nRowInStage_;
	delete stageColPtr_;
	delete stageRowPtr_;
	delete colStage_;
	delete rowStage_;
	delete colEx2In_;
	delete rowEx2In_;
	delete colIn2Ex_;
	delete rowIn2Ex_;

	for(int t=0; t<this->getNumStages(); ++t)
	{
		int irow=this->getRowStart(t);
		int icol=this->getColStart(t);
		cdrlo_[t]-=irow;
		cdrup_[t]-=irow;
		cdclo_[t]-=icol;
		cdcup_[t]-=icol;
		cdobj_[t]-=icol;
		delete cdrlo_[t];
		delete cdrup_[t];
		delete cdclo_[t];
		delete cdcup_[t];
		delete cdobj_[t];
	}
	
	delete cdrlo_;
	delete cdrup_;
	delete cdclo_;
	delete cdcup_;
	delete cdobj_;


}

void 
SmiNodeData::setCoreNode()
{
	isCoreNode_=true;
}

#include <vector>
#include <assert.h>

using namespace std;

// constructor from LP data
// TODO: allow for special node data like integer variables not in core, etc
SmiNodeData::SmiNodeData(SmiStageIndex stg, SmiCoreData *core,
				 const CoinPackedMatrix * const matrix,
				 CoinPackedVector *dclo, 
				 CoinPackedVector *dcup,
				 CoinPackedVector *dobj,
				 CoinPackedVector *drlo, 
				 CoinPackedVector *drup)
{
	// initialize specialized core node info
	isCoreNode_ = false;

	core_ = core;
	stg_ = stg;
	if (matrix)
		nels_ = matrix->getNumElements();
	else
		nels_ = 0;

	int i;
	int nrow = core->getNumRows(stg_);
	int ncol = core->getNumCols(stg_);

		
	if (matrix && nels_>0)
	{
		// should already be done but no harm checking
		assert(!matrix->isColOrdered());
		
		// pick up and store rows belonging to node's stage
		// TODO: is this a fast way to do this?
		for (i=core->getRowStart(stg); i<core->getRowStart(stg+1) ; i++)
		{
			int irow = core->getRowExternalIndex(i);
			if (irow < matrix->getNumRows())
			{
				const CoinShallowPackedVector row = matrix->getVector(irow);
				if (row.getNumElements())
				{
					CoinPackedVector *stored = new CoinPackedVector(
						row.getNumElements(), row.getIndices(), row.getElements(),
						false);
					int *indx = stored->getIndices();
					// revise indices
					for(int j=0;j<stored->getNumElements();j++)
					{
						int t= core->getColStage(indx[j]);
						indx[j] = core->getColInternalIndex(indx[j]);
						// TODO: message about row stage incompatible with col stage
						assert(!( t > stg));
					}
					//TODO: this is nice for the addNode function but is it too expensive?
					stored->sortIncrIndex();

					//TODO: is map a good container?
					this->rowMap.insert(SmiRowMap::value_type(i,stored));
				}
			}
		}
	}
	
	const int *ind;
	const double *elt;

	if (dclo && dclo->getNumElements())
	{
		//this->dclo_ = new CoinPackedVector(false);
		this->getMutableColLower().reserve(ncol);
		ind = dclo->getIndices();
		elt = dclo->getElements();
		// TODO: is this a fast way to do this?
		for (i=0; i<dclo->getNumElements(); i++)
		{
			int icol = ind[i];
			if ( core->getColStage(icol) == stg)
				this->getMutableColLower().insert(core->getColInternalIndex(icol),elt[i]);
		}
	}

	
	if (dcup && dcup->getNumElements())
	{
		//this->dcup_ = new CoinPackedVector(false);
		this->getMutableColUpper().reserve(ncol);
		ind = dcup->getIndices();
		elt = dcup->getElements();
		for (i=0; i<dcup->getNumElements(); i++)
		{
			int icol = ind[i];
			if ( core->getColStage(icol) == stg)
				this->getMutableColUpper().insert(core->getColInternalIndex(icol),elt[i]);
		}
	}	

	
	if (dobj && dobj->getNumElements())
	{
		//this->dobj_ = new CoinPackedVector(false);
		this->getMutableObjective().reserve(ncol);
		ind = dobj->getIndices();
		elt = dobj->getElements();
		for (i=0; i<dobj->getNumElements(); i++)
		{
			int icol = ind[i];
			if ( core->getColStage(icol) == stg)
				this->getMutableObjective().insert(core->getColInternalIndex(icol),elt[i]);
		}
	}	

	
	if (drlo && drlo->getNumElements())
	{
		//this->drlo_ = new CoinPackedVector(false);
		this->getMutableRowLower().reserve(nrow);
		ind = drlo->getIndices();
		elt = drlo->getElements();
		for (i=0; i<drlo->getNumElements(); i++)
		{
			int icol = ind[i];
			if ( core->getRowStage(icol) == stg)
				this->getMutableRowLower().insert(core->getRowInternalIndex(icol),elt[i]);
		}
	}


				
	if (drup && drup->getNumElements())
	{
		//this->drup_ = new CoinPackedVector(false);

		this->getMutableRowUpper().reserve(nrow);
		ind = drup->getIndices();
		elt = drup->getElements();
		for (i=0; i<drup->getNumElements(); i++)
		{
			int icol = ind[i];
			if ( core->getRowStage(icol) == stg)
				this->getMutableRowUpper().insert(core->getRowInternalIndex(icol),elt[i]);
		}
	}
}


CoinPackedVector * SmiNodeData::combineWithCoreRow(CoinPackedVector *cr, CoinPackedVector *nr)
{
	return getCoreCombineRule()->Process(cr,nr);
}

void SmiNodeData::combineWithCoreDoubleArray(double *d_out, const CoinPackedVector &cpv, int o)
{
	if (!isCoreNode_)
		getCoreCombineRule()->Process(d_out,o,cpv);
}

void SmiNodeData::copyRowLower(double * drlo)
{
	int t=getStage();
	getCore()->copyRowLower(drlo,t);
	combineWithCoreDoubleArray(drlo,getRowLower(),getCore()->getRowStart(t));	
}

void SmiNodeData::copyRowUpper(double * d){
	int t=getStage();
	getCore()->copyRowUpper(d,t);
	combineWithCoreDoubleArray(d,getRowUpper(),getCore()->getRowStart(t));
}

void SmiNodeData::copyColLower(double * d){
	int t=getStage();
	getCore()->copyColLower(d,t);
	combineWithCoreDoubleArray(d,getColLower(),getCore()->getColStart(t));
}

void SmiNodeData::copyColUpper(double * d){
	int t=getStage();
	getCore()->copyColUpper(d,t);
	combineWithCoreDoubleArray(d,getColUpper(),getCore()->getColStart(t));
}

void SmiNodeData::copyObjective(double * d){
	int t=getStage();
	getCore()->copyObjective(d,t);
	combineWithCoreDoubleArray(d,getObjective(),getCore()->getColStart(t));
}


SmiNodeData::~SmiNodeData()
{

	
}

