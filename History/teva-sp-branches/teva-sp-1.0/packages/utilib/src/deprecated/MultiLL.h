/*  _________________________________________________________________________
 *
 *  UTILIB: A utility library for developing portable C++ codes.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top UTILIB directory.
 *  _________________________________________________________________________
 */

/**
 * \file MultiLL.h
 *
 * \depricated
 */

#ifndef utilib_MultiLL_h
#define utilib_MultiLL_h

#include <utilib/IntVector.h>
#include <utilib/Uniform.h>

namespace utilib {

//=========================================================================
// MLL.h - Class for static implementation of multiple linked lists
//             within shared data structure 
//=========================================================================
// PRELIMINARY DESCRIPTION:
// This class is meant to provide a single set of structures that supports
// the management of multiple linked lists that are dynamic in length.  
// Housekeeping methods keep the structures from growing too sparse, and 
// users may choose the level of housekeeping enforced in the structures


class UTILIB_API MultiLL
{
 public:

    MultiLL();

    ~MultiLL();

    // returns 1 if the value sought is found in the 
    // chain referenced, returns 0 otherwise
    int find_value(int type_id,int value);

    // inserts a new value into available slot in
    // the value table and manages chains
    int insert(int type_id, int value,int expiration);


    void remove_index(int type_id, int remove_index);


    // finds value in table, removes, manages chain
    int remove_value(int type_id,int value);

    void show_list(int maxtype);

    int sublist_empty(int type_id);

    void resize();

    void set_up(int number_of_types,int number_of_entries_per);

       /// expires all tabu entries that have expired
    void clean_up(int current_iteration);



 protected:

    int MAXTYPES;
    int MAXTABLESIZE;

    IntVector First;     // holds the index of the first entry 
                         // in table for each type 

    IntVector Last;      // holds the index of the first entry 
                         // in table for each type


    // provide doubly linked chaining                
    IntVector Next; 
    IntVector Prev;

    IntVector table;     // represents the table of entries
    IntVector timestamp; // holds expiration for each local tabu table entry

    IntVector Reserved; // 0/1 for whether a table slot is reserved
                        // for the next entry of some type

    // returns the table index of the earliest free spot in table
    int next_free();

    // contains a value in a chain.
    IntVector Value_Table; 

    // time Value entry made
    IntVector TimeStamp; 

};

} // namespace utilib

#endif
