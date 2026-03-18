C     ( Last modified on Tue May 28 14:07:02 CDT 2002 )
      SUBROUTINE MINTR( N   , X0   , MVAL  , DELTA, LWRBND, UPRBND,
     *                  A   , LDA  , NCLIN , NCNLN, WRK   , LWRK  ,
     *                  IWRK, LIWRK, INFORM, METHOD )
C
C  *******************************************************************
C  THIS SUBROUTINE FINDS THE MINIMUM OF THE QUADRATIC MODEL WITHIN THE
C  GIVEN REGION. THE REGION IS DEFINED BY THE INTERSECTION OF THE
C  TRUST REGION OF RADIUS DELTA AND THE ANALYTICALLY DEFINED FEASIBLE
C  SET OF THE ORIGINAL PROBLEM.
C
C                         T       T
C                MIN [GMOD X+0.5*X HMOD X]
C
C                  X0-DELTA <=  X   <= XO+DELTA
C        S.T.
C                               / X  \
C                        LB <= ( AX   ) <= UB
C                               \C(X)/
C  PARAMETERS:
C
C   N      (INPUT)  DIMENTION OF THE PROBLEM
C
C   X0     (INPUT)  ARRAY OF LENGTH N CONTAINING THE CENTER OF THE TRUST
C                   REGION
C          (OUTPUT) CONTAINS THE OPTIMAL POINT FOR THE MODEL
C
C   MVAL   (OUTPUT) THE VALUE OF THE MODEL AT THE OPTIMAL POINT
C
C   DELTA  (INPUT)  TRUST REGION RADIUS
C
C   LWRBND (INPUT)  ARRAY OF LENGHT N+NCLIN+NCNLN OF LOWER BOUNDS
C
C   UPRBND (INPUT)     ''       ''         ''        UPPER   ''
C
C   NCLIN  (INPUT)  NUMBER OF LINEAR ANALYTIC CONSTRAINTS
C
C   A      (INPUT)  (LDA X N) MATRIX OF LINEAR ANALYTIC CONSTRAINTS
C
C   NCNLN  (INPUT)  NUMBER OF NOLINEAR INEQUALITIES (DIFFICULT AND EASY)
C
C   WRK             REAL SPACE WORKING ARRAY
C
C   IWRK            INTEGER SPACE WORKING ARRAY
C
C   INFORM (OUTPUT) INFORMATION ON EXIT
C              0    SUCCESSFUL MINIMIZATION
C              1    THE DERIVATIVES OF THE CONSTRAINT OR SOME PARAMETER
C                   SET BY THE USER IS INCORRECT
C              2    MINIMIZATION FAILED FOR SOME REASON
C
C
C   METHOD (INPUT)  METHOD FOR HANDLING CONSTRAINTS
C              1    MINIMIZE MODEL OF OBJECTIVE S.T. MODELS OF CONSTRAINTS
C              2    MINIMIZE MERIT FUNCTION OF THE MODELS OF CON AND OBJ
C             3,4   MINIMIZE MODEL OF A MERIT FUNCTION (EXACT OR QUAD)
C
C  **********************************************************************
C
      IMPLICIT NONE

      INTEGER           N ,  NCLIN , NCNLN, LIWRK, LWRK, IWRK(LIWRK),
     +                  LDA, INFORM, METHOD


      DOUBLE PRECISION  X0(N), MVAL, DELTA, LWRBND(N+NCLIN+NCNLN),
     *                  UPRBND(N+NCLIN+NCNLN), WRK(LWRK), A(LDA,N)

      DOUBLE PRECISION DDOT

      EXTERNAL         DDOT


C HERE WE ASSUME THAT IPOPT HAS BEEN COMPILED WITH DYNAMIC MEMORY ALLOCATION
      INTEGER LRW, LIW
      PARAMETER( LRW=0, LIW=0 )
      INTEGER IW
      DOUBLE PRECISION RW

C USE THE FOLLOWING, IF IPOPT HAS NOT BEEN COMPILED TO USE DYNAMIC MEMORY
C ALLOCATION
C      INTEGER LRW, LIW
C      PARAMETER (LRW = 100000, LIW = 100000)
C      INTEGER IW( LIW )
C      DOUBLE PRECISION RW( LRW )

C
C     Parameters for IPOPT
C
      INCLUDE 'dfo_model_inc.f'
