/*  _________________________________________________________________________
 *
 *  UTILIB: A utility library for developing portable C++ codes.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top UTILIB directory.
 *  _________________________________________________________________________
 */

//
// MultiLL.cpp
//
// Defines methods for the MultiLL class.
//

#include <utilib/_math.h>
#include <utilib/MultiLL.h>
#include <utilib/CommonIO.h>

namespace utilib {

#define OPEN -1

//=============================================================
// Constructor for MultiLL
//=============================================================
MultiLL::MultiLL() {}

//=============================================================
// Destructor for MultiLL
//=============================================================
MultiLL::~MultiLL() {}

//=============================================================
// Method:  set_up
// Purpose: Various initialization
//=============================================================
void MultiLL::set_up(int number_of_types,int number_of_entries_per)
{
 MAXTYPES     = number_of_types;
 // MAXTABLESIZE = number_of_entries_per*MAXTYPES+1;
MAXTABLESIZE = number_of_entries_per+1;

 resize();

 int i;
 for(i=0; i< MAXTABLESIZE; i++)
   {
    timestamp   << 0;
    Reserved[i] = 0;
    Next[i]     = OPEN;
    Prev[i]     = OPEN;
    table[i]    = OPEN;
   }
 for(i=0; i< MAXTYPES; i++)
   {
    First[i] = OPEN;
    Last[i]  = OPEN;
   }

}

//=============================================================
// Method:  
// Purpose: 
//          
//=============================================================
void MultiLL::resize()
{
 table.resize(MAXTABLESIZE);
 Reserved.resize(MAXTABLESIZE);
 Next.resize(MAXTABLESIZE);
 Prev.resize(MAXTABLESIZE);
 First.resize(MAXTYPES);
 Last.resize(MAXTYPES);
 timestamp.resize(MAXTABLESIZE);
}


//=============================================================
// returns the table index of the earliest free spot in table
//=============================================================
int MultiLL::next_free()
{
 size_type i=0;
 int free_index;



 // while((table[i] != OPEN)||(Reserved[i]==1)) i++;
 while(table[i] != OPEN) i++;
 
 if(i==table.size()){
   while(1)
     EXCEPTION_MNGR(runtime_error, "MultiLL::next_free - Table is full")
 }


 free_index=i;



 return free_index;
}




//=============================================================
// Method:  find_value
// Purpose: returns 1 if the value sought is found in the
//          chain referenced, returns 0 otherwise
//=============================================================
int MultiLL::find_value(int type_id,int value)
{
 int found_index=First[type_id];
 


 if(found_index != OPEN){
   while((found_index != OPEN)&&(table[found_index] != value))
       found_index=Next[found_index];
 }



 return found_index;
}

//=============================================================
// Method:  insert
// Purpose: inserts a new value into available slot in
//          the value table and manages chains. returns insertion
//          index
//=============================================================
int MultiLL::insert(int type_id, int value, int expiration)
{
  int current;   



  if(First[type_id]==OPEN)
  {
   First[type_id] = next_free();    // begin chain for this type at free slot
   current        = First[type_id];
   Prev[current]  = OPEN;           // null previous ptr
  }
  else
  {
   current=Last[type_id];           // maintain chain around last entry
   Next[current]=next_free();
   Prev[Next[current]]=current; // point next slot to current last 
   current=Next[current];
  }
  table[current]=value;      // assign entry
  Last[type_id]=current;         // assign last in chain ptr
  //  Next[current]=next_free();
  Next[current]=OPEN;
  //  Reserved[Next[current]]=1;  



  timestamp[current]=expiration;


  return current;
}

//=============================================================
// Method:  remove_value
// Purpose: removes entry from value table 
//          and manages chain
//=============================================================
int MultiLL::remove_value(int type_id, int value)
{
 int candidate=find_value(type_id,value);
 


 if(candidate!=OPEN)
  {

    if(candidate==First[type_id])
      {

	if(Next[candidate]!=OPEN)
	  {
	    First[type_id] = Next[candidate];
	  }
	else
	  {
	    First[type_id] = OPEN;
	    Last[type_id]  = OPEN;
	  }

      }
    else{


       if(candidate==Last[type_id]){
         Next[Prev[candidate]]=OPEN;
         Last[type_id]=Prev[candidate];
       }
       else{ 
	 Next[Prev[candidate]] = Next[candidate];
         Prev[Next[candidate]] = Prev[candidate];
       }

    }
    table[candidate]      = OPEN;
    Reserved[candidate]   = 0;  
    Next[candidate]       = OPEN;
    Prev[candidate]       = OPEN;
   }




 return candidate;
}

//=============================================================
// Method:  remove_index
// Purpose: removes entry at the given index from value table 
//          and manages chain
//=============================================================
void MultiLL::remove_index(int type_id, int remove_index)
{
 int candidate=remove_index;
 



 if(candidate!=OPEN)
  {

    if(candidate==First[type_id])
      {  // first

       if(Next[candidate]!=OPEN) // Next not open
	{
         Prev[Next[candidate]]=OPEN;
         First[type_id] = Next[candidate];
	}
        else                      // next open
	{

         First[type_id] = OPEN;
         Last[type_id]  = OPEN;
	}

      }
    else{    // last

       if(candidate==Last[type_id]){

         Next[Prev[candidate]]=OPEN;
         Last[type_id]=Prev[candidate];

       }
       else{ // in the middle

	 Next[Prev[candidate]] = Next[candidate];
         Prev[Next[candidate]] = Prev[candidate];

       }

    }

    table[candidate]      = OPEN;
    Reserved[candidate]   = 0;  
    Next[candidate]       = OPEN;
    Prev[candidate]       = OPEN;

   }



} 

//=============================================================
// Method:  
// Purpose: 
//          
//=============================================================
void MultiLL::clean_up(int current_iteration)
{
  int check_index;

  for(int i=0;i<MAXTYPES;i++)
    {
      check_index=First[i];
      if((check_index > -1) &&
	 (timestamp[check_index] <= current_iteration) &&
         (timestamp[check_index] > 0))
	{
          EXCEPTION_MNGR(runtime_error, "MultiLL::clean_up - Tabu expired on dimension " << i 
               << " value " << table[check_index])
	  remove_index(i,check_index);
	  check_index=Next[check_index];
	}
    }
}

//=============================================================
// Method:  
// Purpose: 
//          
//=============================================================
void MultiLL::show_list(int type_id)
{
  
 int candidate=First[type_id];

 ucout << "Dimension " << type_id << ": ";
 while(candidate !=OPEN)
   {
     ucout << table[candidate] << " ";   
     candidate=Next[candidate];
   }
 ucout << endl;


}

//=============================================================
// Method:  
// Purpose: 
//          
//=============================================================
int MultiLL::sublist_empty(int type_id)
{
  
 return First[type_id];
  
}

} // namespace utilib
