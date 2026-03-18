/* glpqmd.h (quotient minimum degree algorithm) */

/*----------------------------------------------------------------------
-- Copyright (C) 2000, 2001, 2002, 2003, 2004, 2005 Andrew Makhorin,
-- Department for Applied Informatics, Moscow Aviation Institute,
-- Moscow, Russia. All rights reserved. E-mail: <mao@mai2.rcnet.ru>.
--
-- This file is part of GLPK (GNU Linear Programming Kit).
--
-- GLPK is free software; you can redistribute it and/or modify it
-- under the terms of the GNU General Public License as published by
-- the Free Software Foundation; either version 2, or (at your option)
-- any later version.
--
-- GLPK is distributed in the hope that it will be useful, but WITHOUT
-- ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
-- or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public
-- License for more details.
--
-- You should have received a copy of the GNU General Public License
-- along with GLPK; see the file COPYING. If not, write to the Free
-- Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
-- 02111-1307, USA.
----------------------------------------------------------------------*/

#ifndef _GLPQMD_H
#define _GLPQMD_H

#define genqmd                glp_qmd_genqmd
#define qmdrch                glp_qmd_qmdrch
#define qmdqt                 glp_qmd_qmdqt
#define qmdupd                glp_qmd_qmdupd
#define qmdmrg                glp_qmd_qmdmrg

extern void genqmd(int *neqns, int* xadj, int* adjncy, int* perm,
      int* invp, int* deg, int* marker, int* rchset, int* nbrhd,
      int* qsize, int* qlink, int *nofsub);
/* GENeral Quotient Minimum Degree algorithm */

extern void qmdrch(int *root, int* xadj, int* adjncy, int* deg,
      int* marker, int *rchsze, int* rchset, int *nhdsze,
      int* nbrhd);
/* Quotient MD ReaCHable set */

extern void qmdqt(int *root, int* xadj, int* adjncy, int* marker,
      int *rchsze, int* rchset, int* nbrhd);
/* Quotient MD Quotient graph Transformation */

extern void qmdupd(int* xadj, int* adjncy, int *nlist, int* list,
      int* deg, int* qsize, int* qlink, int* marker, int* rchset,
      int* nbrhd);
/* Quotient MD UPDate */

extern void qmdmrg(int* xadj, int* adjncy, int* deg, int* qsize,
      int* qlink, int* marker, int *deg0, int *nhdsze, int* nbrhd,
      int* rchset, int* ovrlp);
/* Quotient MD MeRGe */

#endif

/* eof */