C
C  PRINTOUT PARAMETERS
C
      INTEGER          IOUT  , IPRINT
      DOUBLE PRECISION MCHEPS, CNSTOL

      COMMON / DFOCM /  IOUT, IPRINT, MCHEPS, CNSTOL
      SAVE / DFOCM /

      integer DFO_NMAX, DFO_MMAX
      parameter( DFO_MMAX = NCONMX + NNLNMX + NLINMX )
      parameter( DFO_NMAX = NVARMX + DFO_MMAX )
      double precision DFO_INF
      parameter( DFO_INF = 1.d20 )

      INTEGER NIPOPT, MIPOPT, NLB, NUB
      DOUBLE PRECISION X( DFO_NMAX )
      INTEGER ILB( DFO_NMAX )
      INTEGER IUB( DFO_NMAX )
      DOUBLE PRECISION BNDS_L( DFO_NMAX )
      DOUBLE PRECISION BNDS_U( DFO_NMAX )
      DOUBLE PRECISION V_L( DFO_NMAX )
      DOUBLE PRECISION V_U( DFO_NMAX )
      DOUBLE PRECISION LAM( DFO_MMAX )
      DOUBLE PRECISION C( DFO_MMAX )
      INTEGER IDAT(3)
      DOUBLE PRECISION DAT(1)
C
C     Algorithmic Parameters (INITPARAMS)
C
      INTEGER NARGS
      DOUBLE PRECISION ARGS( 50 )
      CHARACTER*20 CARGS( 50 )

      INTEGER ITER
      INTEGER IERR

      INTEGER I, IDUMMY, NCONT, NNLNT, NLINT, M
      DOUBLE PRECISION DUMMY, VAL

      EXTERNAL EVAL_F, EVAL_G, EVAL_C, EVAL_A, EVAL_H
      EXTERNAL EV_HLV_DUMMY, EV_HOV_DUMMY, EV_HCV_DUMMY

      USEIPOPT=1
      IF (METHOD .EQ. 2) THEN 
         USEMERIT=.TRUE.
      ELSE
         USEMERIT=.FALSE.
      ENDIF
      NCONT=NCON
      IF ( NCNLN .LE. NNLN .AND. .NOT.USEMERIT) NCON=0
      DO I = 1, N
        CALL DCOPY(NCLIN, A(1,I), 1, AMAT(1,I), 1)
      ENDDO
C
C     Consider bounds for original variables
C
      MIPOPT = NCLIN + NCNLN
      NIPOPT = N + MIPOPT
      IF( NIPOPT.GT.DFO_NMAX ) THEN
         WRITE(*,*) 'DFO_NMAX too small. Must be at least ',NIPOPT
         INFORM = 1
         GOTO 9999
      ENDIF
      IF( MIPOPT.GT.DFO_MMAX ) THEN
         WRITE(*,*) 'DFO_MMAX too small. Must be at least ',MIPOPT
         INFORM = 1
         GOTO 9999
      ENDIF
      DO I = 1, N
         ILB(I)    = I
         BNDS_L(I) = MAX(X0(I)-DELTA, LWRBND(I))
         IUB(I)    = I
         BNDS_U(I) = MIN(X0(I)+DELTA, UPRBND(I))
      enddo
      NLB = N
      NUB = N
C
C     We create slacks for all constraints.  For the equality constraints,
C     IPOPT will then treat them as fixed variables (and this way it also
C     knows about the right hand side.)
C
      DO I = N+1, N+MIPOPT
         IF( LWRBND(I).GT.-DFO_INF ) THEN
            NLB         = NLB + 1
            ILB(NLB)    = I
            BNDS_L(NLB) = LWRBND(I)
         ENDIF
         IF( UPRBND(I).LT. DFO_INF ) THEN
            NUB         = NUB + 1
            IUB(NUB)    = I
            BNDS_U(NUB) = UPRBND(I)
         ENDIF
      ENDDO
C
C     Compute initial point for IPOPT
C
      CALL DCOPY(N, X0, 1, X, 1)
      CALL FUNCON(1, NCNLN, N, NCNLN, 0, X0, X(N+1), DUMMY, IDUMMY)
