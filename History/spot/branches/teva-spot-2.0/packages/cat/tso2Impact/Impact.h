
#ifndef tevasp_Impact_h
#define tevasp_Impact_h

///
class Impact
{
public:

  /// The node id
  int nodeID;

  /// The value of the impact at that node
  double impact;

  /// The time of detection (in minutes)
  double timeOfDetection;
};

#ifdef IMPOSE_IMPACT_NODE_ORDER

// imposes a global node order - sort by 
// impact first, and node ID second.
namespace std
{
  ///
  template <> struct less<Impact>
  {
    ///
    bool operator()(const Impact &a, const Impact &b)const
    {
      if(a.impact<b.impact)
	{
	  return true;
	}
      else if(a.impact>b.impact)
	{
	  return false;
	}
      else if(a.nodeID==UNDETECTED_NODE_INDEX)
	{
	  return false;
	}
      else if(b.nodeID==UNDETECTED_NODE_INDEX)
	{
	  return true;
	}
      else
	{
	  return a.nodeID<b.nodeID;
	}

      // UNREACHABLE
    }
  };
}

#endif

#endif
