/*  _________________________________________________________________________
 *
 *  PICO: A C++ library of scalable branch-and-bound and related methods.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top PICO directory.
 *  _________________________________________________________________________
 */
//
// readmps.cpp
//
// Read in a free column MPS file into datatypes that can be used to initialize
// a generic LP object.
//
/*
A:  MPS format was named after an early IBM LP product and has emerged
as a de facto standard ASCII medium among most of the commercial LP
codes.  Essentially all commercial LP codes accept this format, but if
you are using public domain software and have MPS files, you may need
to write your own reader routine for this.  It's not too hard.  The
main things to know about MPS format are that it is column oriented (as
opposed to entering the model as equations), and everything (variables,
rows, etc.) gets a name.  MPS format is described in more detail in
Murtagh's book, referenced in another section. Also,

ftp://softlib.cs.rice.edu/pub/miplib/mps_format

is a nice short introduction.

MPS is an old format, so it is set up as though you were using punch
cards, and is not free format. Fields start in column 1, 5, 15, 25, 40
and 50.  Sections of an MPS file are marked by so-called header cards,
which are distinguished by their starting in column 1.  Although it is
typical to use upper-case throughout the file (like I said, MPS has
long historical roots), many MPS-readers will accept mixed-case for
anything except the header cards, and some allow mixed-case anywhere.
The names that you choose for the individual entities (constraints or
variables) are not important to the solver; you should pick names that
are meaningful to you, or will be easy for a post-processing code to
read, but do not use keywords when using this free column MPS reader.

Here is a little sample model written in MPS format (explained in more
detail below):

NAME          TESTPROB
ROWS
 N  COST
 L  LIM1
 G  LIM2
 E  MYEQN
COLUMNS
    XONE      COST                 1   LIM1                 1
    XONE      LIM2                 1
    YTWO      COST                 4   LIM1                 1
    YTWO      MYEQN               -1
    ZTHREE    COST                 9   LIM2                 1
    ZTHREE    MYEQN                1
RHS
    RHS1      LIM1                 5   LIM2                10
    RHS1      MYEQN                7
BOUNDS
 UP BND1      XONE                 4
 LO BND1      YTWO                -1
 UP BND1      YTWO                 1
ENDATA

means:

Optimize
 COST:    XONE + 4 YTWO + 9 ZTHREE
Subject To
 LIM1:    XONE + YTWO <= 5
 LIM2:    XONE + ZTHREE >= 10
 MYEQN:   - YTWO + ZTHREE  = 7
Bounds
 0 <= XONE <= 4
-1 <= YTWO <= 1
End

*/

#include <acro_config.h>
#include <utilib/std_headers.h>
#include <pico/ilp_read.h>

using namespace std;

namespace pico {

/* defines */
#define NAME     -1
#define OBJSENSE -2
#define ROWS     0
#define COLUMNS  1
#define RHS      2
#define BOUNDS   3
#define RANGES   4



//
// Data taken from one line of an MPS file.
//

class MPSRecord
{
public:

	/// Constructor
  MPSRecord() : field1(8), field2(8), field3(8), field4(0.0), field5(8),
			field6(0.0), items(0), lineno(-1) {}

	///
  int operator==(const MPSRecord& tmp) const
	{return FALSE;}
	///
  CharString field1;
	///
  CharString field2;
	///
  CharString field3;
	///
  double     field4;
	///
  CharString field5;
	///
  double     field6;

	///
  int items;
	///
  int lineno;