C
C     Initialize parameters
C
      ARGS(1) = -1.d0
      CARGS(1) = 'IPRINT'
      ARGS(2) = 1.d3
      CARGS(2) = 'DMU0'
      ARGS(3) = 1.d-1
      CARGS(3) = 'DBNDPUSH'
      ARGS(4) = 1.d-1
      CARGS(4) = 'DBNDFRAC'
      ARGS(5) = 1.d-2
      CARGS(5) = 'DRESTOREDFACT'
      NARGS = 5
C
C     The following data is communicated for the EVAL_* routines
C
      IDAT(1) = N
      IDAT(2) = NCLIN
      IDAT(3) = NCNLN

      CALL IPOPT(NIPOPT, X, MIPOPT, NLB, ILB, BNDS_L, NUB, IUB, 
     .     BNDS_U, V_L, V_U, LAM, C, LRW, RW, LIW, IW, ITER, IERR,
     .     EVAL_F, EVAL_C, EVAL_G, EVAL_A, EVAL_H, EV_HLV_DUMMY,
     .     EV_HOV_DUMMY, EV_HCV_DUMMY, DAT, IDAT, NARGS, ARGS, CARGS)
      CALL DCOPY(N, X, 1, X0, 1)
       IF (IERR.EQ.0) THEN
         IF( IPRINT.GE.3 )    WRITE(IOUT,8000) 
         INFORM=0
       ELSE IF (IERR.EQ.1) THEN
         IF( IPRINT.GE.3 )    WRITE(IOUT,8004) 
         INFORM=3
       ELSE IF (IERR.GE.97 .OR. ( IERR .GE. 4 .AND. IERR .LE. 8)
     .          .OR. IERR .EQ. 15 ) THEN
         IF( IPRINT.GE.3 )   WRITE(IOUT,8009) 
         INFORM=1
       ELSE
         IF( IPRINT.GE.3 )    WRITE(IOUT,8001) 
         INFORM=3        
       ENDIF

       IF ( INFORM .EQ. 3 ) THEN
         INFORM = 0
         DO 40 I=1, N
           IF ( X0(I) .LT. BNDS_L(I) - CNSTOL .OR.
     *          X0(I) .GT. BNDS_U(I) + CNSTOL     ) 
     *         INFORM = 2
 40      CONTINUE
         IF ( NCLIN .GT. 0 .AND. INFORM .EQ. 0 ) THEN
           DO 70 I=1, NCLIN 
             VAL = DDOT(N, A(I, 1), LDA, X0, 1 )
             IF ( VAL .LT. LWRBND(N+I) - CNSTOL .OR.
     *            VAL .GT. UPRBND(N+I) + CNSTOL     ) 
     *            INFORM = 2
 70        CONTINUE
         ENDIF
         IF ( NCNLN .GT. 0 .AND. INFORM .EQ. 0 ) THEN
           CALL FUNCON(1, NCNLN+NCLIN, N, NCNLN+NCLIN, IWRK, X, 
     *                 WRK, WRK(NCNLN+NCLIN+1), 1)
           DO 60 I=1, NNLN 
             IF ( WRK(NLIN+I) .LT. LWRBND(N+NLIN+I) - CNSTOL .OR.
     *            WRK(NLIN+I) .GT. UPRBND(N+NLIN+I) + CNSTOL     ) 
     *            INFORM = 2
 60        CONTINUE
           IF ( .NOT. USEMERIT ) THEN 
             DO 80 I=1, NCON 
               IF ( WRK(NLIN+NNLN+I) .LT. LWRBND(N+NLIN+NNLN+I) - CNSTOL 
     *              .OR. WRK(NLIN+NNLN+I) .GT. UPRBND(N+NLIN+NNLN+I) 
     *              + CNSTOL     ) 
     *              INFORM = 3
 80          CONTINUE
           ENDIF
         ENDIF
       ENDIF
      CALL FUNOBJ(1, N, X, MVAL, WRK, 1)
      IF ( NCON .GT. 0 .AND. .NOT. USEMERIT) THEN
         IF ( INFORM .EQ. 0 ) THEN
           CALL DCOPY(NCON, X(N+NLIN+NNLN+1), 1, WRK, 1)
         ELSE
           NLINT=NLIN
           NNLNT=NNLN
           NNLN=0
           NLIN=0
           CALL FUNCON(1, NCON, N, NCON, IWRK, X, WRK, WRK(NCON+1), 1)
           NNLN=NNLNT
           NLIN=NLINT
         ENDIF
      ENDIF
 9999 CONTINUE
      NCON=NCONT
      RETURN
 8000  FORMAT( ' DFO: MINTR: SUCCESSFUL MINIMIZATION' )
 8001  FORMAT( ' DFO: MINTR: NO FURTHER IMPROVEMENT CAN BE OBTAINED' )
 8004  FORMAT( ' DFO: MINTR: MAXIMUM NUMBER OF ITER. REACHED IN IPOPT' )
 8009  FORMAT( ' DFO: MINTR: AN INPUT PARAMETER TO IPOPT IS INVALID' )
      END



