#ifndef RESULT_H
#define RESULT_H

#include <vector>

using namespace std;

class Vertex;
class Edge;

double hr2min(double hr);
double min2hr(double min);

class RData {	// Result data
public:
	RData(double consumed) 
	{ 
		consumedBeforeAlarmAt = consumed;
	}
	virtual ostream& print(ostream& os) const { return os;}
	virtual ostream& printLoc(ostream& os) const { return os;}
	virtual ostream& printVC(ostream& os) const { return os;}
	virtual int      getID() = 0; 
	friend ostream& operator<<(ostream& os, const RData& p) 
	{	
		return p.print(os);
	}
	double consumedBeforeAlarmAt;
};

class VRData : public RData {
public:
	int index;

	VRData(int vv, double consumed)
	  : RData(consumed), index(vv)  
	{}

	int      getID()  { return index; }
	ostream& print(ostream& os) const
	{
		os << index << " " << consumedBeforeAlarmAt;
		return os;
	}
	ostream& printLoc(ostream& os) const
	{
		os << index;
		return os;
	}
	ostream& printVC(ostream& os) const
	{
		os << consumedBeforeAlarmAt;
		return os;
	}
};

class SRData : public RData {
public:
	int svid;

	SRData(int sv, double consumed)
	  : RData(consumed), svid(sv)  
	{	
	}

	int      getID()  { return svid; }

	ostream& print(ostream& os) const
	{
		os << svid << " " << consumedBeforeAlarmAt;
		return os;
	}
	ostream& printLoc(ostream& os) const
	{
		os << svid;
		return os;
	}
	ostream& printVC(ostream& os) const
	{
		os << consumedBeforeAlarmAt;
		return os;
	}
};

class DummyRData : public RData {
public:
	DummyRData(double consumed, int n)
			: RData(consumed), num_network_locs(n)
	{}
	ostream& print(ostream& os) const
	{
		os << "dummy" << " " << consumedBeforeAlarmAt;
		return os;
	}
	int 	 getID() { return num_network_locs+1; }
	ostream& printLoc(ostream& os) const
	{
		os << num_network_locs+1;
		return os;
	}
	ostream& printVC(ostream& os) const
	{
		os << consumedBeforeAlarmAt;
		return os;
	}
	int num_network_locs;
};

typedef RData* RDataPtr;

/*
namespace std {
template<> struct less<RDataPtr>
{
	bool operator()(const RDataPtr a, const RDataPtr b) 
	{
		cout << "called!" << endl;
    		if (a->getID() - b->getID() < 0) return true;
		return false;
	}
};

};

struct ltvec
{
  bool operator()(const vector<RDataPtr>& v1, const vector<RDataPtr>& v2) const
  {
	int v1s = v1.size();
	int v2s = v2.size();
	bool done = false;
	int i=0;
	while (!done) {
		if (i<v1s && i<v2s) {
			RDataPtr a1 = v1[i];
			RDataPtr a2 = v2[i];
    			if (a1->data - a2->data < 0) return true;
    			else if (a1->data - a2->data > 0) return false;
		} else if (i<v1s && i>=v2s)
			return true;
	
	}
	return false;
  }
};
*/
#endif