  void reset()
  	{
	field4=0.0;
	field6=0.0;
	items=0;
	lineno=-1;
	field1 << '\000';
	field2 << '\000';
	field3 << '\000';
	field5 << '\000';
#if 0
	char* f1=field1.data();
	char* f2=field2.data();
	char* f3=field3.data();
	char* f5=field5.data();
        for (int i=0; i<8; i++)
          f1[i] = f2[i] = f3[i] = f5[i] = '\000';
#endif
        }

};



//
// scan an MPS line, and pick up the information in the fields that are
// present
//
static void scan_line(char* line, MPSRecord& record)
{
  int line_len;
  char buf1[BUFSIZ], buf2[BUFSIZ], buf3[BUFSIZ], buf5[BUFSIZ];

  record.items = 0;
  line_len = sscanf(line, "%s%s%s", buf1, buf2, buf3);

  if(line_len >= 1) {
    record.field1[0] = '\0';
    if(line_len < 3 && (strcmp(buf1, "N") == 0 || strcmp(buf1, "G") == 0 ||
			strcmp(buf1, "L") == 0 || strcmp(buf1, "E") == 0) ||
       strcmp(buf1, "LO") == 0 || strcmp(buf1, "UP") == 0 ||
       strcmp(buf1, "FX") == 0 || strcmp(buf1, "FR") == 0 ||
       strcmp(buf1, "MI") == 0 || strcmp(buf1, "PL") ==0 ||
       strcmp(buf1, "BV") == 0 ||
       strcmp(buf1, "UI") == 0) {
      istringstream tmp(buf1);
      tmp >> record.field1;
      while (line[0] == ' ' || line[0] == '\t')
	++line;
      line += 2;
    }
    ++record.items;
  }

  line_len = sscanf(line, "%s%s", buf2, buf3);

  if(line_len >= 1) { /* name */
    istringstream tmp(buf2);
    tmp >> record.field2;
  }
  else
    record.field2[0] = '\0';

  if(line_len >= 2) { /* name */
    istringstream tmp(buf3);
    tmp >> record.field3;
    if (strcmp(buf3, "'MARKER'") == 0) {
      record.field4 = 0;
      line_len = sscanf(line, "%s%s%s",	buf2, buf3, buf5) + 1;
    }
    else
      line_len = sscanf(line, "%s%s%lf%s%lf",
			buf2,
			buf3, &record.field4,
			buf5, &record.field6);
  }
  else
    record.field3[0] = '\0';

  record.items += line_len;

  if(line_len < 3) /* number */
    record.field4 = 0;

  if(line_len >= 4) { /* name */
    istringstream tmp(buf5);
    tmp >> record.field5;
  }
  else
    record.field5[0] = '\0';

  if(line_len < 5) /* number */
    record.field6 = 0;
}



void read_mps(const char* filename, short verbose,
			CMSparseMatrix<Ereal<double> >& A,
			CharString& probname,
                        int& objsen,
                        BasicArray<Ereal<double> >& objCoefs,
                        BasicArray<Ereal<double> >& lowerBounds,
                        BasicArray<Ereal<double> >& upperBounds,
                        BasicArray<CharString>&  variableNames,
                        CharString&  constraintTypes,
                        BasicArray<Ereal<double> >& constraintLHS,
                        BasicArray<Ereal<double> >& constraintRHS,
                        BasicArray<CharString>& constraintNames,
			BasicArray<int>& listOfIntegers)
{
  ifstream is(filename);
  if (!is)
     EXCEPTION_MNGR(runtime_error, "read_mps -- bad filename \"" << filename << "\"");

  ILPread_reset();
  ILPread_set_sense(FALSE);
  MPSRecord record;

  char line[BUFSIZ];
  char tmp1[BUFSIZ], tmp2[BUFSIZ];
  int section=-1, tmp;
  int OF_found = FALSE;
  short Int_section = FALSE;
  short Debug = verbose;
  int Lineno = 0;
  short  Unconstrained_rows_found = FALSE;

  /* let's initialize line to all zero's */
  memset(line, '\0', BUFSIZ);

  while(is.getline(line, BUFSIZ)) {
    Lineno++;
    tmp = sscanf(line, "%s%s", tmp1, tmp2);

    /* skip lines which start with "*", they are comment */
    if(tmp1[0] == '*') {
      if(Debug)
	fprintf(stderr, "Comment on line %d: %s\n", Lineno, line);
      continue;
    }

    if(Debug)
      fprintf(stderr, "Line %d: %s\n", Lineno, line);

    /* first check for "special" lines: NAME, ROWS, BOUNDS .... */
    if(strcmp(tmp1, "NAME") == 0) {
      section = NAME;
      istringstream tmp(line);
      tmp >> probname;
      if (probname != "NAME")
	 EXCEPTION_MNGR(runtime_error, "readmps -- Unrecognized line " << Lineno << ": " << line);
      tmp >> probname;
    }
    else if(strcmp(tmp1, "OBJSENSE") == 0 && tmp == 1) {
      section = OBJSENSE;
      if(Debug)
	fprintf(stderr, "Switching to OBJSENSE section\n");
    }
    else if(strcmp(tmp1, "ROWS") == 0 && tmp == 1) {
      section = ROWS;
      if(Debug)
	fprintf(stderr, "Switching to ROWS section\n");
    }
    else if(strcmp(tmp1, "COLUMNS") == 0 && tmp == 1) {
      section = COLUMNS;
      if(Debug)
	fprintf(stderr, "Switching to COLUMNS section\n");
    }
    else if(strcmp(tmp1, "RHS") == 0 && tmp == 1) {
      section = RHS;
      if(Debug)
	fprintf(stderr, "Switching to RHS section\n");
    }
    else if(strcmp(tmp1, "BOUNDS") == 0 && tmp == 1) {
      section = BOUNDS;
      if(Debug)
	fprintf(stderr, "Switching to BOUNDS section\n");
    }
    else if(strcmp(tmp1, "RANGES") == 0 && tmp == 1) {
      section = RANGES;
      if(Debug)
	fprintf(stderr, "Switching to RANGES section\n");
    }
    else if(strcmp(tmp1, "ENDATA") == 0 && tmp == 1) {
      if(Debug)
	fprintf(stderr, "Finished reading MPS file\n");
      break;
    }
    else if(tmp > 0) { /* normal line, process */
      record.reset();
      scan_line(line, record);
      record.lineno = Lineno;

      switch(section) {

      //
      // NAME
      //
      case NAME:
	EXCEPTION_MNGR(runtime_error, "readmps -- Error, extra line under NAME line");
	break;


      //
      // OBJSENSE
      //
      case OBJSENSE:
        {
        CharString sense;
        if (strstr(line,"MAX") != NULL)
           ILPread_set_sense(TRUE);
        else if (strstr(line,"MIN") != NULL)
           ILPread_set_sense(FALSE);
        else
           EXCEPTION_MNGR(runtime_error, "read_mps -- bad object sense: \"" << line << "\"");
        }
        break;


      //
      // ROWS
      //
      case ROWS:
	/* field1: rel. operator; field2: name of constraint */

	if(Debug) {
	  fprintf(stderr, "Rows line: ");
	  fprintf(stderr, "%s %s\n", record.field1.data(), 
						record.field2.data());
	}
         
	if(strcmp(record.field1.data(), "N") == 0) {
	  if(!OF_found) { /* take the first N row as OF, ignore others */
            ILPread_set_constr_type(record.field2.data(), record.field1[0]);
	    OF_found = TRUE;
	  }
	  else if(!Unconstrained_rows_found) {
	    fprintf(stderr, "Unconstrained row %s will be ignored\n", 
			record.field2.data());
	    fprintf(stderr,
		    "Further messages of this kind will be suppressed\n");
	    Unconstrained_rows_found = TRUE;
	  }
	}
	else if ( (strcmp(record.field1.data(), "L") == 0) ||
	          (strcmp(record.field1.data(), "G") == 0) ||
	          (strcmp(record.field1.data(), "E") == 0) ) {
          ILPread_set_constr_type(record.field2.data(), record.field1[0]);
        }
	else {
	  fprintf(stderr, "Unknown relat '%s' on line %d\n", 
			record.field1.data(), Lineno);
	  exit(EXIT_FAILURE);
	}
	break;


      //
      // COLUMNS
      //
      case COLUMNS:
	/* field2: variable; field3: constraint; field4: coef */
	/* optional: field5: constraint; field6: coef */

	if(Debug) {
	  fprintf(stderr, "Columns line: ");
	  fprintf(stderr, "%s %s %g %s %g\n", 
			record.field2.data(), record.field3.data(), 
			record.field4, record.field5.data(),
		  	record.field6);
	}

	if((record.items != 4) && (record.items != 6) && (record.items != 5)) {
	  fprintf(stderr,
		  "Wrong number of items (%d) in COLUMNS section (line %d)\n",
		  record.items, Lineno);
	  exit(EXIT_FAILURE);
	}

	if ((record.items == 4) || (record.items == 6)) {
           ILPread_add_coef(record.field3.data(), record.field2.data(), record.field4);
           if (Int_section) ILPread_set_int_var(record.field2.data());
           }
	if (record.items == 6)
           ILPread_add_coef(record.field5.data(), record.field2.data(), record.field6);
        if (record.items == 5) {
           // there might be an INTEND or INTORG marker 
           // look for "    <name>  'MARKER'                 'INTORG'"
           // or       "    <name>  'MARKER'                 'INTEND'" 
           if (strcmp(record.field3.data(), "'MARKER'") ==0) {
	      ////addmpscolumn();
	      if (strcmp(record.field5.data(), "'INTORG'") == 0) {
	         Int_section = TRUE;
	         if(Debug) fprintf(stderr, "Switching to integer section\n");
	         }
	      else if(strcmp(record.field5.data(), "'INTEND'") == 0) {
	         Int_section = FALSE;
	         if(Debug)fprintf(stderr, "Switching to non-integer section\n");
	         }
	      else fprintf(stderr, "Unknown marker (ignored) at line %d: %s\n",
		            Lineno, record.field5.data());
           }
	}
	break;


      //
      // RHS
      //
      case RHS:
	/* field2: uninteresting name; field3: constraint name */
	/* field4: value */
	/* optional: field5: constraint name; field6: value */

	if(Debug) {
	  fprintf(stderr, "RHS line: ");
	  fprintf(stderr, "%s %s %g %s %g\n", record.field2.data(), 
			record.field3.data(), record.field4, 
			record.field5.data(), record.field6);
	}

	if((record.items != 4) && (record.items != 6)) {
	  fprintf(stderr,
		  "Wrong number of items (%d) in RHS section line %d\n",
		  record.items, Lineno);
	  exit(EXIT_FAILURE);
	}

        ILPread_set_constr_bound(record.field3.data(), record.field4, ' ');
	if (record.items == 6) 
           ILPread_set_constr_bound(record.field5.data(), record.field6, ' ');
	break;


      //
      // BOUNDS
      //
      case BOUNDS:
	/* field1: bound type; field2: uninteresting name; */
	/* field3: variable name; field4: value */

	if(Debug) {
	  fprintf(stderr, "BOUNDS line: %s %s %s %g\n", record.field1.data(),
		record.field2.data(), record.field3.data(), record.field4);
	}

        ILPread_set_bounds(record.field3.data(), record.field1.data(), record.field4);
	break;
               
      //
      // RANGES
      //
      case RANGES:

	//
	// We have to implement the following semantics:
	//
	// D. The RANGES section is for constraints of the form: h <=
	// constraint <= u .  The range of the constraint is r = u - h .  The
	// value of r is specified in the RANGES section, and the value of u or
	// h is specified in the RHS section.  If b is the value entered in the
	// RHS section, and r is the value entered in the RANGES section, then
	// u and h are thus defined:
	//
        // row type       sign of r       h          u
        // ----------------------------------------------
        //   G            + or -         b        b + |r|
        //   L            + or -       b - |r|      b
        //   E              +            b        b + |r|
        //   E              -          b - |r|      b
	// 

	// field2: uninteresting name; field3: constraint name
	// field4: value
	// optional: field5: constraint name; field6: value

	if(Debug) {
	  fprintf(stderr, "RANGES line: ");
	  fprintf(stderr, "%s %s %g %s %g\n", record.field2.data(),
		record.field3.data(), record.field4, record.field5.data(),
		record.field6);
	}

	if((record.items != 4) && (record.items != 6)) {
	  fprintf(stderr,
		  "Wrong number of items (%d) in RANGES section line %d\n",
		  record.items, Lineno);
	  exit(EXIT_FAILURE);
	}

	if (record.field4 != 0) {
	  // find out constraint type. If ch_sign[row] is TRUE, it is GE. If
	  // ch_sign[row] is FALSE, it is an equality constraint if
	  // orig_upbo[row] == 0. For a LE constraint, orig_upbo[row] should be
	  // +infinity
	 
	  if (fabs(record.field4) >= HUGE_VAL) {
	     fprintf(stderr,
		    "Warning, Range for row %s >= infinity (value %g) on line %d, ignoring\n",
		    record.field3.data(), record.field4, Lineno);
	  }
	  else 
             ILPread_set_constr_bound(record.field3.data(), record.field4, 'R');
	}
	if ( (record.items == 6) && (record.field6 != 0) ) {
	    
	    if(fabs(record.field6) >= HUGE_VAL) {
	      fprintf(stderr,
		      "Warning, Range for row %s >= infinity (value %g) on line %d, ignoring\n",
		      record.field5.data(), record.field6, Lineno);
	    }
	    else
               ILPread_set_constr_bound(record.field5.data(), record.field6, 'R');
	}
	break;
      }
    }
  }

ILPread_process_input(A, objsen, objCoefs, lowerBounds,
                upperBounds, variableNames, constraintTypes, constraintLHS,
                constraintRHS, constraintNames, listOfIntegers);

ILPread_reset();
}

} // namespace pico