C Copyright (C) 2002, Carnegie Mellon University and others.
C All Rights Reserved.
C This code is published under the Common Public License.
C*******************************************************************************
C
      subroutine EVAL_A(TASK, N, X, NZ, A, ACON, AVAR, DAT, IDAT)
C
C*******************************************************************************
C
C    $Id: mintr_ipopt.f,v 1.1.1.1 2006/09/06 16:16:46 wehart Exp $
C
C-------------------------------------------------------------------------------
C                                 Title
C-------------------------------------------------------------------------------
C
CT    Compute Jacobian of constraints for DFO problem
C
C-------------------------------------------------------------------------------
C                          Programm description
C-------------------------------------------------------------------------------
C
CB    
C
C-------------------------------------------------------------------------------
C                             Author, date
C-------------------------------------------------------------------------------
C
CA    Andreas Waechter      03/27/03
C
C-------------------------------------------------------------------------------
C                             Documentation
C-------------------------------------------------------------------------------
C
CD
C
C-------------------------------------------------------------------------------
C                             Parameter list    
C-------------------------------------------------------------------------------
C
C    Name     I/O   Type   Meaning
C
CP   TASK      I    INT     =0: Obtain NZ
CP                         <>0: Compute Jacobian
CP   N         I    INT    number of variables in problem statement
CP                            (including slacks for inequality constraints)
CP   X         I    DP     point where A is to be evaluated
CP   NZ       I/O   INT    TASK = 0: O: number of nonzero elements
CP                         otherwise: number of nonzero elements
CP                                     (size of A, AVAR, ACON)
CP   A         O    DP     (only TASK<>0) values in Jacobian
CP   ACON      O    INT    (only TASK<>0) row indices
CP   AVAR      O    INT    (only TASK<>0) column indices
CP   DAT       I    DP     ignored
CP   IDAT      I    INT    IDAT(1) = N
CP                         IDAT(2) = NCLIN
CP                         IDAT(3) = NCNLN
C
C-------------------------------------------------------------------------------
C                             local variables
C-------------------------------------------------------------------------------
C
CL
C
C-------------------------------------------------------------------------------
C                             used subroutines
C-------------------------------------------------------------------------------
C
CS    FUNCON
C
C*******************************************************************************
C
C                              Declarations
C
C*******************************************************************************
C
      IMPLICIT NONE
C
C*******************************************************************************
C
C                              Include files
C
C*******************************************************************************
C
C
C
C-------------------------------------------------------------------------------
C                             Parameter list
C-------------------------------------------------------------------------------
C
      integer TASK
      integer N
      double precision X(N)
      integer NZ
      double precision A(NZ)
      integer ACON(NZ)
      integer AVAR(NZ)
      double precision DAT(*)
      integer IDAT(3)
C
C-------------------------------------------------------------------------------
C                            Local varibales
C-------------------------------------------------------------------------------
C
      double precision dummy
      integer idummy, i, j
C
C*******************************************************************************
C
C                           Executable Statements
C
C*******************************************************************************
C
      if( TASK.eq.0 ) then
C
C     We assume dense Jacobian for original variables, and have the slacks
C     for all constraints
C
         NZ = (IDAT(1)+1)*(IDAT(2)+IDAT(3))

      else
C
C     Call FUNCON to get Jacobian (for nonslack variables)
C
         call FUNCON(2, IDAT(3), IDAT(1), IDAT(2)+IDAT(3), 0, X, dummy,
     1        A, idummy)
         NZ = 0
         do i = 1, IDAT(1)
            do j = 1, IDAT(2)+IDAT(3)
               NZ = NZ + 1
               ACON(NZ) = j
               AVAR(NZ) = i
            enddo
         enddo
