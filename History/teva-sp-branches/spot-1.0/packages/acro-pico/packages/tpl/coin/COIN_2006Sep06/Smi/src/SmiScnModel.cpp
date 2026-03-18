#include "SmiDiscreteDistribution.hpp"
#include "SmiScenarioTree.hpp"
#include "SmiScnModel.hpp"
#include "SmiSmpsIO.hpp"
#include "CoinPackedMatrix.hpp"
#include "OsiSolverInterface.hpp"
#include "CoinHelperFunctions.hpp"
#include "CoinError.hpp"
#include <assert.h>
#include <algorithm>

using namespace std;


int 
SmiScnNode::getCoreColIndex(int i)
{
	SmiCoreData *core = node_->getCore();
	return core->getColExternalIndex(i-coffset_+core->getColStart(node_->getStage()));
}

int 
SmiScnNode::getCoreRowIndex(int i){
	SmiCoreData *core = node_->getCore();
	return core->getRowExternalIndex(i-roffset_+core->getRowStart(node_->getStage()));
}

SmiScnModel::~SmiScnModel()
{
	if (osiStoch_)
		delete osiStoch_;
	
	if (core_)
		delete core_;
	
	if (drlo_)
		delete [] drlo_;
	
	if (drup_)
		delete [] drup_;
	
	if (dclo_)
		delete [] dclo_;
	
	if (dcup_)
		delete [] dcup_;

	if (dobj_)
		delete [] dobj_;

	if (matrix_)
		delete matrix_;
	

}

