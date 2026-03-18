/*  _________________________________________________________________________
 *
 *  PICO: A C++ library of scalable branch-and-bound and related methods.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top PICO directory.
 *  _________________________________________________________________________
 */
//
// MILPSymbInfo.cpp
//
// Allow 'Infinity' to be a valid integer value for aslong or asdouble
//
// Fix bugs with non-integer arrays
//

#include <acro_config.h>
#include <utilib/BitArray.h>
#include <utilib/comments.h>
#include <utilib/_math.h>
#include <utilib/BasicArray.h>
#include <pico/MILPSymbol.h>
#include <pico/MILPSymbInfo.h>

using namespace std;
using namespace utilib;

namespace pico {

char dummy_char='a';

vector<CharString> MILPSymbolBase::symb_list;

int MILPSymbolBase::find_symbol(CharString& str)
{
int status = find(symb_list,str);
if (status != -1)
   return status;
int val;
val = (int) aslong(str,status);
if (status == OK) return val;
return -1;
}



MILPSymbInfo::MILPSymbInfo()
 : set_names(10),
   sets(10),
   set_types(10),
   num_sets(0)
{
}


MILPSymbInfo::~MILPSymbInfo()
{
add_set(true);
}



void MILPSymbInfo::add_set(const bool flag)
{
if (flag) {
   {for (size_type i=0; i<num_sets; i++) {
     if (sets[i].string_set)
        delete sets[i].string_set;
     else if (sets[i].int_set)
        delete sets[i].int_set;
     else if (sets[i].list_set)
        delete sets[i].list_set;
     else
        EXCEPTION_MNGR(runtime_error, "Trying to delete an 'empty' set.")
     }}
   sets.resize(0);
   set_types.resize(0);
   set_names.resize(0);
   num_sets=0;
   return;
   }

num_sets++;
if (num_sets >= sets.size()) {
   unsigned int tmp = num_sets + max(num_sets/10,(unsigned int)10);
   sets.resize(tmp);
   set_names.resize(tmp);
   set_types.resize(tmp);
   }
}



//
// This function assumes that the names that it extracts look like
//       A(B,C,D)   or   A[B,C,D]
//
void MILPSymbInfo::split_args(const CharString& name, 
					QueueList<CharString>& name_list)
{
char* tname = name.data();
tname--;

do {
   tname++;
   char* cndx = tname;
   while ((*cndx != ',') && (*cndx != '\000')) cndx++;

   unsigned int slen = (unsigned int) (cndx - tname);
   if (lname.size() < (slen+1))
      lname.resize(slen+1);
   int ndx=0;
   for (unsigned int i=0; i<slen; i++) {
     if (isalnum(*tname) || (*tname == '_'))
        lname[ndx++] = *tname;
     tname++;
     }
   lname[ndx] = '\000';
   name_list.add(lname);
   
   } while (*tname == ',');
}


//
// This function assumes that the names that it extracts look like
//       A( B , C , D )   or   A[ B , C , D ]
//
void MILPSymbInfo::extract_all_names(const CharString& name, 
					QueueList<CharString>& name_list)
{
const char* tname = name.data();

const char* cndx = strchr(tname,(int)'[');
if (cndx == NULL)
   cndx = strchr(tname,(int)'(');
unsigned int slen;
if (cndx)
   slen = (unsigned int) (cndx - tname);
else
   slen = name.size();
if (lname.size() < (slen+1))
   lname.resize(slen+1);
size_type i;
for (i=0; i<slen; i++) {
  if (isalnum(*tname) || (*tname == '_'))
     lname[i] = *tname;
  tname++;
  }
lname[i] = '\000';
name_list.add(lname);
if (cndx == NULL) return;


do {
   tname++;
   cndx = strchr(tname, (int)',');
   if (cndx == NULL) {
      cndx = strchr(tname,(int)')');
      if (cndx == NULL)
         cndx = strchr(tname,(int)']');
      }
   if (cndx == NULL)
      EXCEPTION_MNGR(runtime_error, "MILPSymbInfo::extract_all_names -- ERROR")

   unsigned int slen = (unsigned int) (cndx - tname);
   if (lname.size() < (slen+1))
      lname.resize(slen+1);
   int ndx=0;
   for (i=0,ndx=0; i<slen; i++) {
     if (isalnum(*tname) || (*tname == '_') || (*tname == '.') || (*tname == '+') || (*tname == '-') || (*tname == 'e'))
        lname[ndx++] = *tname;
     tname++;
     }
   lname[ndx] = '\000';
   lname.resize(ndx);
   name_list.add(lname);
   
   } while (*tname == ',');

}



void MILPSymbInfo::create_mapfile(istream& istr)
{
CharString token;
CharString args;
CharString name;
int num_vars=0;
QueueList<CharString> name_queue;
 size_type num_sets_idx = num_sets;

while (istr) {
  //
  // Read symbol name
  //
  istr >> name;
  if (!istr) return;

  if (isalpha(name[0]) == false)
     EXCEPTION_MNGR(runtime_error, "MILPSymbInfo::create_mapfile -- the symbol is invalid \"" << name << "\"")
  HashedSet<ILPSymbItem>::iterator item;
  { ILPSymbItem tmp(name); item = hashset.find(tmp); }
  bool newitem=false;
  istr >> args;
  //
  // A constant parameter
  //
  if (name == args) {
     istr >> args;
     if (item == hashset.end()) {
        ILPSymbItem foo(name,num_vars++);
        item = hashset.insert(foo).first;
        newitem=true;
        }
     item->symb_type = 0;
     item->map.range_type = MILPVarMap::compute_type(args);
     istr >> token;				// read the ';'
     }
  //
  // A set or array
  //
  else {
     if (args == ";") {
        EXCEPTION_MNGR(runtime_error,"MILPSymbInfo::create_mapfile -- an empty set or array: \"" << name << "\"")
        }
     name_queue.clear();
     split_args(args,name_queue);
     for (size_type i=0; i<name_queue.size(); i++) {
       name_queue.remove(token);
       name_queue.add(token);
       }
     int num_args = -1;
     int set_ndx = -1;
     if (name == token) {
        //
        // Array
        //
        if (item == hashset.end()) {
           ILPSymbItem foo(name,num_vars++);
           item = hashset.insert(foo).first;
           newitem=true;
           }
        item->symb_type = 1;
        num_args = name_queue.size()-1;
        if (newitem)
           item->args.resize(num_args);
        }

     else {
        //
        // Set
        //
        num_args = name_queue.size();
        if (num_args > 1) {
           add_set();
           set_ndx=num_sets-1;
           set_types[set_ndx].resize(num_args);
           set_names[set_ndx] = name;
           sets[set_ndx].list_set = new HashedSet<BasicArray<int> >();
           }
        }
     ////
     //// Loop through data, verifying its types and collecting symbols
     ////
     bool first=true;
     istr >> args;
     bool item_flag = !(item == hashset.end());
     do {
        name_queue.clear();
        split_args(args,name_queue);
        num_args = name_queue.size() - 1;		// need this line?

        for (int j=0; j<num_args; j++) {
          name_queue.remove(token);
          int type = MILPVarMap::compute_type(token);
          //
          // The index is a symbolic set
          //
          if (type != 0) {
             if (first) {
		add_set();
		num_sets_idx = num_sets;
                sets[num_sets_idx-1].string_set = new HashedSet<CharString>();
		set_types[num_sets_idx-1].resize(1);
		set_types[num_sets_idx-1][0].string_set = sets[num_sets_idx-1].string_set;
		set_types[num_sets_idx-1][0].index = num_sets_idx-1;
		if (item_flag) {
                   item->args[j].string_set = sets[num_sets_idx-1].string_set;
                   item->args[j].index   = num_sets_idx-1;
                   }
                else {
                   if (num_args > 1) {
		      set_types[set_ndx][j].string_set = sets[num_sets_idx-1].string_set;
		      set_types[set_ndx][j].index = num_sets_idx-1;
                      }
                   else
                      set_names[num_sets_idx-1] = name;
                   }
                }
             if (item_flag) {
		if (item->args[j].string_set->contains(token) == false)
                   item->args[j].string_set->insert(token);
                }
             else {
		if (set_types[num_sets_idx-1][j].string_set->contains(token) == false)
                   set_types[num_sets_idx-1][j].string_set->insert(token);
                }
             }
          //
          // The index is an integer set
          //
          else {
             if (first) {
                add_set();
		num_sets_idx = num_sets;
                sets[num_sets_idx-1].int_set = new HashedSet<int>();
		set_types[num_sets_idx-1].resize(1);
		set_types[num_sets_idx-1][0].int_set = sets[num_sets_idx-1].int_set;
		set_types[num_sets_idx-1][0].index = num_sets_idx-1;
		if (item_flag) {
                   item->args[j].int_set = sets[num_sets_idx-1].int_set;
                   item->args[j].index   = num_sets_idx-1;
                   }
                else {
		   if (num_args > 1) {
		      set_types[set_ndx][j].int_set = sets[num_sets_idx-1].int_set;
		      set_types[set_ndx][j].index = num_sets_idx-1;
		      }
                   else
                      set_names[num_sets_idx-1] = name;
                   }
                }
             }
          }
        first = false;
        istr >> args;
        } while (args != ";");
     if (item_flag) {
        //
        // Compute the type of the array value.  The default is an integer,
	// so only change if we see a noninteger, and if we see an integer than
	// don't change back (this coerces ints to doubles where appropriate).
        //
        name_queue.remove(token);
        int tmp = MILPVarMap::compute_type(token);
        if (tmp != 0)
           item->map.range_type = tmp;
        }
     }
  }
}



void MILPSymbInfo::read_valfile(CharString& fname)
{
ifstream istr(fname.data());

CharString varname;
CharString token;
CharString args;
CharString name;
int num_vars=0;
QueueList<CharString> name_queue;
int set_ndx=-1;
HashedSet<ILPSymbItem>::iterator item;

while (istr) {
  //
  // Read symbol name
  //
  istr >> varname;
  if (!istr) return;

  if (isalpha(varname[0]) == false)
     EXCEPTION_MNGR(runtime_error, "MILPSymbInfo::read_mapfile -- the symbol is invalid \"" << varname << "\n")
  { ILPSymbItem tmp(varname); item = hashset.find(tmp); }
  bool item_flag = !(item == hashset.end());
  if (!item_flag) {
     set_ndx=-1;
     for (size_type i=0; i<num_sets; i++)
       if (varname == set_names[i]) { set_ndx = i; break; }
     if (set_ndx == -1)
        EXCEPTION_MNGR(runtime_error, "MILPSymbInfo::read_valfile -- missing variable \"" << varname << "\"")
     }
  else {
     //
     // Setup the map object for this variable and add to the hashset
     //
     item->rc_flag = -1;
     item->map.set_val(num_vars);
     num_vars++;
     }
  //
  // Figure out the type of symbol
  //
  istr >> args;
  if (item_flag && item->symb_type == 0) {
     
     
     //
     item->symb_type = 0;
     name_queue.clear();
     name_queue.add(varname);
     add_valid_tuple(name_queue);
     istr >> args;
     item->map.set_val(args);
     istr >> token;				// read the ';'
     other_index_ht.insert(item->map);
     }
  else {
     //
     // A set or array
     //
     if (args == ";")
        EXCEPTION_MNGR(runtime_error, "MILPSymbInfo::read_valfile -- an empty set or array: \"" << varname << "\"")
     ////
     //// Loop through data, verifying its types and collecting symbols
     ////
     name_queue.clear();
     split_args(args,name_queue);
     int num_args = (item_flag ? name_queue.size() - 1 : name_queue.size());
     istr >> args;
     BasicArray<int> listvals(num_args);
     do {
        name_queue.clear();
        split_args(args,name_queue);
        if (item_flag) {
           name_queue.add(varname);       		// Add the symbol name
           name_queue.remove(name);			// Remove the first
           for (int ii=0; ii<num_args; ii++) {
             name_queue.add(name);
             name_queue.remove(name);			// The last is the range
             }
           add_valid_tuple(name_queue);
           name_queue.add(name);			// Add the range
           name_queue.remove(name);			// Remove the symb name
	   if (name != varname)  EXCEPTION_MNGR(runtime_error,"BUG!")
           }
        for (int j=0; j<num_args; j++) {
          name_queue.remove(name);
          int type = MILPVarMap::compute_type(name);
	  //
	  // Symbolic Set
          //
          if (type != 0) {
             HashedSet<CharString>* set;
             if (item_flag)
                set = item->args[j].string_set;
             else
		set = set_types[set_ndx][j].string_set;
             if (set->contains(name) == false)
                set->insert(name);
             if (item_flag) 
                item->map.varmap[2*j+2] = find(MILPSymbolBase::symb_list,name);
             else
                //listvals[j] = set->index(name); WEH - is this needed?
                listvals[j] = -1;
             }
	  //
	  // Integer Set
          //
          else {
             HashedSet<int>* set;
             if (item_flag)
                set = item->args[j].int_set;
             else
		set = set_types[set_ndx][j].int_set;
             int status;
             int val = aslong(name,status);
	     if (set->contains(val) == false)
                set->insert(val);
             if (item_flag)
                item->map.varmap[2*j+2] = val;
             else
                //listvals[j] = set->index(val); WEH - is this needed?
                listvals[j] = -1;
             }
          }
        if (item_flag) {
           //
           // Compute the type of the array value.  The default is an integer,
	   // so only change if we see a noninteger, and if we see an integer 
	   // then don't change back (this coerces ints to doubles where 
	   // appropriate).
           //
           name_queue.remove(name);
           item->map.set_val(name);
           other_index_ht.insert(item->map);
           }
        else {
           //
           // Compute the type of the array value.  The default is an integer,
	   // so only change if we see a noninteger, and if we see an integer 
	   // then don't change back (this coerces ints to doubles where 
	   // appropriate).
           //
           if (sets[set_ndx].list_set)
              sets[set_ndx].list_set->insert(listvals);
           }
        istr >> args;
        } while (args != ";");
     }
  }
}



void MILPSymbInfo::create_mapfile(ostream& ostr,
			BasicArray<CharString>& names,
			const int write_flag)
{
CharString name;
QueueList<CharString> name_queue;
int num_vars=0;
 size_type num_sets_idx = num_sets;

//
// Setup the index sets
//
{for (size_type i=0; i<names.size(); i++) {
  extract_all_names(names[i],name_queue);  

  if (name_queue.size() == 0)
     EXCEPTION_MNGR(runtime_error, "MILPSymbInfo::setup_map_file -- extract_all_names failed to extract even one name from \"" << names[i] << "\"!")
  name_queue.remove(name);
  if (isalpha(name[0]) == false)
     EXCEPTION_MNGR(runtime_error, "setup_ilp_symb -- the " << i << "-th row name is invalid \"" << name << "\"")
  HashedSet<ILPSymbItem>::iterator item;
  { ILPSymbItem tmp(name); item = hashset.find(tmp); }
  bool item_flag = !(item == hashset.end());
  bool flag=false;
  if (!item_flag) {
     ILPSymbItem foo(name,num_vars++);
     item = hashset.insert(foo).first;
     item->symb_type = 1;
     flag=true;
     }
  int num_args = name_queue.size();
  if (flag) {
     item->args.resize(num_args);
     }

  for (int j=0; j<num_args; j++) {
    name_queue.remove(name);
    int status;
    aslong(name,status);
    if (status != OK) {		// the index is a symbolic set
       if (flag) {
          add_set();
	  num_sets_idx = num_sets;
          sets[num_sets_idx-1].string_set = new HashedSet<CharString>();
          set_types[num_sets_idx-1].resize(1);
          set_types[num_sets_idx-1][0].string_set = sets[num_sets_idx-1].string_set;
          set_types[num_sets_idx-1][0].index      = num_sets-1;
          item->args[j].string_set = sets[num_sets_idx-1].string_set;
          item->args[j].index      = num_sets-1;
          }
       if (item->args[j].string_set->contains(name) == false)
          item->args[j].string_set->insert(name);
       }
    else {
       if (flag) {
          add_set();
	  num_sets_idx = num_sets;
          sets[num_sets_idx-1].int_set = new HashedSet<int>();
          set_types[num_sets_idx-1].resize(1);
          set_types[num_sets_idx-1][0].int_set = sets[num_sets_idx-1].int_set;
          set_types[num_sets_idx-1][0].index   = num_sets-1;
          item->args[j].int_set = sets[num_sets_idx-1].int_set;
          item->args[j].index   = num_sets-1;
          }
       }
    }
}}


//
// Reduce the number of sets
//
BasicArray<size_type> map(num_sets);
{for (size_type i=0; i<num_sets; i++) map[i] = i;}
{for (size_type i=0; i<num_sets; i++) 
   if (sets[i].string_set && (sets[i].string_set->size() == 0))
      EXCEPTION_MNGR(runtime_error, "Empty string set")
}
{for (size_type i=0; i<num_sets-1; i++) {
   if (sets[i].string_set) {
      for (size_type j=i+1; j<num_sets; j++) {
        if (sets[j].string_set && (map[i] == i) &&
            (is_disjoint( *(sets[i].string_set), *(sets[j].string_set)) == false)) {
	   if (map[j] < i) {
             map[i] = map[j];
             HashedSet<CharString>::iterator curr = sets[i].string_set->begin();
             HashedSet<CharString>::iterator end  = sets[i].string_set->end();
	     while (curr != end) {
               sets[map[j]].string_set->insert(*curr);
	       curr++;
	     }
	   } else {
             map[map[j]] = i;
             HashedSet<CharString>::iterator curr = sets[map[j]].string_set->begin();
             HashedSet<CharString>::iterator end  = sets[map[j]].string_set->end();
	     while (curr != end) {
               sets[i].string_set->insert(*curr);
	       curr++;
	     }
	   }
           }
        }
      }
  }}


//
// Write out the mapfile to the ostream 
//
if (write_flag) {
   ostr << "#" << endl;
   ostr << "# *.map file automatically generated by MILPSymbInfo::create_mapfile." << endl;
   ostr << "#" << endl;
   //
   // Write the list of global symbols
   //
   //int ctr=0;
   ostr << "GlobalSymbols =" << endl;
   {for (size_type i=0; i<num_sets; i++) {
     if (sets[i].string_set && (map[i] == i)) {
        //
        // Write out the set members
        //
        CharString str;
        HashedSet<CharString>::iterator curr = sets[i].string_set->begin();
        HashedSet<CharString>::iterator end  = sets[i].string_set->end();
        while (curr != end) {
          ostr << "\t" << *curr << endl;;
	  curr++;
          }
        //map[i] = ctr;
        //for (int j=i+i; j<string_sets.size(); j++)
          //if (map[j] == i)  map[j] = ctr;
        //ctr++;
        }
     }}
   ostr << "\t;" << endl;


   //
   // Write the elementary set symbols
   //
   {for (size_type i=0; i<num_sets; i++) {
      if (map[i] != i) continue;
      if (set_names[i] == "") {
         set_names[i] = "Set";
         set_names[i] += (unsigned int)i;
         }
      if (sets[i].string_set)
         ostr << set_names[i] << " ( STRING ) ;" << endl;
      else if (sets[i].int_set)
         ostr << set_names[i] << " ( INT ) ;" << endl;
      }}
   {for (size_type i=0; i<num_sets; i++) {
      if (map[i] != i) continue;
      if (! sets[i].list_set) continue;

      ostr << set_names[i] << " ( ";
      for (size_type j=0; j<set_types[i].size(); j++) {
        ostr << set_names[map[set_types[i][j].index]];
        if ((j+1) < set_types[i].size())
           ostr << " , ";
        }
      ostr << " ) ;" << endl;
      }}

   //
   // Write the list of array symbols
   //
   HashedSet<ILPSymbItem>::iterator item = hashset.begin();
   HashedSet<ILPSymbItem>::iterator end  = hashset.end();
   while (item != end) {
     
     ostr << item->name;
     if (item->args.size() > 0) {
        ostr << " [ ";
	unsigned int res = map[item->args[0].index];
	while (map[res] != res) res = map[res];
        ostr << set_names[res] << " ";
        for (size_type i=1; i<item->args.size(); i++) {
          ostr << " , ";
	  unsigned int res = item->args[i].index;
	  while (map[res] != res) res = map[res];
          ostr << set_names[res] << " ";
          }
        ostr << " ] ";
        }
     ostr << " ;" << endl;
     ostr.flush();
     item++;
     }
   //
   // After writing out the mapfile, reset the data structures used here.
   //
   add_set(1);
   hashset.clear();
   }
}



void MILPSymbInfo::setup_ilp_symbols(BasicArray<CharString>&    names, 
			HashedSet<MILPVarMap>&            index_ht,
			const int rc_flag)
{
CharString name;
CharString varname;
QueueList<CharString> name_queue;
HashedSet<ILPSymbItem>::iterator item;

for (size_type i=0; i<names.size(); i++) {
  //
  // Extract the variable/argument names from a string
  //
  extract_all_names(names[i],name_queue);  
  add_valid_tuple(name_queue);
  if (name_queue.size() == 0)
     EXCEPTION_MNGR(runtime_error, "MILPSymbInfo::setup_ilp_symbols -- extract_all_names failed to extract even one name from \"" << names[i] << "\"!")
  //
  // Remove the variable name
  //
  name_queue.remove(varname);
  if (isalpha(varname[0]) == false)
     EXCEPTION_MNGR(runtime_error, "setup_ilp_symb -- the " << i << "-th row/col name is invalid \"" << varname << "\"")
  { ILPSymbItem tmp(varname); item = hashset.find(tmp); }
  bool item_flag = !(item == hashset.end());
  if (!item_flag)
    {
      cout << hashset;
     EXCEPTION_MNGR(runtime_error, "MILPSymbInfo::setup_ilp_symbols -- missing variable \"" << varname << "\"")
       }
  //
  // Setup the map object for this variable and add to the hashset
  //
  item->rc_flag = rc_flag;
  item->map.set_val((int)i);
  unsigned int num_args = name_queue.size();
  //
  for (size_type j=0; j<num_args; j++) {
    name_queue.remove(name);

    int val;
    int status;
    val = (int) aslong(name,status);
    if (status==OK) {		// the index is an integer
       //ucout << "MILPSymbInfo: name: " << name << ", j: " << j << ", varname: "
			//<< varname << endl;
       if (item->args[j].int_set == NULL)
          EXCEPTION_MNGR(runtime_error, "MILPSymbInfo::setup_ilp_symbols -- Error reading arg " << j << " of variable " << varname << " : numeric argument (" << val << ") for a symbolic arg type")
       if (item->args[j].int_set->contains(val) == false)
          item->args[j].int_set->insert(val);
       item->map.varmap[2*j+2] = val;
       }
    else {
       if (item->args[j].string_set == NULL)
          EXCEPTION_MNGR(runtime_error, "MILPSymbInfo::setup_ilp_symbols -- Error reading arg " << j << " of variable " << varname << " : symbolic argument (" << name << ") for a numeric arg type")
       if (item->args[j].string_set->contains(name) == false)
          item->args[j].string_set->insert(name);
       item->map.varmap[2*j+2] = find(MILPSymbolBase::symb_list,name);
       }
    }
  index_ht.insert(item->map);
  }
}



void MILPSymbInfo::read_mapfile(CharString& mapfile)
{
ifstream ifstr(mapfile.data());
if (!ifstr)
   EXCEPTION_MNGR(runtime_error, "MILPSymbInfo::read_mapfile -- Error opening file \"" << mapfile << "\"")

int num_vars=0;
CharString token;
CharString dummy;
CharString setname;
QueueList<CharString> name_queue;
int symbol_flag=1;
bool defn_flag=true;
size_type num_sets_idx;

while (ifstr) {
  //
  // Read comment lines
  //
  ifstr >> comment_lines;
  if (!ifstr) break;
  //
  // Read variable name and add to the hashset
  //
  ifstr >> token;
  if (symbol_flag) {
     if (symbol_flag == 1) {
	if (token != "GlobalSymbols")
	   EXCEPTION_MNGR(runtime_error, "MILPSymbInfo::read_mapfile -- Error processing global symbols")
        ifstr >> token;
	if (token != "=")
	   EXCEPTION_MNGR(runtime_error, "MILPSymbInfo::read_mapfile -- Error processing global symbols")
        symbol_flag++;
        }
     else {
	if (token == ";") symbol_flag=0;
        else MILPSymbolBase::symb_list.push_back(token);
        }
     }
  else {
     ifstr >> dummy;
     if ((dummy != "[") && (dummy != "(") && (dummy != ";"))
        EXCEPTION_MNGR(runtime_error, "MILPSymbInfo::read_mapfile -- a '[' or '(' or ';' must follow a symbol declaration!")
     HashedSet<ILPSymbItem>::iterator item;
     //
     // If we have an array of parameter, add it to the hashset
     //
     if ((dummy == "[") || (dummy == ";")) {
        defn_flag = false;
        ILPSymbItem tmp_item(token,num_vars++);
        item = hashset.find(tmp_item);
        if (item != hashset.end())
           EXCEPTION_MNGR(runtime_error, "MILPSymbInfo::read_mapfile -- symbol \"" << token << "\" already exists!")
        item = hashset.insert(tmp_item).first;
        if (dummy == "[") item->symb_type=1;
        }
     else {
        item = hashset.end();
        if (defn_flag == false)
           EXCEPTION_MNGR(runtime_error, "Must move array/parameter definitions below the definition of all sets: \"" << token << "\"")
        setname = token;
        }
     //
     // Read in the argument list, which is stored in 'name_queue'
     //
     if (dummy != ";") {
        ifstr >> token;
        name_queue.add(token);
        ifstr >> dummy;				// this is a ',' so ignore
        while ((dummy != ")") && (dummy != "]")) {
          ifstr >> token;			// this is a ',' so ignore
          name_queue.add(token);
          ifstr >> dummy;
          }
        ifstr >> dummy;				// this is a ';'
        }
     //
     // Process the list of argument types for an array
     //
     unsigned int num_args = name_queue.size();
     if (item != hashset.end()) {
        item->args.resize(num_args);
        item->map.varmap.resize(2*num_args + 1);
        item->map.varmap[0] = item->index;

        for (size_type j=0; j<num_args; j++) {
          name_queue.remove(token);
          if ((token == "int") || (token == "INT") || (token == "integer") ||
	         (token == "INTEGER")) {
             add_set();
	     num_sets_idx = num_sets;
             sets[num_sets_idx-1].int_set = new HashedSet<int>();
             set_types[num_sets_idx-1].resize(1);
	     set_types[num_sets_idx-1][0].int_set = sets[num_sets_idx-1].int_set;
	     set_types[num_sets_idx-1][0].index   = num_sets-1;
             item->args[j].int_set = sets[num_sets_idx-1].int_set;
             item->args[j].index = num_sets-1;
             item->map.varmap[2*j+1] = num_sets-1;
             }
          else if ((token == "string") || (token == "STRING") || 
				(token == "char") || (token == "CHAR")) {
             EXCEPTION_MNGR(runtime_error, "Only should use a STRING declaration for a simple set definition")
             }
          else {
             size_type i;
             for (i=0; i<num_sets; i++)
               if (token == set_names[i]) break;
             if (i < num_sets) {
		if (sets[i].string_set)
                   item->args[j].string_set = sets[i].string_set;
                else if (sets[i].int_set)
                   item->args[j].int_set = sets[i].int_set;
                else {
                   if (set_types[i].size() == 1) {
                      if (set_types[i][0].string_set)
                         item->args[j].string_set = set_types[i][0].string_set;
                      else
                         item->args[j].int_set = set_types[i][0].int_set;
                      }
                   else 
                      item->args[j].list_set = sets[i].list_set;
                   }
                item->args[j].index   = i;
                item->map.varmap[2*j+1] = i;
                }
             else {
                EXCEPTION_MNGR(runtime_error, "MILPSymbInfo::read_mapfile - Missing definition for set type \"" << token << "\" (1)")
                }
             }
          }
        }
     //
     // Process a set definition
     //
     else {
        //
        // Define a simple set
        //
	int curr = num_sets;
        if (num_args > 1) {
           add_set();
           sets[curr].list_set = new HashedSet<BasicArray<int> >();
           set_names[curr] = setname;
           set_types[curr].resize(num_args);
           }
        //
        // Create a dummy set element that defines the types of the 
	// elements in a list for this set.
	//
        for (size_type j=0; j<num_args; j++) {
          name_queue.remove(token);
          //
          // An unlabeled integer list element
          //
          if ((token == "int") || (token == "INT") || (token == "integer") ||
	      (token == "INTEGER")) {
	     add_set();
	     num_sets_idx = num_sets;
             sets[num_sets_idx-1].int_set = new HashedSet<int>();
	     set_types[num_sets_idx-1].resize(1);
	     set_types[num_sets_idx-1][0].int_set = sets[num_sets_idx-1].int_set;
	     set_types[num_sets_idx-1][0].index = num_sets-1;
	     if (num_args > 1) {
	        set_types[curr][j].int_set = sets[num_sets_idx-1].int_set;
	        set_types[curr][j].index = num_sets-1;
                }
             else
	        set_names[num_sets_idx-1] = setname;
             }
          //
          // An string list element
          //
          else if ((token == "string") || (token == "STRING") || 
				  (token == "char") || (token == "CHAR")) {
	     add_set();
	     num_sets_idx = num_sets;
             sets[num_sets_idx-1].string_set = new HashedSet<CharString>();
	     set_types[num_sets_idx-1].resize(1);
	     set_types[num_sets_idx-1][0].string_set = sets[num_sets_idx-1].string_set;
	     set_types[num_sets_idx-1][0].index = num_sets-1;
	     if (num_args > 1) {
	        set_types[curr][j].int_set = sets[num_sets_idx-1].int_set;
	        set_types[curr][j].index = num_sets-1;
                }
             else
	        set_names[num_sets_idx-1] = setname;
             }
          //
          // A labeled set element is prefered
          //
          else {
             size_type i;
	     //
	     // Is this an existing set?
	     //
             for (i=0; i<num_sets; i++)
               if (token == set_names[i]) break;
             if (i < num_sets) {
		if (set_types[i].size() == 1) {
                   if (set_types[i][0].string_set)
		      set_types[curr][j].string_set = set_types[i][0].string_set;
                   else
		      set_types[curr][j].int_set = set_types[i][0].int_set;
                   }
                else
		   set_types[curr][j].list_set = sets[i].list_set;
	        set_types[curr][j].index = i;
                }
	     else
                EXCEPTION_MNGR(runtime_error, "MILPSymbInfo::read_mapfile - Missing definition for set type \"" << token << "\" (1)")
             }
           }
        }
     }
  }
}



void MILPSymbInfo::preprocess_ILP(CharString& probname, 
			BasicArray<CharString>& row_names, 
			BasicArray<CharString>& col_names,
			CharString& mapfile,
			CharString& ofile,
			const bool write_flag)
{
//
// Create a mapfile if one does not exist
//
if (mapfile == "") {
   mapfile = probname;
   mapfile += ".map";
   }
CharString valfile;
valfile = probname;
valfile += ".val";

ifstream ifstr(mapfile.data());
if (!ifstr) {
   ifstr.close();
   //
   // Process the *.val file, if available
   //
   ifstream val_ifstr(valfile.data());
   create_mapfile(val_ifstr);
   //
   // Process the row/column names
   //
   ofstream ofstr(mapfile.data());
   create_mapfile(ofstr, row_names, 0);
   create_mapfile(ofstr, col_names, 1);
   ofstr.close();
   }
else
   ifstr.close();

//
// Read in the map file
//
read_mapfile(mapfile);
//
// Read in a valfile 
//
read_valfile(valfile);

//
// Process the row/col names and setup the hashset
//
setup_ilp_symbols(row_names, row_index_ht,0);
setup_ilp_symbols(col_names, col_index_ht,1);


//
// FOR ACRO_VALIDATING
//
/***
ucout << "HASHTABLE" << endl;
ucout << hashset << endl;
ucout << "COL_INDEX_HT" << endl;
ucout << col_index_ht << endl;
ucout << "ROW_INDEX_HT" << endl;
ucout << row_index_ht << endl;
ucout << "NAMESET_LABELS" << endl;
ucout << set_names << endl;
ucout << "NAMESETS" << endl;
{for (size_type i=0; i<num_sets; i++) {
  ucout << sets[i] << endl;
  }}
ucout << "GLOBAL SYMBOLS" << endl;
ucout << MILPSymbolBase::symb_list << endl;
ucout << Flush;
***/
//
// END ACRO_VALIDATING
//

if (write_flag == false) return;
//
// Write out the *.h file
//
CharString fname = ofile;
fname += "_info.h";
ofstream ofstr(fname.data());

////
////
//// Class definition for the SymbInfo file
////
////
ofstr << "//" << endl;
ofstr << "// " << ofile << "_info.h" << endl;
ofstr << "//" << endl;
ofstr << "// Automatically generated by MILPSymbInfo::setup_ilp_symbols" << endl;
ofstr << "//" << endl;
ofstr << "" << endl;
ofstr << "#ifndef " << ofile << "_info_h" << endl;
ofstr << "#define " << ofile << "_info_h" << endl;
ofstr << "" << endl;
ofstr << "#include <utilib/PackBuf.h>" << endl;
ofstr << "#include <set>" << endl;
ofstr << "#include <pico/MILPSymbInfo.h>" << endl;
ofstr << "#include <pico/MILPSymbol.h>" << endl;
ofstr << "" << endl;
ofstr << "namespace " << ofile << " {" << endl;
ofstr << "" << endl;
ofstr << "using std::set;" << endl;
ofstr << "using utilib::CharString;" << endl;
ofstr << "using utilib::Tuple1;" << endl;
ofstr << "using utilib::Tuple2;" << endl;
ofstr << "using utilib::Tuple3;" << endl;
ofstr << "using utilib::Tuple4;" << endl;
ofstr << "using utilib::Tuple5;" << endl;
ofstr << "using utilib::Tuple6;" << endl;
ofstr << "using utilib::Tuple7;" << endl;
ofstr << "using utilib::QueueList;" << endl;
ofstr << "using pico::MILPSymbol0;" << endl;
ofstr << "using pico::MILPSymbol1;" << endl;
ofstr << "using pico::MILPSymbol2;" << endl;
ofstr << "using pico::MILPSymbol3;" << endl;
ofstr << "using pico::MILPSymbol4;" << endl;
ofstr << "using pico::MILPSymbol5;" << endl;
ofstr << "using pico::MILPSymbol6;" << endl;
ofstr << "using pico::MILPSymbol7;" << endl;
ofstr << "" << endl;
ofstr << "class " << "Info;" << endl;
ofstr << "" << endl;

ofstr << "" << endl;
ofstr << "class " << "SymbInfo" << endl;
ofstr << "{" << endl;
ofstr << "public:" << endl;
ofstr << "	///" << endl;
ofstr << "  " << "SymbInfo(" << "Info* _info=0) : infoptr(_info) {}" << endl;
ofstr << "	///" << endl;
ofstr << "  " << "Info* infoptr;" << endl;
ofstr << "	///" << endl;
ofstr << "  " << "Info* info() {return infoptr;}" << endl;
ofstr << "};" << endl;
ofstr << "" << endl;
ofstr << "" << endl;

ofstr << "" << endl;
ofstr << "class " << "MILPSymbFunctions : public " << 
			"SymbInfo" <<  endl;
ofstr << "{" << endl;
ofstr << "public:" << endl;
ofstr << "	///" << endl;
ofstr << "  " << "MILPSymbFunctions(" << "Info* _info=NULL) : "
		<< "SymbInfo(_info) {}" << endl;
ofstr << "	///" << endl;
ofstr << "  void test_varmap();" << endl;
ofstr << "	///" << endl;
////
//// Enum definitions for the SymbInfo file
////
{
int ctr=0;
for (size_type ii=0; ii<num_sets; ii++) {
  if (! sets[ii].string_set) continue;
  ofstr << "  enum " << set_names[ii] << "Enum { ";
  CharString name;
  utilib::HashedSet<CharString>::iterator curr = sets[ii].string_set->begin();
  utilib::HashedSet<CharString>::iterator end  = sets[ii].string_set->end();
  bool first=true;
  while (curr != end) {
    if (first)
       ofstr << *curr << "=" << (ctr++);
    else
       ofstr << ", " << *curr << "=" << (ctr++);
    first = false;
    curr++;
    }
  ofstr << " };" << endl;
  }
}
////
//// Set definitions for the SymbInfo file
////
{for (size_type ii=0; ii<num_sets; ii++) {
  if (sets[ii].string_set)
     ofstr << "  set<" << set_names[ii] << "Enum>& " << set_names[ii] << "();" << endl;
  else if (sets[ii].int_set)
     ofstr << "  set<int>& " << set_names[ii] << "();" << endl;
  else
     ofstr << "  set<ILPArgList>& " << set_names[ii] << "();" << endl;
  }
}
////
//// Array definitions for the SymbInfo file
////
HashedSet<ILPSymbItem>::iterator curr_row = hashset.begin();
HashedSet<ILPSymbItem>::iterator end      = hashset.end();
while (curr_row != end) {
  ofstr << "	///" << endl;
  ofstr << "  CharString& " << curr_row->name  << "_name();" << endl;

  //
  // isvalid Tuple function
  //
  if (curr_row->args.size() > 0) {
     ofstr << "  bool " << curr_row->name << "_isvalid(";

     ofstr << "const Tuple" << curr_row->args.size() << "<";
     if (curr_row->args[0].string_set)
        ofstr << set_names[curr_row->args[0].index] << "Enum";
     else
        ofstr << "int";
     if (curr_row->args.size() > 1) {
        for (size_type i=1; i<curr_row->args.size(); i++)
          if (curr_row->args[i].string_set)
             ofstr << "," << set_names[curr_row->args[i].index] << "Enum";
          else
             ofstr << ",int";
        }
     ofstr << ">& tuple) const;" << endl;
     }
  //
  // isvalid Array function
  //
  if (curr_row->args.size() > 0) {
     ofstr << "  bool " << curr_row->name << "_isvalid(";
     if (curr_row->args[0].string_set)
        ofstr << "const " << set_names[curr_row->args[0].index] << "Enum& arg0";
     else
        ofstr << "const int& arg0";
     if (curr_row->args.size() > 1) {
        for (size_type i=1; i<curr_row->args.size(); i++) {
          if (curr_row->args[i].string_set)
             ofstr << ",const " << set_names[curr_row->args[i].index] << "Enum& arg";
          else
             ofstr << ",const int& arg";
          ofstr << i;
          }
        }
     ofstr << ") const;" << endl;
     }

  //
  // Array functions with > 0 arguments
  //
  if (curr_row->args.size() > 0) {

  if ((curr_row->rc_flag == 1) || (curr_row->rc_flag == 0))
     ofstr << "  int " << curr_row->name << "(";
  else {
     if (curr_row->map.range_type == 0)
        ofstr << "  int ";
     else if (curr_row->map.range_type == 1)
        ofstr << "  double ";
     else {
        EXCEPTION_MNGR(runtime_error, "Not yet supported arrays returning symbols");
	// Q: where do we keep the index for the symbol index set?
        }
     ofstr << curr_row->name << "(";
     }

  if (curr_row->args.size() > 0) {
     ofstr << "const Tuple" << curr_row->args.size() << "<";
     if (curr_row->args[0].string_set)
        ofstr << set_names[curr_row->args[0].index] << "Enum";
     else
        ofstr << "int";
     if (curr_row->args.size() > 1) {
        for (size_type i=1; i<curr_row->args.size(); i++)
          if (curr_row->args[i].string_set)
             ofstr << "," << set_names[curr_row->args[i].index] << "Enum";
          else
             ofstr << ",int";
        }
     }
  ofstr << ">& tuple) const;" << endl;
  }
  //
  // Array functions with 0 arguments
  //
  ofstr << "  int " << curr_row->name << "(";
  if (curr_row->args.size() > 0) {
     if (curr_row->args[0].string_set)
        ofstr << "const " << set_names[curr_row->args[0].index] << "Enum& arg0";
     else
        ofstr << "const int& arg0";
     if (curr_row->args.size() > 1) {
        for (size_type i=1; i<curr_row->args.size(); i++) {
          if (curr_row->args[i].string_set)
             ofstr << ",const " << set_names[curr_row->args[i].index] << "Enum& arg";
          else
             ofstr << ",const int& arg";
          ofstr << i;
          }
        }
     }
  ofstr << ") const;" << endl;

  //
  // Array functions with > 0 arguments
  //
  if (curr_row->args.size() > 0) {
     ofstr << "  set<Tuple" << curr_row->args.size() << "<";
     if (curr_row->args[0].string_set)
        ofstr << set_names[curr_row->args[0].index] << "Enum";
     else
        ofstr << "int";
     if (curr_row->args.size() > 1) {
        for (size_type i=1; i<curr_row->args.size(); i++)
          if (curr_row->args[i].string_set)
             ofstr << "," << set_names[curr_row->args[i].index] << "Enum";
          else
             ofstr << ",int";
        }
     ofstr << "> >& " << curr_row->name << "_valid();" << endl;
     //
     // Typedefs
     //
     ofstr << "  typedef set<Tuple" << curr_row->args.size();
     ofstr << "<";
     if (curr_row->args[0].string_set)
        ofstr << set_names[curr_row->args[0].index] << "Enum";
     else
        ofstr << "int";
     if (curr_row->args.size() > 1) {
        for (size_type i=1; i<curr_row->args.size(); i++)
          if (curr_row->args[i].string_set)
             ofstr << "," << set_names[curr_row->args[i].index] << "Enum";
          else
             ofstr << ",int";
        }
     ofstr << "> >::const_iterator ";
     ofstr  << curr_row->name << "_const_iterator;" << endl;
   
     ofstr << "  typedef set<Tuple" << curr_row->args.size();
     ofstr << "<";
     if (curr_row->args[0].string_set)
        ofstr << set_names[curr_row->args[0].index] << "Enum";
     else
        ofstr << "int";
     if (curr_row->args.size() > 1) {
        for (size_type i=1; i<curr_row->args.size(); i++)
          if (curr_row->args[i].string_set)
             ofstr << "," << set_names[curr_row->args[i].index] << "Enum";
          else
             ofstr << ",int";
        }
     ofstr << "> >::iterator ";
     ofstr  << curr_row->name << "_iterator;" << endl;
     }
  curr_row++;
  }
ofstr << "};" << endl;
ofstr << "" << endl;
ofstr << "" << endl;
ofstr << "" << endl;
////
////
//// Class definition for the Info file
////
////
ofstr << "class " << "Info : public pico::MILPSymbInfo, public " << 
			"MILPSymbFunctions" << endl;
ofstr << "{" << endl;
ofstr << "" << endl;
ofstr << "public:" << endl;
ofstr << "  /**@name Variable/Row Mapping Data */" << endl;
ofstr << "  //@{" << endl;
////
//// Set definitions for the Info file
////
{for (size_type ii=0; ii<num_sets; ii++)
  if (sets[ii].string_set)
     ofstr << "  set<" << set_names[ii] << "Enum> " << set_names[ii] << ";" << endl;
  else if (sets[ii].int_set)
     ofstr << "  set<int> " << set_names[ii] << ";" << endl;
  else
     ofstr << "  set<ILPArgList> " << set_names[ii] << ";" << endl;
}
ofstr << "  //@}" << endl;
ofstr << "" << endl;
////
//// Constructors for the Info file
////
ofstr << "	///" << endl;
ofstr << "  " << "Info();" << endl;
ofstr << "	///" << endl;
ofstr << "  ~" << "Info() {}" << endl;
ofstr << "	///" << endl;
ofstr << "  void add_valid_tuples();" << endl;
ofstr << "  void add_valid_tuple(QueueList<CharString> );" << endl;
ofstr << "" << endl;
////
//// Array definitions for the Info file
////
ofstr << "  /**@name Arrays and Parameters, Including Variable/Row Names */" << endl;
ofstr << "  //@{" << endl;
curr_row = hashset.begin();
while (curr_row != end) {
  ofstr << "  MILPSymbol" << curr_row->args.size();
  if (curr_row->args.size() > 0) {
     ofstr << "<";
     if (curr_row->args[0].string_set)
        ofstr << set_names[curr_row->args[0].index] << "Enum";
     else
        ofstr << "int";
     if (curr_row->args.size() > 1) {
        for (size_type i=1; i<curr_row->args.size(); i++)
          if (curr_row->args[i].string_set)
             ofstr << "," << set_names[curr_row->args[i].index] << "Enum";
          else
             ofstr << ",int";
        }
     ofstr << ",";
     }
  else
     ofstr << "<";
  if (curr_row->map.range_type == 0)
     ofstr << "int";
  else if (curr_row->map.range_type == 1)
     ofstr << "double";
  else // String
     ofstr << "BUG";
  ofstr << "> ";
  ofstr  << curr_row->name << "_symbol;" << endl;
  curr_row++;
  }
ofstr << "  //@}" << endl;
ofstr << "};" << endl;
ofstr << "" << endl;
ofstr << "" << endl;

////
//// Inline functions
////
{
for (size_type ii=0; ii<num_sets; ii++)
  if (sets[ii].string_set)
     ofstr << "  inline set<" << "MILPSymbFunctions::" << set_names[ii] << "Enum>& " << "MILPSymbFunctions::" << set_names[ii] << "() {return infoptr->" << set_names[ii] << ";}" << endl;
  else if (sets[ii].int_set)
     ofstr << "  inline set<int>& " << "MILPSymbFunctions::" << set_names[ii] << "() {return infoptr->" << set_names[ii] << ";}" << endl;
  else
     ofstr << "  inline set<ILPArgList>& " << "MILPSymbFunctions::" << set_names[ii] << "() {return infoptr->" << set_names[ii] << ";}" << endl;
}

////
//// Inline functions
////
curr_row = hashset.begin();
while (curr_row != end) {
  ofstr << "" << endl;
  ofstr << "	///" << endl;
  ofstr << "  inline CharString& " << "MILPSymbFunctions::" <<
		curr_row->name  <<
		"_name() {return infoptr->"  <<
		curr_row->name << "_symbol.name;}" << endl;
  //
  //
  //
  if (curr_row->args.size() > 0) {
     ofstr << "  inline bool MILPSymbFunctions::" << curr_row->name << 
		"_isvalid(";
     ofstr << "const Tuple" << curr_row->args.size() << "<";
     if (curr_row->args[0].string_set)
        ofstr << "MILPSymbFunctions::" << set_names[curr_row->args[0].index] << "Enum";
     else
        ofstr << "int";
     if (curr_row->args.size() > 1) {
        for (size_type i=1; i<curr_row->args.size(); i++)
          if (curr_row->args[i].string_set)
             ofstr << "," << "MILPSymbFunctions::" << set_names[curr_row->args[i].index] << "Enum";
          else
             ofstr << ",int";
        }
  ofstr << ">& tuple) const { try { infoptr->" << 
		curr_row->name << "_symbol(tuple);} catch (std::runtime_error& err) {return false;} return true; }" << endl;

  //
  //
  //
  ofstr << "  inline bool " << "MILPSymbFunctions::" << curr_row->name << "_isvalid(";
  if (curr_row->args.size() > 0) {
     if (curr_row->args[0].string_set)
        ofstr << "const MILPSymbFunctions::" << set_names[curr_row->args[0].index] << "Enum& arg0";
     else
        ofstr << "const int& arg0";
     if (curr_row->args.size() > 1) {
        for (size_type i=1; i<curr_row->args.size(); i++) {
          if (curr_row->args[i].string_set)
             ofstr << ",const " << "MILPSymbFunctions::" << set_names[curr_row->args[i].index] << "Enum& arg";
          else
             ofstr << ",const int& arg";
          ofstr << i;
          }
        }
     }
  ofstr << ") const { try { infoptr->" << curr_row->name << "_symbol(";
  if (curr_row->args.size() > 0) {
     ofstr << "arg0";
     {for (size_type i=1; i<curr_row->args.size(); i++)
        ofstr << ",arg" << i;}
     }
  ofstr << ");} catch (std::runtime_error& err) {return false;} return true;}" << endl;
  //
  //
  //
  if ((curr_row->rc_flag == 1) || (curr_row->rc_flag == 0))
     ofstr << "  inline int ";
  else {
     if (curr_row->map.range_type == 0)
        ofstr << "  inline int ";
     else if (curr_row->map.range_type == 1)
        ofstr << "  inline double ";
     else {
        EXCEPTION_MNGR(runtime_error, "Not yet supported arrays returning symbols");
	// Q: where do we keep the index for the symbol index set?
        }
     }

     ofstr << "MILPSymbFunctions::" << curr_row->name << "(";
     ofstr << "const Tuple" << curr_row->args.size() << "<";
     if (curr_row->args[0].string_set)
        ofstr << "MILPSymbFunctions::" << set_names[curr_row->args[0].index] << "Enum";
     else
        ofstr << "int";
     if (curr_row->args.size() > 1) {
        for (size_type i=1; i<curr_row->args.size(); i++)
          if (curr_row->args[i].string_set)
             ofstr << "," << "MILPSymbFunctions::" << set_names[curr_row->args[i].index] << "Enum";
          else
             ofstr << ",int";
        }
  ofstr << ">& tuple) const {return infoptr->" << 
		curr_row->name << "_symbol(tuple);}" << endl;
  }
  //
  //
  //
  if ((curr_row->rc_flag == 1) || (curr_row->rc_flag == 0))
     ofstr << "  inline int ";
  else {
     if (curr_row->map.range_type == 0)
        ofstr << "  inline int ";
     else if (curr_row->map.range_type == 1)
        ofstr << "  inline double ";
     else {
        EXCEPTION_MNGR(runtime_error, "Not yet supported arrays returning symbols");
	// Q: where do we keep the index for the symbol index set?
        }
     }
  ofstr << "MILPSymbFunctions::" << curr_row->name << "(";
  if (curr_row->args.size() > 0) {
     if (curr_row->args[0].string_set)
        ofstr << "const MILPSymbFunctions::" << set_names[curr_row->args[0].index] << "Enum& arg0";
     else
        ofstr << "const int& arg0";
     if (curr_row->args.size() > 1) {
        for (size_type i=1; i<curr_row->args.size(); i++) {
          if (curr_row->args[i].string_set)
             ofstr << ",const " << "MILPSymbFunctions::" << set_names[curr_row->args[i].index] << "Enum& arg";
          else
             ofstr << ",const int& arg";
          ofstr << i;
          }
        }
     }
  ofstr << ") const {return infoptr->" << curr_row->name << "_symbol(";
  if (curr_row->args.size() > 0) {
     ofstr << "arg0";
     {for (size_type i=1; i<curr_row->args.size(); i++)
        ofstr << ",arg" << i;}
     }
  ofstr << ");}" << endl;
  //
  //
  //
  if (curr_row->args.size() > 0) {
     ofstr << "  inline set<Tuple" << curr_row->args.size() << "<";
     if (curr_row->args[0].string_set)
        ofstr << "MILPSymbFunctions::" << set_names[curr_row->args[0].index] << "Enum";
     else
        ofstr << "int";
     if (curr_row->args.size() > 1) {
        for (size_type i=1; i<curr_row->args.size(); i++)
          if (curr_row->args[i].string_set)
             ofstr << "," << "MILPSymbFunctions::" << set_names[curr_row->args[i].index] << "Enum";
          else
             ofstr << ",int";
        }
     ofstr << "> >& " << "MILPSymbFunctions::"
		<< curr_row->name << "_valid() {return infoptr->"
		<< curr_row->name << "_symbol.valid;}" << endl;
     }
  curr_row++;
  }
ofstr << "" << endl;
ofstr << "} // namespace " << ofile << endl;
ofstr << "" << endl;
////
//// Inline stream operators for Enum types
////
{for (size_type ii=0; ii<num_sets; ii++) {
  if (! sets[ii].string_set) continue;

  ofstr << "inline ostream& operator<<(ostream& os, const " << ofile << "::MILPSymbFunctions::" << set_names[ii] << "Enum& obj)" << endl;
  ofstr << "{ os << (int) obj; return(os); }" << endl;
  ofstr << "" << endl;

  ofstr << "inline istream& operator>>(istream& is, " << ofile << "::MILPSymbFunctions::" << set_names[ii] << "Enum& obj)" << endl;
  ofstr << "{ int tmp; is >> tmp; obj = (" << ofile << "::MILPSymbFunctions::" << set_names[ii] << 
		"Enum) tmp; return(is); }" << endl;
  ofstr << "" << endl;
  ofstr << "inline utilib::PackBuffer& operator<<(utilib::PackBuffer& os, const " << ofile << "::MILPSymbFunctions::" << set_names[ii] << "Enum& obj)" << endl;
  ofstr << "{ os << (int) obj; return(os); }" << endl;
  ofstr << "" << endl;

  ofstr << "inline utilib::UnPackBuffer& operator>>(utilib::UnPackBuffer& is, " << ofile << "::MILPSymbFunctions::" << set_names[ii] << "Enum& obj)" << endl;
  ofstr << "{ int tmp; is >> tmp; obj = (" << ofile << "::MILPSymbFunctions::" << set_names[ii] << 
		"Enum) tmp; return(is); }" << endl;
  ofstr << "" << endl;
  }}
ofstr << "" << endl;
ofstr << "" << endl;

ofstr << "" << endl;
ofstr << "#endif" << endl;
ofstr.close();

//
// Write out the *.cpp file
//
fname = ofile;
fname += "_info.cpp";
ofstr.open(fname.data());
ofstr << "//" << endl;
ofstr << "// " << ofile << "_info.cpp" << endl;
ofstr << "//" << endl;
ofstr << "// Automatically generated by MILPSymbInfo::setup_ilp_symbols" << endl;
ofstr << "//" << endl;
ofstr << "" << endl;
ofstr << "#include <utilib/CommonIO.h>" << endl;
ofstr << "#include \"" << ofile << "_info.h\"" << endl;
ofstr << "" << endl;
ofstr << "namespace " << ofile << " {" << endl;
ofstr << "" << endl;
ofstr << "using namespace pico;" << endl;
ofstr << "using namespace utilib;" << endl;
ofstr << "" << endl;

ofstr << "Info::" << "Info()" << endl;
ofstr << "  : " << "MILPSymbFunctions(this)," << endl;

{
bool flag=false;
curr_row = hashset.begin();
while (curr_row != end) {
  if (flag)
     ofstr << ",";
  ofstr << endl;
  ofstr  << "   " << curr_row->name << "_symbol(\"" <<
		curr_row->name << "\",&hashset,";
  if (curr_row->rc_flag == 1)
     ofstr << "&col_index_ht";
  else if (curr_row->rc_flag == 0)
     ofstr << "&row_index_ht";
  else
     ofstr << "&other_index_ht";
  flag = true;

  if (curr_row->args.size() > 0) {
     for (size_type i=0; i<curr_row->args.size(); i++) {
       ofstr << ",&"; 
       //ucout << curr_row->name << " : " << 
		//curr_row->args[i].string_set << " "  <<
		//curr_row->args[i].int_set << " "  <<
		//curr_row->args[i].list_set << " "  <<
		//curr_row->args[i].index << endl << Flush;
       if (curr_row->args[i].string_set)
          ofstr << set_names[curr_row->args[i].index];
       else if (curr_row->args[i].int_set) {
          ofstr << set_names[curr_row->args[i].index];
          }
       else if (curr_row->args[i].list_set)
          ofstr << set_names[curr_row->args[i].index];
       else 
          EXCEPTION_MNGR(runtime_error,"Bad index type for curr_row->args[i].index");
       }
     }

  ofstr << ")";
  curr_row++;
  }
ofstr << endl;
}

ofstr << "{" << endl;
ofstr << "}" << endl;
ofstr << "" << endl;
ofstr << "" << endl;

// ************************************* new 1/05
ofstr << "void " << "Info::add_valid_tuples()" << endl;
ofstr << "{"  << endl;
ofstr << "     QueueList<CharString> queue;" << endl;
curr_row = hashset.begin();
while (curr_row != end)  {
	ofstr << "     queue.add(\"" << curr_row->name << "\");" << endl;
	curr_row++;
}
ofstr << "     while(!queue.empty())" << endl;
ofstr << "         add_valid_tuple(queue);" << endl;
ofstr << "}" << endl << endl;
// ************************************* new 1/05

ofstr << "void " << "Info::add_valid_tuple(QueueList<CharString> queue)" << endl;
ofstr << "{" << endl;
ofstr << "CharString name;" << endl;
ofstr << "queue.remove(name);" << endl;

curr_row = hashset.begin();
while (curr_row != end) {

  if (curr_row->args.size() > 0) {
     ofstr << "if (name == \"" << curr_row->name << "\")" << endl;
     ofstr << "   " << curr_row->name << "_symbol.add_valid_tuple(queue);" << endl;
     }
  curr_row++;
  }

ofstr << "}" << endl;
ofstr << "" << endl;
ofstr << "" << endl;

ofstr << "void " << "MILPSymbFunctions::test_varmap()" << endl;
ofstr << "{" << endl;

curr_row = hashset.begin();
while (curr_row != end) {

  ofstr << "{" << endl;
  if (curr_row->args.size() > 0) {
     ofstr << "ucout << \"Symbol " << curr_row->name << "\" << endl;" << endl;
     ofstr << curr_row->name << "_const_iterator curr = "
			<< curr_row->name << "_valid().begin();" << endl;
     ofstr << curr_row->name << "_const_iterator last = "
			<< curr_row->name << "_valid().end();" << endl;
     ofstr << "while (curr != last) {" << endl;

     ofstr << "  ucout ";
     for (size_type i=0; i<curr_row->args.size(); i++)
       ofstr << "<< curr->val" << (i+1) << " << \" \"";
     ofstr << " << endl;" << endl;
     ofstr << "  ucout << " << curr_row->name << "_name() << \" \" << "
     		 << curr_row->name << "(*curr) << endl;" << endl;
     ofstr << "  curr++;" << endl;
     ofstr << "  }" << endl;
     }
  else {
     ofstr << "ucout << \"Symbol " << curr_row->name << "\" << endl;" << endl;
     ofstr << "ucout << " << curr_row->name << "() << endl;" << endl;
     }
  ofstr << "}" << endl;
  curr_row++;
  }


ofstr << "ucout << Flush;" << endl;
ofstr << "}" << endl;
ofstr << "" << endl;
ofstr << "} // namespace " << ofile << endl;
ofstr << "" << endl;

ofstr.close();
}


void MILPSymbInfo::write(PackBuffer& pack) const
{
pack << lname;
pack << other_index_ht << col_index_ht << row_index_ht << hashset;
pack << set_names << sets << set_types << num_sets;
}

void MILPSymbInfo::read(UnPackBuffer& unpack)
{
unpack >> lname;
unpack >> other_index_ht >> col_index_ht >> row_index_ht >> hashset;
unpack >> set_names >> sets >> set_types >> num_sets;
}

} // namespace pico
