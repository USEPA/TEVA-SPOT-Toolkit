// SmiCoreCombineRule.cpp: implementation of the SmiCoreCombineRule class.
//
//////////////////////////////////////////////////////////////////////

#include "SmiCoreCombineRule.hpp"
#include "CoinPackedVector.hpp"
#include "CoinHelperFunctions.hpp"

//////////////////////////////////////////////////////////////////////
// SmiCoreCombineReplace
//////////////////////////////////////////////////////////////////////
SmiCoreCombineReplace *SmiCoreCombineReplace::_instance = 0;

SmiCoreCombineReplace* SmiCoreCombineReplace::Instance()
{
	if (_instance==0) {
		_instance = new SmiCoreCombineReplace;
	}
	return _instance;
}

CoinPackedVector *  SmiCoreCombineReplace::Process(CoinPackedVector *cr,CoinPackedVector *nr, char *type)
{	
	
	
	CoinPackedVector *newrow=NULL;

	if (!cr && nr)
	{
		newrow = new CoinPackedVector(*nr);
	}

	if (cr && !nr)
	{
		newrow = new CoinPackedVector(*cr);
	}

	
	if (cr && nr)
	{
		// merge using denseVector

		// get max entries
		int maxentries = CoinMax(cr->getMaxIndex(),nr->getMaxIndex());
		
		double* dense = cr->denseVector(maxentries+1);
		double* elt_nr = nr->getElements();
		int* ind_nr = nr->getIndices();
		
		int j;

			for (j=0; j<nr->getNumElements(); ++j)
			{
				dense[ind_nr[j]] = elt_nr[j];
			}

		// generate new packed vector
		newrow = new CoinPackedVector();

		for (int i=0; i<maxentries+1; ++i)
		{
			if (dense[i])
				newrow->insert(i,dense[i]);
		}

		delete [] dense;
	}

	return newrow;

	
}

void SmiCoreCombineReplace::Process(double *d, int o, const CoinPackedVector &cpv, char *type)
{
	const double *cd = cpv.getElements();
	const int *ci = cpv.getIndices();
	
	for (int j=0; j < cpv.getNumElements(); ++j) 
		d[ci[j]-o] = cd[j];

}

//////////////////////////////////////////////////////////////////////
// SmiCoreCombineReplace
//////////////////////////////////////////////////////////////////////

SmiCoreCombineAdd *SmiCoreCombineAdd::_instance = 0;

SmiCoreCombineAdd* SmiCoreCombineAdd::Instance()
{
	if (_instance==0) {
		_instance = new SmiCoreCombineAdd;
	}
	return _instance;
}

CoinPackedVector *SmiCoreCombineAdd::Process(CoinPackedVector *cr,CoinPackedVector *nr, char *type)
{
	CoinPackedVector *newrow=NULL;

	if (!cr && nr)
	{
		newrow = new CoinPackedVector(*nr);
	}

	if (cr && !nr)
	{
		newrow = new CoinPackedVector(*cr);
	}

	
	if (cr && nr)
	{
		newrow = new CoinPackedVector(*cr + *nr);
	}

	return newrow;
}

void SmiCoreCombineAdd::Process(double *d1, int o1, const CoinPackedVector &cpv2, char *type)
{
	const double *cd = cpv2.getElements();
	const int *ci = cpv2.getIndices();
	
	for (int j=0; j < cpv2.getNumElements(); ++j) 
		d1[ci[j]-o1] += cd[j];

}