SmiScenarioIndex 
SmiScnModel::generateScenario(SmiCoreData *core, 
				CoinPackedMatrix *matrix,
				CoinPackedVector *v_dclo, CoinPackedVector *v_dcup,
				CoinPackedVector *v_dobj,
				CoinPackedVector *v_drlo, CoinPackedVector *v_drup,				
				SmiStageIndex branch, SmiScenarioIndex anc, double prob,
				SmiCoreCombineRule *r)
{

	// this coding takes branch to be the node that the scenario branches *from*
	--branch;

	vector<SmiScnNode *> node_vec;

	node_vec.reserve(core->getNumStages());

	// first scenario
	if (this->getNumScenarios()==0)
	{
		// TODO: warnings if branch and anc are not 0
		anc = 0;
		branch = 0;

		// generate root node
		SmiNodeData *node = core->getNode(0);
		SmiScnNode *tnode = new SmiScnNode(node);
		node_vec.push_back(tnode);
		this->ncol_ = core->getNumCols(0);
		this->nrow_ = core->getNumRows(0);
		this->nels_ = node->getNumElements();

	}
	else
	{
		// TODO: throw error if branch too large
		assert(branch<core->getNumStages());
	}

	// TODO: what to do about duplicate matrix entries?
	// ...can't do the following because eliminates zero entries...
	// matrix->eliminateDuplicates(0.0);

	int t;
	for (t=branch+1; t<core->getNumStages(); t++)
	{
		// generate new data node
		SmiNodeData *node = new SmiNodeData(t,core,matrix,
			v_dclo,v_dcup,v_dobj,v_drlo,v_drup);
		node->setCoreCombineRule(r);
		// generate new tree node
		SmiScnNode *tnode = new SmiScnNode(node);
		node_vec.push_back(tnode);
		
		this->ncol_ += core->getNumCols(t);
		this->nrow_ += core->getNumRows(t);
		this->nels_ += node->getNumElements();
	}


	int scen = smiTree_.addPathtoLeaf(anc,branch,node_vec);

	// add probability to all scenario nodes in path
	SmiTreeNode<SmiScnNode *> *child = smiTree_.getLeaf(scen);
	SmiTreeNode<SmiScnNode *> *parent = child->getParent();
	SmiTreeNode<SmiScnNode *> *root = smiTree_.getRoot();

	while (child != root)
	{
		SmiScnNode *tnode = child->getDataPtr();
		tnode->addProb(prob);
		tnode->setParent(parent->getDataPtr());
		child = parent;
		parent = child->getParent();
	}
	root->getDataPtr()->addProb(prob);

	this->totalProb_+=prob;

	return scen;

}
SmiScenarioIndex 
SmiScnModel::generateScenario(SmiCoreData *core, 
				CoinPackedMatrix *matrix,
				CoinPackedVector *v_dclo, CoinPackedVector *v_dcup,
				CoinPackedVector *v_dobj,
				CoinPackedVector *v_drlo, CoinPackedVector *v_drup,				
				vector<int> labels, double prob,
				SmiCoreCombineRule *r)
{

	// this code assumes that full path data (incl root node data)
	// is passed in.

	vector<SmiScnNode *> node_vec;

	node_vec.reserve(core->getNumStages());


	// TODO: what to do about duplicate matrix entries?
	// ...can't do the following because eliminates zero entries...
	// matrix->eliminateDuplicates(0.0);

	int t;
	for (t=0; t<core->getNumStages(); t++)
	{
		// generate new data node
		SmiNodeData *node = new SmiNodeData(t,core,matrix,
			v_dclo,v_dcup,v_dobj,v_drlo,v_drup);
		node->setCoreCombineRule(r);
		// generate new tree node
		SmiScnNode *tnode = new SmiScnNode(node);
		node_vec.push_back(tnode);
		
		this->ncol_ += core->getNumCols(t);
		this->nrow_ += core->getNumRows(t);
		this->nels_ += node->getNumElements();
	}

	SmiTreeNode<SmiScnNode *> *node = smiTree_.find(labels);
	int scen = smiTree_.addPathtoLeaf(node->scenario(),node->depth(),node_vec);
	smiTree_.setChildLabels(node,labels);

	// add probability to all scenario nodes in path
	SmiTreeNode<SmiScnNode *> *child = smiTree_.getLeaf(scen);
	SmiTreeNode<SmiScnNode *> *parent = child->getParent();
	SmiTreeNode<SmiScnNode *> *root = smiTree_.getRoot();

	while (child != root)
	{
		SmiScnNode *tnode = child->getDataPtr();
		tnode->addProb(prob);
		tnode->setParent(parent->getDataPtr());
		child = parent;
		parent = child->getParent();
	}
	root->getDataPtr()->addProb(prob);

	this->totalProb_+=prob;

	return scen;

}




 
OsiSolverInterface *
SmiScnModel::loadOsiSolverData()
{
	osiStoch_->reset();

	// initialize arrays
	this->dclo_ = new double[this->ncol_];
	this->dcup_ = new double[this->ncol_];
	this->dobj_ = new double[this->ncol_];
	this->drlo_ = new double[this->nrow_];
	this->drup_ = new double[this->nrow_];
	// initialize row-ordered matrix with no extragaps or extramajors
	CoinPackedMatrix *matrix = new CoinPackedMatrix(false,0,0);
	matrix->reserve(nrow_,4*nels_);
	this->matrix_=matrix;

	ncol_=0;
	nrow_=0;
	
	// loop to addNodes
	for_each(smiTree_.treeBegin(),smiTree_.treeEnd(),SmiScnModelAddNode(this));

	// pass data to osiStoch
	osiStoch_->loadProblem(CoinPackedMatrix(*matrix_),dclo_,dcup_,dobj_,drlo_,drup_);

	return osiStoch_;
}