C
C     Now add entries for the slacks
C
         do i = 1, IDAT(2)+IDAT(3)
            ACON(NZ+i) = i
            AVAR(NZ+i) = IDAT(1) + i
         enddo
         call DCOPY(IDAT(2)+IDAT(3), -1.d0, 0, A(NZ+1), 1)
         NZ = NZ + IDAT(2)+IDAT(3)

      endif

      return
      end
C Copyright (C) 2002, Carnegie Mellon University and others.
C All Rights Reserved.
C This code is published under the Common Public License.
C*******************************************************************************
C
      subroutine EVAL_C(N, X, M, C, DAT, IDAT)
C
C*******************************************************************************
C
C    $Id: mintr_ipopt.f,v 1.1.1.1 2006/09/06 16:16:46 wehart Exp $
C
C-------------------------------------------------------------------------------
C                                 Title
C-------------------------------------------------------------------------------
C
CT    Compute values of constraints for DFO problem
C
C-------------------------------------------------------------------------------
C                          Programm description
C-------------------------------------------------------------------------------
C
CB    
C
C-------------------------------------------------------------------------------
C                             Author, date
C-------------------------------------------------------------------------------
C
CA    Andreas Waechter      03/27/03
C
C-------------------------------------------------------------------------------
C                             Documentation
C-------------------------------------------------------------------------------
C
CD
C
C-------------------------------------------------------------------------------
C                             Parameter list    
C-------------------------------------------------------------------------------
C
C    Name     I/O   Type   Meaning
C
CP   N         I    INT    number of variables in problem statement
CP                            (including slacks for inequality constraints)
CP   X         I    DP     point where G is to be evaluated
CP   M         I    INT    number of constraints
CP   C         O    DP     values of constraints
CP   DAT       I    DP     ignored
CP   IDAT      I    INT    IDAT(1) = N
CP                         IDAT(2) = NCLIN
CP                         IDAT(3) = NCNLN
C
C-------------------------------------------------------------------------------
C                             local variables
C-------------------------------------------------------------------------------
C
CL
C
C-------------------------------------------------------------------------------
C                             used subroutines
C-------------------------------------------------------------------------------
C
CS    FUNOBJ
C
C*******************************************************************************
C
C                              Declarations
C
C*******************************************************************************
C
      IMPLICIT NONE
C
C*******************************************************************************
C
C                              Include files
C
C*******************************************************************************
C
C
C
C-------------------------------------------------------------------------------
C                             Parameter list
C-------------------------------------------------------------------------------
C                        
      integer N
      double precision X(N)
      integer M
      double precision C(M)
      double precision DAT(*)
      integer IDAT(3)
C
C-------------------------------------------------------------------------------
C                            Local varibales
C-------------------------------------------------------------------------------
C
      double precision dummy
      integer idummy
C
C*******************************************************************************
C
C                           Executable Statements
C
C*******************************************************************************
C

C
C     First get constraint values
C
      call FUNCON(1, IDAT(3), IDAT(1), IDAT(2)+IDAT(3), 0, X, C,
     1     dummy, idummy)
C
C     Now substract values of slacks
C
      call DAXPY(IDAT(2)+IDAT(3), -1.d0, X(IDAT(1)+1), 1, C, 1)

      return
      end
C Copyright (C) 2002, Carnegie Mellon University and others.
C All Rights Reserved.
C This code is published under the Common Public License.
C*******************************************************************************
C
      subroutine EVAL_F(N, X, F, DAT, IDAT)
