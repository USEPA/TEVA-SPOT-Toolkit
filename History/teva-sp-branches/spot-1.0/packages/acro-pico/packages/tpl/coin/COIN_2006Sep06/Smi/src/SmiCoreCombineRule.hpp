// Copyright (C) 2004, International Business Machines
// Corporation and others.  All Rights Reserved.
//
// SmiCoreCombineRule.hpp: interface for the SmiCoreCombineRule classes.
//
//
// Alan King 
// 9 March 2004
//////////////////////////////////////////////////////////////////////

#if !defined(SmiCoreCombineRule_HPP)
#define SmiCoreCombineRule_HPP

#if defined(_MSC_VER)
// Turn off compiler warning about long names
#  pragma warning(disable:4786)
#endif

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CoinPackedVector.hpp"
#include <string>

//////////////////////////////////////////////////////////////////////
// SmiCoreCombineRule
// -- abstract base class
//////////////////////////////////////////////////////////////////////
class SmiCoreCombineRule  
{
public:
	virtual void Process(double *d1, int o1, const CoinPackedVector &cpv2, char *type=0)=0;
	virtual CoinPackedVector * Process(CoinPackedVector *cpv1, CoinPackedVector *cpv2, char *type=0)=0;
	virtual ~SmiCoreCombineRule(){};
};

//////////////////////////////////////////////////////////////////////
// SmiCoreCombineReplace
// -- concrete "replace" rule -- singleton pattern
//////////////////////////////////////////////////////////////////////
class SmiCoreCombineReplace : public SmiCoreCombineRule
{
public:
	static SmiCoreCombineReplace * Instance();
	virtual void Process(double *d1, int o1, const CoinPackedVector &cpv2, char *type=0);
	virtual CoinPackedVector * Process(CoinPackedVector *cpv1, CoinPackedVector *cpv2, char *type=0);
protected:
	SmiCoreCombineReplace(){}
private:
	static SmiCoreCombineReplace * _instance;
};

//////////////////////////////////////////////////////////////////////
// SmiCoreCombineAdd
// -- concrete "add" rule -- singleton pattern
//////////////////////////////////////////////////////////////////////
class SmiCoreCombineAdd : public SmiCoreCombineRule
{
public:
	static SmiCoreCombineAdd * Instance();
	virtual void Process(double *d1, int o1, const CoinPackedVector &cpv2, char* type=0);
	virtual CoinPackedVector * Process(CoinPackedVector *cpv1, CoinPackedVector *cpv2, char* type=0);
protected:
	SmiCoreCombineAdd(){}
private:
	static SmiCoreCombineAdd * _instance;
};


#endif // !defined(SmiCoreCombineRule_HPP)