void 
SmiScnModel::addNode(SmiScnNode *tnode)
{

	SmiNodeData *node = tnode->getNode();

	// set offsets for current node
	tnode->setColOffset(ncol_);
	tnode->setRowOffset(nrow_);

	// OsiSolverInterface *osi = this->osiStoch_;
	SmiCoreData *core = node->getCore();

	// get stage and associated core node
	int stg = node->getStage();
	SmiNodeData *cnode = core->getNode(stg);

	core->copyRowLower(drlo_+nrow_,stg);
	core->copyRowUpper(drup_+nrow_,stg);
	core->copyColLower(dclo_+ncol_,stg);
	core->copyColUpper(dcup_+ncol_,stg);
	core->copyObjective(dobj_+ncol_,stg);

	node->copyColLower(dclo_+ncol_);
	node->copyColUpper(dcup_+ncol_);
	node->copyObjective(dobj_+ncol_);
	node->copyRowLower(drlo_+nrow_);
	node->copyRowUpper(drup_+nrow_);
	
	// multiply obj coeffs by node probability and normalize
	double prob = tnode->getProb()/this->totalProb_;
	tnode->setModelProb(prob);

	for(int j=ncol_; j<ncol_+core->getNumCols(stg); ++j)
		dobj_[j] *= prob;
	
	
	// add rows to matrix
	for (int i=core->getRowStart(stg); i<core->getRowStart(stg+1) ; i++)
	{
		// get node rows
		CoinPackedVector *cr = cnode->getRow(i);
		if (stg)
		{
			CoinPackedVector *newrow = node->combineWithCoreRow(cr,node->getRow(i));

			// TODO: this is probably a throwable error
			if (!newrow)
				continue;
			
			
			// coefficients of new row
			int *indx = newrow->getIndices();
			
			// stage starts
			int t=stg;
			int jlo=core->getColStart(stg);
			
			// net offset to be added to indices
			int coff = ncol_-jlo;
			
			if(coff)
			{
				// parent node
				SmiScnNode *pnode=tnode;
				
				// main loop iterates backwards through indices
				for (int j=newrow->getNumElements()-1; j>-1;--j)
				{
					// get new offset from parent node when crossing stage bndy
					while (indx[j]<jlo)
					{
						jlo = core->getColStart(--t);
						pnode=pnode->getParent();
						coff = pnode->getColStart()-jlo;
					}
					
					// add offset to index
					indx[j]+=coff;
				}
			}
			matrix_->appendRow(*newrow);
		}
		else
			matrix_->appendRow(*cr);

	}

	// update row, col counts
	ncol_ += core->getNumCols(stg);
	nrow_ += core->getNumRows(stg);

	// for debug sanity
	int mnrow = matrix_->getNumRows();
	int mncol = matrix_->getNumCols();
	assert(mnrow == nrow_);
	assert(mncol == ncol_);
}

OsiSolverInterface *
SmiScnModel::getOsiSolverInterface()
{
	return osiStoch_;
}

const double *
SmiScnModel::getColSolution(int ns)
{
	return NULL;
}

const double *
SmiScnModel::getRowSolution(int ns)
{
	return NULL;
}

int
SmiScnModel::readSmps(const char *c, SmiCoreCombineRule *r)
{
	int i;
	SmiSmpsIO smiSmpsIO;

	if (r != NULL)
		smiSmpsIO.setCoreCombineRule(r);

	const char* core_ext[] = {"core", "cor"};
	for (i = sizeof(core_ext)/sizeof(const char*) - 1; i >= 0; --i) {
		if (smiSmpsIO.readMps(c,core_ext[i]) >= 0)
			break;
	}
	if (i == -1)
		return -1;

	SmiCoreData *smiCore = NULL;
	const char* time_ext[] = {"time", "tim"};
	for (i = sizeof(time_ext)/sizeof(const char*) - 1; i >= 0; --i) {
		smiCore = smiSmpsIO.readTimeFile(this,c,time_ext[i]);
		if (smiCore)
			break;
	}
	if (i == -1)
		return -1;

	const char* stoch_ext[] = {"stoch", "stoc", "sto"};
	for (i = sizeof(stoch_ext)/sizeof(const char*) - 1; i >= 0; --i) {
		if (smiSmpsIO.readStochFile(this,smiCore,c,stoch_ext[i]) >= 0)
			break;
	}
	if (i == -1)
		return -1;

	return 0;
}
  