C
C*******************************************************************************
C
C    $Id: mintr_ipopt.f,v 1.1.1.1 2006/09/06 16:16:46 wehart Exp $
C
C-------------------------------------------------------------------------------
C                                 Title
C-------------------------------------------------------------------------------
C
CT    Compute objective function value for DFO problem
C
C-------------------------------------------------------------------------------
C                          Programm description
C-------------------------------------------------------------------------------
C
CB    
C
C-------------------------------------------------------------------------------
C                             Author, date
C-------------------------------------------------------------------------------
C
CA    Andreas Waechter      03/27/03
C
C-------------------------------------------------------------------------------
C                             Documentation
C-------------------------------------------------------------------------------
C
CD
C
C-------------------------------------------------------------------------------
C                             Parameter list    
C-------------------------------------------------------------------------------
C
C    Name     I/O   Type   Meaning
C
CP   N         I    INT    number of variables in problem statement
CP                            (including slacks for inequality constraints)
CP   X         I    DP     point where F is to be evaluated
CP   F         O    DP     objective function value
CP   DAT       I    DP     ignored
CP   IDAT      I    INT    IDAT(1) = N
CP                         IDAT(2) = NCLIN
CP                         IDAT(3) = NCNLN
C
C-------------------------------------------------------------------------------
C                             local variables
C-------------------------------------------------------------------------------
C
CL
C
C-------------------------------------------------------------------------------
C                             used subroutines
C-------------------------------------------------------------------------------
C
CS    FUNOBJ
C
C*******************************************************************************
C
C                              Declarations
C
C*******************************************************************************
C
      IMPLICIT NONE
C
C*******************************************************************************
C
C                              Include files
C
C*******************************************************************************
C
C
C
C-------------------------------------------------------------------------------
C                             Parameter list
C-------------------------------------------------------------------------------
C                        
      integer N
      double precision X(N)
      double precision F
      double precision DAT(*)
      integer IDAT(3)
C
C-------------------------------------------------------------------------------
C                            Local varibales
C-------------------------------------------------------------------------------
C
      double precision dummy
      integer idummy
C
C*******************************************************************************
C
C                           Executable Statements
C
C*******************************************************************************
C

      call FUNOBJ(1, IDAT(1), X, F, dummy, idummy)

 9999 continue
      return
      end
C Copyright (C) 2002, Carnegie Mellon University and others.
C All Rights Reserved.
C This code is published under the Common Public License.
C*******************************************************************************
C
      subroutine EVAL_G(N, X, G, DAT, IDAT)
C
C*******************************************************************************
C
C    $Id: mintr_ipopt.f,v 1.1.1.1 2006/09/06 16:16:46 wehart Exp $
C
C-------------------------------------------------------------------------------
C                                 Title
C-------------------------------------------------------------------------------
C
CT    Compute gradient of objective function for DFO problem
C
C-------------------------------------------------------------------------------
C                          Programm description
C-------------------------------------------------------------------------------
C
CB    
C
C-------------------------------------------------------------------------------
C                             Author, date
C-------------------------------------------------------------------------------
C
CA    Andreas Waechter      03/27/03
C
C-------------------------------------------------------------------------------
C                             Documentation
C-------------------------------------------------------------------------------
C
CD
C
C-------------------------------------------------------------------------------
C                             Parameter list    
C-------------------------------------------------------------------------------
C
C    Name     I/O   Type   Meaning
C
CP   N         I    INT    number of variables in problem statement
CP                            (including slacks for inequality constraints)
CP   X         I    DP     point where G is to be evaluated
CP   G         O    DP     gradient of objective function
CP   DAT       I    DP     ignored
CP   IDAT      I    INT    IDAT(1) = N
CP                         IDAT(2) = NCLIN
CP                         IDAT(3) = NCNLN
C
C-------------------------------------------------------------------------------
C                             local variables
C-------------------------------------------------------------------------------
C
CL
C
C-------------------------------------------------------------------------------
C                             used subroutines
C-------------------------------------------------------------------------------
C
CS    FUNOBJ
C
C*******************************************************************************
C
C                              Declarations
C
C*******************************************************************************
C
      IMPLICIT NONE
C
C*******************************************************************************
C
C                              Include files
C
C*******************************************************************************
C
C
C
C-------------------------------------------------------------------------------
C                             Parameter list
C-------------------------------------------------------------------------------
C                        
      integer N
      double precision X(N)
      double precision G(*)
      double precision DAT(*)
      integer IDAT(3)
C
C-------------------------------------------------------------------------------
C                            Local varibales
C-------------------------------------------------------------------------------
C
      double precision f
      integer idummy
C
C*******************************************************************************
C
C                           Executable Statements
C
C*******************************************************************************
C
      call FUNOBJ(2, IDAT(1), X, f, G, idummy)
C
C     Add entries for slack variables
C
      call DCOPY(IDAT(2)+IDAT(3), 0.d0, 0, G(IDAT(1)+1), 1)

 9999 continue
      return
      end
