# THAI NAVY PROBLEM
#
# This model is used to allocate ships to transport personnel from
# different port to a training center.
#
# Reference: Choypeng P, Puakpong P and Rosenthal R E, Optimal Ship
#            Routing and Personnel Assignment for Naval Recruitment
#            in Thailand, Interfaces, Vol 16, No 4, pp.356-366, 1986.
#
# Translated from GAMS by Andrew Makhorin <mao@mai2.rcnet.ru>

set I;
/* ports */

set J;
/* voyages */

set K;
/* ship classes */

set SC within I cross K;
/* ship capability */

param D{i in I};
/* number of men at port i needing transport */

param SHIPCAP{k in K};
/* ship capacity in men */

param N{k in K};
/* number of ships of class k available */

param A{J, {"DIST"} union I};
/* assignment of ports to voyages */

param W1 := 1.00;
/* ship assignment weight */

param W2 := .01;
/* ship distance traveled weight */

param W3 := .0001;
/* personnel distance travel weight */

set VC within J cross K :=
   {j in J, k in K: forall{i in I: A[j,i]} (i,k) in SC};
/* voyage capability */

var Z{j in J, k in K} integer;
/* number of times voyage (j,k) is used */

var Y{j in J, k in K, i in I} >= 0;
/* number of men transported from port i via voyage (j,k) */

s.t. DEMAND{i in I}:
   sum{j in J, k in K: A[j,i] and (j,k) in VC} Y[j,k,i] >= D[i];
/* pick up all the men at port i */

s.t. VOYCAP{j in J, k in K: (j,k) in VC}:
   sum{i in I: A[j,i]} Y[j,k,i] <= SHIPCAP[k] * Z[j,k];
/* observe variable capacity of voyage (j,k) */

s.t. SHIPLIM{k in K}: sum{j in J: (j,k) in VC} Z[j,k] <= N[k];
/* observe limit of class k */

minimize OBJ:
   W1 * sum{j in J, k in K: (j,k) in VC} Z[j,k] +
   W2 * sum{j in J, k in K: (j,k) in VC} A[j,"DIST"] * Z[j,k] +
   W3 * sum{j in J, k in K, i in I: A[j,i] and (j,k) in VC}
      A[j,"DIST"] * Y[j,k,i];

solve;

printf{1..56} "-"; printf "\n";
printf "Number of men transported:\n\n";
printf "%-12s", "";
for {i in I} printf " %10s", i;
printf "\n";
for {j in J, k in K: sum{i in I} Y[j,k,i] != 0}
{  printf "%-12s", j & "." & k;
   for {i in I} printf " %10d", Y[j,k,i];
   printf "\n";
}
printf{1..56} "-"; printf "\n";
printf "Number of times voyage used:\n\n";
printf "%-12s", "";
for {k in K} printf " %10s", k;
printf "\n";
for {j in J: sum{k in K} Z[j,k] != 0}
{  printf "%-12s", j;
   for {k in K} printf " %10d", Z[j,k];
   printf "\n";
}
printf{1..56} "-"; printf "\n";

data;

set I := CHUMPHON SURAT NAKON SONGKHLA ;

set J := V-01 V-02 V-03 V-04 V-05 V-06 V-07 V-08 V-09 V-10 V-11 V-12
         V-13 V-14 V-15 ;

set K := SMALL MEDIUM LARGE ;

set SC : SMALL MEDIUM LARGE :=
CHUMPHON   +     +      +
SURAT      -     +      +
NAKON      -     +      +
SONGKHLA   -     -      +   ;

param D := CHUMPHON 475, SURAT 659, NAKON 672, SONGKHLA 1123 ;

param SHIPCAP := SMALL 100, MEDIUM 200, LARGE 600 ;

param N := SMALL 2, MEDIUM 3, LARGE 4 ;

param A default 0:
      DIST  CHUMPHON SURAT NAKON SONGKHLA :=
V-01   370     1       .     .      .
V-02   460     .       1     .      .
V-03   600     .       .     1      .
V-04   750     .       .     .      1
V-05   515     1       1     .      .
V-06   640     1       .     1      .
V-07   810     1       .     .      1
V-08   665     .       1     1      .
V-09   665     .       1     .      1
V-10   800     .       .     1      1
V-11   720     1       1     1      .
V-12   860     1       1     .      1
V-13   840     1       .     1      1
V-14   865     .       1     1      1
V-15   920     1       1     1      1     ;

end;