void replaceFirstWithSecond(CoinPackedVector &dfirst, const CoinPackedVector &dsecond)
{
	double *delt1 = dfirst.getElements();
	const double *delt2 = dsecond.getElements();
	const int *indx2 = dsecond.getIndices();
	for(int j=0;j<dsecond.getNumElements();++j)
				delt1[dfirst.findIndex(indx2[j])] = delt2[j];
}

void
SmiScnModel::processDiscreteDistributionIntoScenarios(SmiDiscreteDistribution *smiDD, bool test)

{
	SmiCoreData *core=smiDD->getCore();
	
	int nindp = smiDD->getNumRV();
	int nstages = 1;

	if (test)
	{
		nstages = 3;
		assert(nindp==4);
	}
	else
	{
		nstages = core->getNumStages();
		assert(nindp > 0);
	}

	
	int ns=1;
	double dp=1.0;
	
	CoinPackedMatrix matrix ;
	CoinPackedVector cpv_dclo ;
	CoinPackedVector cpv_dcup ;
	CoinPackedVector cpv_dobj ;
	CoinPackedVector cpv_drlo ;
	CoinPackedVector cpv_drup ;
	
	cpv_dclo.setTestForDuplicateIndex(true);
	cpv_dcup.setTestForDuplicateIndex(true);
	cpv_dobj.setTestForDuplicateIndex(true);
	cpv_drlo.setTestForDuplicateIndex(true);
	cpv_drup.setTestForDuplicateIndex(true);
	
	// initialize data for first scenario
	vector<int> indx(nindp);
	vector<int> nsamp(nindp);
	vector<int> label(nstages);
	vector<int>::iterator iLabel;
	
	for (iLabel=label.begin(); iLabel<label.end(); ++iLabel)
		*iLabel=0;
	
	int jj;
	for (jj=0;jj<nindp;jj++) {
		SmiDiscreteRV *smiRV = smiDD->getDiscreteRV(jj);
		
		indx[jj] = 0;
		nsamp[jj] = smiRV->getNumEvents();
		ns *= nsamp[jj];
		dp *= smiRV->getEventProb(indx[jj]);

		if (test) 
		{
			double p=0.5*nsamp[jj]*(nsamp[jj]+1);
			assert(smiRV->getEventProb(indx[jj])==(indx[jj]+1)/p);
		}
		
		
		cpv_dclo.append(smiRV->getEventColLower(indx[jj]));
		cpv_dcup.append(smiRV->getEventColUpper(indx[jj]));
		cpv_dobj.append(smiRV->getEventObjective(indx[jj]));
		cpv_drlo.append(smiRV->getEventRowLower(indx[jj]));
		cpv_drup.append(smiRV->getEventRowUpper(indx[jj]));
		
		//TODO test smiModel code
		CoinPackedMatrix m = smiRV->getEventMatrix(indx[jj]);
		assert(!m.isColOrdered());
		if (matrix.getNumElements())
		{
			for (int i=0; i<m.getNumRows(); ++i)
			{
				CoinPackedVector row=m.getVector(i);
				CoinPackedVector rrow=matrix.getVector(i);
				for (int j=m.getVectorFirst(i); j<m.getVectorLast(j); ++j)
				{
					matrix.modifyCoefficient(i,j,row[j],true);
				}
			}
		}
		else
			matrix = m;
		
    }


	// first scenario
	int anc = 0;
	int branch = 1;
	int	is = 0;
	
	if (!test) 
		is=this->generateScenario(core,&matrix,&cpv_dclo,&cpv_dcup,&cpv_dobj,
						&cpv_drlo,&cpv_drup,branch,anc,dp);
	else
	{
		assert(matrix.getNumElements()==4);
		assert(cpv_dclo.getNumElements()==4);
		for (int j=0;j<nindp;j++)
		{
			assert(cpv_dclo.getIndices()[j]==j);
			assert(cpv_drlo.getIndices()[j]==indx[j]);
			assert(matrix.getCoefficient(indx[j],j)==(double)(j*indx[j]));
		}
	}

	
	
	SmiTreeNode<SmiScnNode *> *root = this->smiTree_.getRoot();
	this->smiTree_.setChildLabels(root,label);
	
	/* sample space increment initialized to 1 */
    int *incr = (int *) malloc( nindp*sizeof(int) );
    for (jj=0;jj<nindp;jj++) incr[jj] = 1;
	
	/***** ...main loop to generate scenarios from discrete random variables
	For each scenario index ii:
	If the sample size nsamp[jj] divides the scenario index ii,
	reverse the increment direction incr[jj]
	and increase the random variable index jj by 1.
	Increment the jj'th random variable by incr[jj]
	and generate new sample data.
    ***** */
	
    for (int iss=1;iss<ns;iss++) {
		int iii=iss; jj=0;
		while ( !(iii%nsamp[jj]) ) {
			iii /= nsamp[jj];
			incr[jj] = -incr[jj];
			jj++;
		}
		
		SmiDiscreteRV *smiRV = smiDD->getDiscreteRV(jj);
		
		dp /= smiRV->getEventProb(indx[jj]);
		indx[jj] += incr[jj];
		dp *= smiRV->getEventProb(indx[jj]);

		if (test) 
		{
			double p=0.5*nsamp[jj]*(nsamp[jj]+1);
			assert(smiRV->getEventProb(indx[jj])==(indx[jj]+1)/p);
		}

		for (iLabel=label.begin(); iLabel<label.end(); ++iLabel)
			*iLabel=0;

		for(int jjj=0; jjj<nindp; jjj++)
		{	
			SmiDiscreteRV *s = smiDD->getDiscreteRV(jjj);
			
			label[s->getStage()] *= s->getNumEvents();
			label[s->getStage()] += indx[jjj];
		}

		
		// set data
		//TODO -- should we declare NULL entries to have 0 entries?  
		//this would eliminate these tests
		replaceFirstWithSecond(cpv_dclo,smiRV->getEventColLower(indx[jj]));
		replaceFirstWithSecond(cpv_dcup,smiRV->getEventColUpper(indx[jj]));
		replaceFirstWithSecond(cpv_dobj,smiRV->getEventObjective(indx[jj]));
		replaceFirstWithSecond(cpv_drlo,smiRV->getEventRowLower(indx[jj]));
		replaceFirstWithSecond(cpv_drup,smiRV->getEventRowUpper(indx[jj]));
		
		//TODO test this code
		CoinPackedMatrix m = smiRV->getEventMatrix(indx[jj]);
		assert(!m.isColOrdered());
		if (matrix.getNumElements())
		{
			for (int i=0; i<m.getNumRows(); ++i)
			{
				CoinPackedVector row=m.getVector(i);
				CoinPackedVector rrow=matrix.getVector(i);
				for (int j=m.getVectorFirst(i); j<m.getVectorLast(j); ++j)
				{
					matrix.modifyCoefficient(i,j,row[j],true);
				}
			}
		}
		else
			matrix = m;		
		
		// find ancestor node
		SmiTreeNode<SmiScnNode *> *tnode = this->smiTree_.find(label);
		anc = tnode->scenario();
		branch = tnode->depth();
		if (!test) 
		{
			is = this->generateScenario(core,&matrix,&cpv_dclo,&cpv_dcup,&cpv_dobj,&cpv_drlo,&cpv_drup,branch,anc,dp);
		}
		else
		{	
			assert(matrix.getNumElements()==4);
			assert(cpv_dclo.getNumElements()==4);
			for (int j=0;j<nindp;j++)
			{
				assert(cpv_dclo.getIndices()[j]==j);
				assert(cpv_drlo.getIndices()[j]==indx[j]);
				assert(matrix.getCoefficient(j,indx[j])==(double)(j*indx[j]));
			}
		}
		
		this->smiTree_.setChildLabels(tnode,label);
		
	}
	
	free (incr);
}