C Copyright (C) 2002, Carnegie Mellon University and others.
C All Rights Reserved.
C This code is published under the Common Public License.
C*******************************************************************************
C

      subroutine EVAL_H(TASK, N, X, M, LAM, NNZH, HESS, IRNH, ICNH,
     1     DAT, IDAT)
C
C*******************************************************************************
C
C    $Id: mintr_ipopt.f,v 1.1.1.1 2006/09/06 16:16:46 wehart Exp $
C
C-------------------------------------------------------------------------------
C                                 Title
C-------------------------------------------------------------------------------
C
CT    Compute Hessian of Lagrangian for DFO problem
C
C-------------------------------------------------------------------------------
C                          Programm description
C-------------------------------------------------------------------------------
C
CB    
C
C-------------------------------------------------------------------------------
C                             Author, date
C-------------------------------------------------------------------------------
C
CA    Andreas Waechter      03/27/03
C
C-------------------------------------------------------------------------------
C                             Documentation
C-------------------------------------------------------------------------------
C
CD
C
C-------------------------------------------------------------------------------
C                             Parameter list    
C-------------------------------------------------------------------------------
C
C    Name     I/O   Type   Meaning
C
CP   TASK      I    INT     =0: Obtain NNZH
CP                         <>0: Compute Hessian
CP   N         I    INT    number of variables
CP   X         I    DP     point at which constraints are to be evaluated
CP   M         I    INT    number of equality constraints
CP   LAM       I    DP     vector of Lagrangian multipliers
CP   NNZH     I/O   INT    size of HESS, IRNH, ICNH
CP   HESS      O    DP     nonzero elements of Hessian
CP   IRNH      O    INT    row indices of nonzero elements
CP   ICNH      O    INT    column indices of nonzero elements
CP                         (for each i=1,..,NNZH the nonzero element HESS(i)
CP                         is the element of Hessian in row IRNH(i) and column
CP                         ICNH(i) as well as in row ICNH(i) and column IRNH(i).
CP                         For non-diagonal elements provide only one of them.)
CP   DAT       I    DP     ignored
CP   IDAT      I    INT    IDAT(1) = N
CP                         IDAT(2) = NCLIN
CP                         IDAT(3) = NCNLN
C
C-------------------------------------------------------------------------------
C                             local variables
C-------------------------------------------------------------------------------
C
CL
C
C-------------------------------------------------------------------------------
C                             used subroutines
C-------------------------------------------------------------------------------
C
CS    FUNCON
C
C*******************************************************************************
C
C                              Declarations
C
C*******************************************************************************
C
      IMPLICIT NONE
C
C*******************************************************************************
C
C                              Include files
C
C*******************************************************************************
C
      INCLUDE 'dfo_model_inc.f'
C
C
C-------------------------------------------------------------------------------
C                             Parameter list
C-------------------------------------------------------------------------------
C
      integer TASK
      integer N
      integer M 
      integer NNZH
      double precision LAM(M)
      double precision X(N)
      double precision HESS(NNZH)
      integer IRNH(NNZH)
      integer ICNH(NNZH)
      double precision DAT(*)
      integer IDAT(3)
C
C-------------------------------------------------------------------------------
C                            Local varibales
C-------------------------------------------------------------------------------
C
      integer idummy, i, j
      double precision h(NVARMX, NVARMX)
C
C*******************************************************************************
C
C                           Executable Statements
C
C*******************************************************************************
C
      if( TASK.eq.0 ) then
C
C     Get number of nonzeros in Hessian of the Lagrangian.  Assume dense
C     Hessian (for original variables and lower triangular part only)
C
         NNZH = (IDAT(1)*(IDAT(1)+1))/2
      else
         if( IDAT(1).gt. NVARMX) then
            write(*,*)
     1           'In eval_h: NVARMX too small.  Must be at least',
     2           IDAT(1)
            stop
         endif
C
C     Call FUNCON to get values for Hessian
C
         call FUNCON(3, IDAT(3), IDAT(1), NVARMX, 0, X, LAM,
     *        h, idummy)
C
C     Now copy out the lower triangular part
C
         NNZH = 0
         do i = 1, IDAT(1)
            do j = 1, i
               NNZH = NNZH + 1
               HESS(NNZH) = h(i,j)
               IRNH(NNZH) = i
               ICNH(NNZH) = j
            enddo
         enddo
               
      endif

 9999 continue
      return
      end
