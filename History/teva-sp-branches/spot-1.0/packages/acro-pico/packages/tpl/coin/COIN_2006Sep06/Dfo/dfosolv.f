      SUBROUTINE DFOSLV( N  , M   , NCLIN, NCNLN, X   , FX   ,
     +                   C  , NQ  , BASE , IP   , IV  , ID   , 
     +                   IOB, IC  , LB  , UB   ,
     +                   A  , LDA , SCALE, SCAL , PP  , EXIT ,   
     +                   IT , NF  , NOISE, IPAR , RPAR, IBEGW, 
     +                   WRK, LWRK, IWRK , LIWRK)  
C
C  ******************************************************************
C  THIS SUBROUTINE MINIMIZES A NONLINEAR OBJECTIVE FUNCTION
C  SUBJECT TO LINEAR AND (POSSIBLY) NONLINEAR CONSTRAINTS
C  AND SIMPLE BOUNDS, WITHOUT USING DERIVATIVES OF 
C  THE OBJECTIVE FUNCTION.
C
C  THIS ALGORITHM  IS BASED ON USING QUADRATIC INTERPOLATION OF THE
C  OBJECTIVE FUNCTION IN COMBINATION WITH TRUST REGION FRAMEWORK
C
C
C  PARAMETERS
C  ==========
C
C  (INPUT) 
C
C    X         : ARRAY OF NX  INITIAL POINTS, GIVEN BY THE USER
C
C    LDX       : LEADING DIMENSION OF ARRAY 'X' (LDX>= N)
C
C    FX        : ARRAY OF FUNCTION VALUES AT THE INITIAL POINTS
C                OF LENGTH AT LEAST 1
C
C    NX        : NUMBER OF INITIAL POINTS FOR WHICH THE VALUE IS
C                PROVIDED.
C
C    N         : PROBLEM DIMENSION
C
C    NCLIN     : NUMBER OF LINEAR CONSTRAINTS
C
C    NCNLN     : NUMBER OF NONLINEAR CONSTRAINTS
C
C    NOISE     : NOISE(1) - ABSOLUTE NOISE LEVEL
C                NOISE(2) - RELATIVE NOISE LEVEL
C                ( IF KNOWN BY THE USER )
C
C    IPAR      : ARRAY OF INITIAL INTEGER PARAMETERS
C
C    RPAR      : ARRAY OF INITIAL REAL PARAMETERS
C
C    LB        : ARRAY OF LOWER BOUNDS OF LENGTH >= (N+NCLIN+NCNLN)
C
C    UB        : ARRAY OF UPPER BOUNDS OF LENGTH >= (N+NCLIN+NCNLN)
C
C    A         : MATRIX OF LINEAR CONSTRAINTS,( DIMENSIONS: LDA X N)
C
C    LDA       : LEADING DIMENSION OF MATRIX A, HAS TO BE >= MAX(1, NCLIN)
C
C    WRK       : REAL WORKSPACE ARRAY OF SIZE LWRK
C 
C    IWRK      : INTEGER WORKSPACE ARRAY OF SIZE LIWRK
C
C    (OUTPUT)
C
C     X        : THE OPTIMAL (OR BEST SO FAR)  POINT 
C
C     FX       : THE VALUE OF OBJECTIVE FUNCTION AT X
C
C     EXIT     : THE EXIT INFORMATION:
C                0     SUCCESSFUL MINIMIZATION
C                1     TOO MANY FUNCTION EVALUATIONS
C                2     TOO MANY ITERATIONS
C               -3     REAL WORKSPACE IS TOO SMALL
C               -4     INTEGER WORKSPACE IS TOO SMALL
C               -7     MINIMIZATION OVER TRUST REGION FAILED
C               -9     CANNOT FIND TWO POINTS TO BUILD INITIAL MODEL
C     IT       : THE NUMBER OF ITERATIONS
C
C     NF       : THE NUMBER OF FUNCTION EVALUATIONS
C  ********************************************************************
C

C
C  SUBROUTINE PARAMETERS
C
      INTEGER          N, M , NCLIN    , NCNLN, IT  , EXIT  , NF   ,   
     +                 MAXIT, MAXNF    , LIWRK, LWRK, IWRK( LIWRK ),
     +                 VARNT, IPAR( 8 ), SCALE, LDA , IC, IOB, IBEGW

      DOUBLE PRECISION X(N)   , NOISE( 2 ), RPAR( 8 )  , DELMIN , 
     +                 LOWBND , FX        , WRK( LWRK ), SCAL(N),
     +                 LB( * )    ,
     +                 UB(*), A( LDA*N ) , 
     +                 PP , C(*)



C
C  COMMON VARIABLES
C
      INCLUDE 'dfo_model_inc.f'
C
C  LENGTH OF ARRAYS
C
      INTEGER          LPOLY, LPNTS, LVALUE, LPTINT, LVLINT, LCONVL
      COMMON /RPART/   LPOLY, LPNTS, LVALUE, LPTINT, LVLINT, LCONVL
      SAVE /RPART/

C
C  PROBLEM CONTROL PARAMETERS
C
      INTEGER          IOUT  , IPRINT
      DOUBLE PRECISION MCHEPS, CNSTOL
      COMMON / DFOCM / IOUT  , IPRINT, MCHEPS, CNSTOL
      SAVE / DFOCM /

C
C  INTERPOLATION CONTROL PARAMETERS
C      
      INTEGER          NPMIN, LAYER, EFFORT
      COMMON / OPTI /  NPMIN, LAYER, EFFORT
      SAVE / OPTI /


C
C
C  LOCAL VARIABLES
C  ---------------
C


      INTEGER          IMPR  , NSTOP , BASE  , I     , NQ    , ITER  ,
     +                 LP    , LV    , LK    , IK    , LG    , LH    ,
     +                 LCURW , IG    , IH    , IP    , IV    , ICURW , 
     +                 IBEG  , LSP2IN, NEXT  , INQ   , LNQ   , IBASE ,
     +                 J     , OBASE , IPEND , LBASE , LRS   , LIS   ,
     +                 LD    , ID    , II    , ICURIW, ISP2IN, LPIV  ,
     +                 INP   , NIND  , IVI   , INEXT , IPOLY , IFADD ,
     +                 IFXG  , IPI   , DD    , RSNEED, LNP   , LPI   ,
     +                 MNTRIS, NPSLIS, IMPDRS, LVI   , IPIV  , LIN2SP, 
     +                 IIN2SP, INFORM, OLDNQ , LCURIW, PTNWRS, MNTRRS, 
     +                 NPSLRS, MDBLRS, GTDSRS, MDBLIS, NEQCON, LOB   ,
     +                 LC    , LCQ   , LCL   , LCC   , LCI   , K     ,
     +                 KK    , ICI   , ICQ   , ICL   , ICC   , NINDR ,
     +                 METHOD, STOPCNT, HOWSTOP

      LOGICAL          FAIL  , OKGEOM, LINEAR, IFERR , USEMER

      DOUBLE PRECISION DELTA , DELMAX, ANOISE, RNOISE, RATIO , RHO   ,
     +                 FBASE , MVAL  , PRERED, NOISEF, FNQ   , THETA ,
     +                 FT    , STNORM, RHOW  , DELTHR, VAL   , SNORM ,
     +                 KAPPA , PIVTHR, ADDTHR, PIVT  , ADDT  , XCHTHR,
     +                 KMOD  , DEL   , ODELTA, WHENSTOP

      DOUBLE PRECISION ZERO  , ONE   , TWO   , TEN   ,  HALF , THOUS
      PARAMETER      ( ZERO  = 0.0D0 , TEN = 10.0D0  ,  TWO   =  2.0D0 )
      PARAMETER      ( HALF  = 0.5 D0, ONE = 1.0D0   ,  THOUS =  1.0D3 )

      DOUBLE PRECISION MVALUE, DNRMNF 
      EXTERNAL         MVALUE, DNRMNF
      INTRINSIC        MIN   , MAX   , ABS
C
C     TRUST REGION METHOD PARAMETERS
C
      DOUBLE PRECISION RHOMIN, RHOINC, RHOJMP,  MAXJMP

*
      PARAMETER      ( RHOMIN = 0.05D0, RHOJMP =  0.9D0 ) 
      PARAMETER      ( RHOINC = 0.25D0, MAXJMP =  1.0D2 )

*
      INTEGER          SEED
      PARAMETER      ( SEED = 926865394 )

C
C     SUBROUTINES AND FUNCTIONS CALLED:
C
C       APPLICATION:       PTINIT, MDBLD , MINTR , FUN   , GETDIS, 
C                          PTREPL, PTEXCH, NEXTNP, EVALX , IMPMOD
C                          SCL   , UNSCL , SHIFT , UNSHFT, DNRMNF,
C                          EVALNP, BESTPT, SWAPNP, MVALUE
C       FORTRAN SUPPLIED:  MAX   , MIN   , ABS
C       BLAS:              DCOPY 
C


C
C  SET VARIOUS PARAMETERS
C

C
C  MAXIMUM NUMBER OF ITERATIONS
C
      MAXIT  = IPAR( 1 )
C
C  MAXIMUM NUMBER OF FUNCTION EVALUATIONS
C
      MAXNF  = IPAR( 2 )
C
C  THE DESIRED MINIMUM NUMBER OF POINT IN AN INTERPOLATION SET
C
      NPMIN  = IPAR( 3 )
C
C  THE CONTROL PARAMETER OF MAXIMUM DISTANCE OF POINTS TO THE BASE
C
      LAYER  = IPAR( 4 )
C
C  THE LEVEL OF EFFORT OF INTERPOLATION COMPUTATIONS
C  
      EFFORT = IPAR( 5 )
C
C  FLAG RESPONSIBLE FOR THE CHOICE OF INCOMPLETE MODEL: 
C  1 - MINIMUM FROBENIUS NORM, 2 - DERIVED FROM NEWTON POLYNOMIAL BASIS
C
      VARNT  = IPAR( 6 )
C
C  METHOD USED TO HANDLE CONSTRAINTS 3,4 - PENALTY FUNCTION OPTIMIZATION
C                                    1,2 - MODELING CONSTRAINTS
C
      METHOD  = IPAR( 7 )
C
C  WHICH STOPPING CRITERIA TO USE   1 - SIZE OF THE TRUST REGION, TENDS TO
C                                       PRODUCE MORE ACCURATE RESULTS
C                                   2 - SLOW PROGRESS, TENDS TO STOP SOONER
C
      HOWSTOP  = IPAR( 8 )
C
C  THE INITIAL TRUST REGION RADIUS
C
      DELTA  = RPAR( 1 )
C
C  MAXIMUM TRUST REGION RADIUS
C
      DELMAX = RPAR( 2 )
C
C  MINIMUM TRUST REGION RADIUS (STOPPING CRITERIA)
C
      DELMIN = RPAR( 3 )
C
C  RELATIVE (W.R.T THRUST REGION RADIUS SIZE) PIVOT THRESHOLD
C
      PIVT   = RPAR( 4 )
C
C  RELATIVE THRESHOLD OF ADDING A TRUST REGION POINT WITH BAD RHO 
C
      ADDT   = RPAR( 5 )
C
C  MINIMUM REQUIRE FACTOR OF PIVOT IMPROVEMENT WHILE REPLACING
C  AN INTERPOLATION POINT BY A  GEOMETRY POINT
C
      XCHTHR = RPAR( 6 )
C
C  LOWER BOUND ON DESIRED MINIMUM FUNCTION VALUE 
C
      LOWBND = RPAR( 7 )
C
C  THRESHHOLD FOR CONSIDERING INSUFFICIENT PROGRESS
C
      WHENSTOP = RPAR( 8 )
C
C  ABSOLUTE NOISE LEVEL
C
      ANOISE = NOISE( 1 )
C
C  RELATIVE NOISE LEVEL
C
      RNOISE = MAX( NOISE( 2 ), MCHEPS )




C  ------------------------------------------------------------
C  PARTITION THE REAL WORKSPACE
C  ------------------------------------------------------------

C
C  MAXIMUM NUMBER OF INTERPOLATION POINTS/POLYNOMIALS 
C  IN A QUADRATIC MODEL
C
      DD     = ((N + 1)*(N + 2))/2
C
C  MAXIMUM LENGTH OF ARRAY OF  NEWTON FUNDAMENTAL POLYNOMIALS
C 
      LPOLY  = DD*(N+1)*N/2+(N+1)*N+1
C
C  MAXIMUM LENGTH OF ARRAY OF INTERPOLATION POINTS
C
      LPTINT = DD*N
C
C  MAXIMUM LENGTH OF ARRAY OF FUNCTION  VALUES OF INTERP. POINTS
C
      LVLINT = DD



C 
C  POINTERS OF ARRAYS IN REAL WORKSPACE
C  -----------------------------------


C
C  POINTER TO NEWTON FUNDAMENTAL POLYNOMIALS
C
      INP = IBEGW
C
C  POINTER TO ARRAY OF INTERPOLATION POINTS
C
      IPI = INP + LPOLY
C
C  POINTER TO FUNCTION VALUES AT INTERPOLATION POINTS
C
      IVI = IPI + DD*N
C
C  POINTS TO CONSTRAINT VALUES AT INTERPOLATION POINTS
C
      ICI = IVI + DD
C
C  POINTER TO PIVOT VALUES ASSOCIATED WITH INTERPOLATION POINTS
C
      IPIV= ICI + DD
C
C  POINTER TO LINEAR TERMS OF THE QUADRATIC MODEL
C
      IG  = IPIV + DD
C
C  POINTER TO QUADRATIC TERMS OF THE QUADRATIC MODEL
C
      IH  = IG  + N
C
C  POINTER TO CONSTANT TERMS OF THE MODEL OF CONSTRAINTS
C
      ICC = IH  + N * N
C
C  POINTER TO LINEAR TERMS OF THE MODEL OF CONSTRAINTS 
C
      ICL = ICC + M
C
C  POINTER TO QUADRATIC TERMS OF THE MODEL OF CONSTRAINTS 
C
      ICQ = ICL + M * N
C
C  POINTER TO AUXILIARY SPACE FOR NEW POINTS
C
      IK  = ICQ  + N * N * M
C
C  POINTER TO BEGINNING OF WORKING SPACE
C
      ICURW = IK + N
C
C  THE LENGTH OF THE REMAINING SPACE
C
      LRS    = LWRK - ICURW + 1
C
C  SPACE REQUIRED BY 'MINTR' SUBROUTINE
C
      MNTRRS = 3*(N+NCLIN+M+NCNLN) + (N+1)*MAX(1,NCNLN+M) + N*N + N
C
C  SPACE REQUIRED BY 'NPSOL' SUBROUTINE
C
      NPSLRS = 2*N*N + N*NCLIN + 2*N*(NCNLN+M) + 20*N + 
     +         11*NCLIN + 21*(NCNLN+M)
C
C  SPACE REQUIRED BY 'MDLBLD' SUBROUTINE
C
      MDBLRS = DD*DD + (DD-1)*N + DD - 1
C
C  SPACE REQUIRED BY 'PTNEW' SUBROUTINE
C
      PTNWRS = 2*N + N*N
C
C  SPACE REQUIRED BY 'GETDIS' SUBROUTINE 
C
      GTDSRS = N
C
C  SPACE REQUIRED BY 'IMPMOD' SUBROUTINE
C
      IMPDRS = N
C
C  CHECK IF THE REMAINING REAL SPACE IS SUFFICIENT  
C  BY COMPARING IT  WITH THE "CRITICAL PATH"
C  -----------------------------------------------

      RSNEED = PTNWRS + MNTRRS + NPSLRS + IMPDRS
      IF ( LRS .LT.  MAX( RSNEED, MDBLRS ) ) 
     +   THEN
         IF ( IPRINT .GE. 0 ) WRITE( IOUT, 2000 ) 
     +          MAX(RSNEED, MDBLRS) - LRS
         EXIT = -3
         RETURN
      ENDIF

C
C  HELPFUL 'SHIFTED' POINTERS TO ARRAYS
C
      LD    = ID    - 1
      LP    = IP    - 1
      LV    = IV    - 1
      LC    = IC    - 1
      LOB   = IOB   - 1
      LG    = IG    - 1
      LH    = IH    - 1
      LCC   = ICC   - 1
      LCL   = ICL   - 1
      LCQ   = ICQ   - 1
      LP    = IP    - 1
      LV    = IV    - 1
      LPI   = IPI   - 1
      LVI   = IVI   - 1
      LCI   = ICI   - 1
      LPIV  = IPIV  - 1
      LK    = IK    - 1
      LD    = ID    - 1
      LNP   = INP   - 1
      LCURW = ICURW - 1



C  ---------------------------------------------------------------
C  PARTITION THE INTEGER WORKSPACE
C  ---------------------------------------------------------------

C
C  POINTER TO ARRAY OF 0/1 INDICATORS: 1 - IF A SAMPLE POINT 
C  IS INCLUDED IN THE CURRENT INTERPOLATION SET, 0 - OTHERWISE
C
      ISP2IN = 1
C
C  POINTER TO ARRAY INDICATING FOR EVERY INTERPOLATION POINTS 
C  ITS LOCATION IN SAMPLE SET
C
      IIN2SP = ISP2IN+MAXNF
C
C  POINTER TO THE INTEGER WORKING SPACE
C  ------------------------------------
   
      ICURIW = IIN2SP  + DD

C
C  LENGTH OF THE REMAINING INTEGER SPACE
C
      LIS = LIWRK - ICURIW + 1
C
C  INTEGER SPACE REQUIRES BY 'MINTR' PROCEDURE
C
      MNTRIS = N + NCLIN + NCNLN + M
C
C  INTEGER SPACE REQUIRES BY 'NPSOL' PROCEDURE
C
      NPSLIS = 3*N + NCLIN + 2*(NCNLN + M)
C
C  INTEGER SPACE REQUIRES BY 'MDBLD' PROCEDURE
C
      MDBLIS = DD
C
C  CHECK IF THE REMAINING INTEGER SPACE IS SUFFICIENT
C  BY COMPARING IT WITH THE "CRITICAL PATH"
C
      IF ( LIS .LT. MAX(MNTRIS + NPSLIS, MDBLIS) ) THEN
         IF ( IPRINT .GE. 0 ) WRITE( IOUT, 2020 ) 
     +        MAX(MNTRIS + NPSLIS, MDBLIS) - LIS
         EXIT = -4
         RETURN
      ENDIF
C
C  HELPFUL 'SHIFTED' POINTERS TO INTEGER ARRAYS
C
      LSP2IN = ISP2IN - 1
      LIN2SP = IIN2SP - 1
      LCURIW = ICURIW - 1

C
C  SET PARAMETERS FOR /MERIT/ COMMON BLOCK
C 
      NCON=M
      NLIN=NCLIN
      NNLN=NCNLN
      PENPAR=PP
      DO 5 I=1, M
        CONL(I)=LB(N+NCLIN+NCNLN+I)
        CONU(I)= UB(N+NCLIN+NCNLN+I)
 5    CONTINUE  
      


C  ----------------------------------------------------
C  INITIAL INTERPOLATION SET IS BUILD HERE
C  ----------------------------------------------------





C
C  CALL SUBROUTINE 'NBUILD' TO COMPUTE INITIAL: 
C       INTERPOLATION SET AND 
C       NEWTON FUNDAMENTIAL POLYNOMIALS
C       DISTANCES OF SAMPLE POINTS TO BASE POINT
C       PIVOT VALUES ASSOCIATED WITH INTERPOLATION SET
C       ARRAYS IN2SP AND SP2IN CONNECTING SAMPLE SET WITH INTERP. SET
C

 
      IF ( IPRINT .GE. 2 ) WRITE( IOUT, 8000 ) 
      PIVTHR = PIVT*DELTA
      CALL DCOPY (N, WRK(IP+(BASE-1)*N), 1, WRK(IPI), 1)
      WRK(IVI) = WRK(IV + BASE - 1)
      IWRK(IIN2SP)=BASE
      IWRK( ISP2IN + BASE - 1 ) = 1
      BASE = 1
C
C  COMPUTE THE NUMBER OF LINEARLY INDEPENDENT INEQUALITY CONSTRAINTS
C           
      CALL DCOPY ( N, WRK(IP+(BASE-1)*N), 1, WRK(IK), 1 )
      CALL UNSHFT( N, X, WRK(IK) )
      CALL INTDIM( WRK(IK)   , N   , NCLIN , NCNLN , NEQCON,   
     +             A, LDA    , LB  , UB    , PIVTHR,   
     +             WRK(ICURW), LRS , IWRK(ICURIW)  , LIS    )
C
C  COMPUTE THE BASIS OF POLYNOMIALS
C
      CALL NBUILD( WRK(INP)    , WRK(IP)     , WRK(IV)     , WRK(IPI),
     +             WRK(IVI)    , IWRK(ISP2IN), IWRK(IIN2SP), NIND    , 
     +             N           , BASE        , WRK(ID)     , DELTA   , 
     +             PIVTHR      , WRK(IPIV)   , NEQCON )   
 


C
C  IF THE BASIS CONSISTS ONLY OF A SINGLE POLYNOMIAL (CONSTANT TERM)
C  THEN SOMETHING IS WRONG. WE STOP AND PRINT A MESSAGE      
C
      IF (NIND.LT.2) THEN
        IF ( IPRINT .GE. 0 ) WRITE(IOUT, 2060)
        EXIT=-9
        RETURN
      ENDIF

 
C
C  INITIALIZE  POINTERS TO BASE POINT AND TO THE LAST SAMPLE POINT
C
      LBASE = LPI + ( BASE - 1 ) * N
      IBASE = LBASE + 1
      LNQ   = LP + (NQ-1)*N
      INQ   = LNQ + 1

C
C  SOME MORE INITIALIZATION
C
      OLDNQ = NQ
      OBASE = BASE
      ODELTA=DELTA
      NSTOP = 0
      STOPCNT = 0
      RATIO = TWO 
      RHO   = ZERO
      IMPR  = 3



C
C ===========================================================================
C
C                    MAIN ITERATION
C
C ===========================================================================
C
      DO 200 ITER = 1, MAXIT

         IF ( IPRINT .GE. 2 ) WRITE( IOUT, 8010 ) ITER
C
C  SET THE CURRENT VALUES FOR PIVOT-RELATES THRESHOLDS
C       
         PIVTHR = PIVT*MIN(ONE,DELTA)
         ADDTHR = ADDT*MIN(ONE,DELTA)


C
C  SELECT AN INTERPOLATION POINT WITH THE SMALLEST FUNCTION VALUE AS BASE
C 
         FBASE  = WRK( LV + IWRK( LIN2SP + BASE ))
         DO 10 I = 1, NIND
           IF (( I .LE. N+1-NEQCON .OR. I .GT. N+1 ) .AND.
     .     ( WRK( LV + IWRK( LIN2SP + I )) .LE. FBASE - TEN * MCHEPS))
     +          THEN
             FBASE = WRK(LV + IWRK( LIN2SP + I ))
             BASE  = I
           ENDIF
  10     CONTINUE
         IF ( IPRINT .GE. 2 ) WRITE( IOUT, 8020 ) BASE, NIND

C
C  IF THE BASE POINT HAD CHANGED, WE NEED TO RECOMPUTE THE DISTANCES
C  FROM BASE TO ALL SAMPLE POINTS IN "POINTS" 
C
         IF ( OBASE .NE. BASE ) THEN 
            LBASE = LPI + ( BASE - 1 ) * N
            IBASE = LBASE + 1
            OBASE = BASE
            CALL GETDIS( N, NQ, 0 , WRK( IP ) , IWRK(LIN2SP+BASE), 
     +                   WRK( ID ), WRK(ICURW), LRS )
         ENDIF

C
C  THE MAXIMUM POSSIBLE NUMBER OF INDEPENDENT INTERPOLATION POINTS
C  DEPENDS ON THE ACTUAL DIMENSION OF THE FEASIBLE SET, RATHER THEN
C  ON THE DIMENSION OF THE WHOLE SPACE. SUBROUTINE 'INTDIM' COMPUTES
C  THE DIMENSION OF THE SPACE SPANNED BY EQUALITY CONSTRAINTS AT
C  THE BASE. WE CONSIDER A CONSTRAINT AS  EQUALITY IF THE DIFFERENCE
C  BETWEEN ITS UPPER AND LOWER BOUNDS IS LESS THAN PIVOT THRESHOLD
C
C  NEQCON - NUMBER OF LINEARLY INDEPENDENT EQUALITY CONSTRAINTS
C
         IF ( ODELTA .NE. DELTA ) THEN
           CALL DCOPY ( N, WRK(IBASE), 1, WRK(IK), 1 )
           CALL UNSHFT( N, X, WRK(IK) )
           CALL INTDIM( WRK(IK)   , N   , NCLIN , NCNLN , NEQCON,   
     +                  A, LDA    , LB  , UB    , PIVTHR,   
     +                  WRK(ICURW), LRS , IWRK(ICURIW)  , LIS    )
           ODELTA = DELTA
         ENDIF           

C
C  SET 'OKGEOM' TO FALSE BY DEFAULT
C  SET 'LINEAR' TO TRUE IF THE INTERPOLATION IS FULLY LINEAR
C  (I.E., THERE IS MAXIMUM POSSIBLE NUMBER OF LINEAR NEWTON
C   POLYNOMIALS THAT CAN FIT INTO THE FEASIBLE SET. IN CASE 
C   WHEN THERE ARE NO EQUALITY CONSTRAINTS NIND SHOULD BE AT
C   LEAST N+1, OTHERWISE - AT LEAST N+1 - NUMBER OF EUALITY
C   CONSTRAINTS
C
 
         OKGEOM= .FALSE.
         LINEAR=( NIND .GT. N - NEQCON ) 
C
C  IF THERE ARE EQUALITY CONSTRAINTS, THEN UPON REACHING THE MAXIMUM
C  POSSIBLE NUMBER OF POINTS FOR LINEAR INTERPOLATION WE COMPLETE
C  THE LINEAR INTERPOLATION TO N+1 ELEMENTS WITH DUMMY POLYNOMIALS
C  AND POINTS. THIS IS DONE BECAUSE THE PROGRAM IS WRITTEN UNDER
C  ASSUMPTION THAT QUADRATIC BLOCK IS NOT CONSTRAUCTED UNTILL THE
C  LINEAR BLOCK IS FULL. BY COMPLETETING THE LINEAR BLOCK BY DUMMY
C  ELEMENTS WE MAKE SURE THAT WE CAN MOVE ON TO QUADRATIC ELEMENTS.  
C
         
         IF ( NEQCON .GT. 0 .AND. NIND .EQ. N + 1 - NEQCON ) THEN
           CALL COMPLT( WRK(INP), WRK(IVI), WRK(IPIV), N, NIND, LPOLY )
         ENDIF

C  --------------------------------------
C  PRINT OUT SUMMARY OF CURRENT ITERATION
C  --------------------------------------
C
C  NINDR - NUMBER OF ELEMENTS IN THE INTERPOLATION (WITHOUT THE DUMMY)  
C
         
         IF ( IPRINT .GE. 1 ) THEN
           NINDR = NIND
           IF ( NIND .GT. N + 1 - NEQCON ) NINDR = NIND - NEQCON
C           IF (ITER.EQ.( ITER/10 )*10 +1 ) WRITE( IOUT, 1000 )
           WRITE( IOUT, 1000 )
           WRITE( IOUT, 1010) ITER, NF, FBASE, RHO, DELTA, IMPR, NINDR,
     +                        NEQCON
         ENDIF
C
C  STOPPING TEST: IF THE TRUST REGION RADIUS IS SMALL ENOUGH, WE STOP
C                 IF THE BEST VALUE HAS REACHED THE LOWER BOUND, WE STOP
C
         IF ( IPRINT .GE. 2 ) WRITE( IOUT, 8030 ) DELTA 
         IF ( ( HOWSTOP.EQ.2 .AND. STOPCNT.GE.2)
     +         .OR. (DELTA .LT. DELMIN) ) THEN 
           EXIT = 0
           CALL BESTPT(N, M, X, FX, C, WRK(IP), WRK(IV), WRK(IOB),
     +                 WRK(IC), NQ)
           IT   = ITER
           RETURN               
         ENDIF
C
C  IN DEBUGGING MODE, CHECK THE ACCURACY OF NEWTON FUNDAMENTAL POLYNOMIALS
C
       IF ( IPRINT .GE. 3 ) THEN
         DO 30 I = 2, NIND
           IF( I .LE.  N + 1 - NEQCON) THEN
             II = MIN( N + 1 - NEQCON, NIND )
           ELSE 
             II = NIND
           ENDIF
           DO 20 J = 1, II
            IF (J .LE. N + 1 - NEQCON .OR. J .GT. N + 1 ) THEN
              CALL  EVALNP( VAL, WRK(IPI), J, WRK(INP), I, N,  
     +                      LPTINT , LPOLY )
              IF ( (I.EQ.J .AND. ABS(VAL-ONE) .GT. 1D-10) .OR.
     +             (I.NE.J .AND. ABS(VAL)     .GT. 1D-10) ) THEN
                WRITE(IOUT,1020) I, J, VAL
              ENDIF
            ENDIF
  20       CONTINUE
  30     CONTINUE
       ENDIF

       IF ( METHOD .LE. 2 ) THEN
C  ---------------------------------------------------------------------
C  BUILD THE QUADRATIC MODEL OF THE OBJECTIVE FUNCTION
C  ---------------------------------------------------------------------

         IF ( IPRINT .GE. 2 ) WRITE( IOUT, 8040 ) 
         DO 31 J=1, NIND
           II = IWRK(LIN2SP + J)
           WRK(LVI + J) = WRK( LOB + II ) 
 31      CONTINUE  
         CALL MDBLD( KAPPA       , WRK(IG) , WRK(IH) , N   , NIND      ,
     +               WRK(IPI)    , WRK(IVI), WRK(INP), BASE, VARNT     ,
     +               LPOLY       , LPTINT  , NEQCON  , WRK(ICURW) , LRS,   
     +               IWRK(ICURIW), LIS     )

C  ---------------------------------------------------------------------
C  BUILD THE QUADRATIC MODEL OF THE CONSTRAINTS  
C  ---------------------------------------------------------------------

         DO 33 I = 1, M
           DO 32 J=1, NIND
             II = (IWRK(LIN2SP + J)-1)*M
             WRK(LCI + J) = WRK(LC + II + I) 
 32       CONTINUE  
           CALL MDBLD(WRK(LCC+I), WRK(ICL+(I-1)*N) , WRK(ICQ+(I-1)*N*N), 
     +                N         , NIND             , WRK(IPI)          ,
     +                WRK(ICI)  , WRK(INP)  , BASE , VARNT             ,
     +                LPOLY     , LPTINT           , NEQCON            , 
     +                WRK(ICURW), LRS              , IWRK(ICURIW)      , 
     +                LIS       )

 33     CONTINUE

       ELSE 
C  ---------------------------------------------------------------------
C  BUILD THE QUADRATIC MODEL OF THE MERRIT FUNCTION
C  ---------------------------------------------------------------------

         IF ( IPRINT .GE. 2 ) WRITE( IOUT, 8040 ) 
         DO 34 J=1, NIND
           II = IWRK(LIN2SP + J)
           WRK(LVI + J) = WRK( LV + II ) 
 34     CONTINUE  
         CALL MDBLD( KAPPA       , WRK(IG) , WRK(IH) , N   , NIND      ,
     +               WRK(IPI)    , WRK(IVI), WRK(INP), BASE, VARNT     ,
     +               LPOLY       , LPTINT  , NEQCON  , WRK(ICURW) , LRS,   
     +               IWRK(ICURIW), LIS     )
       ENDIF
C
C  IN DEBUGGING MODE CHECK THE ACCURACY OF QUADRATIC INTERPOLATION
C
       IF ( IPRINT .GE. 3 ) THEN
         DO 41 I=1, M
           DO 40 J = 1, NIND
             IF ( J .LE. N + 1 - NEQCON .OR. J .GT. N + 1 ) THEN
               II = (IWRK(LIN2SP + J)-1)*M
               WRK(LCI + J) = WRK(LC + II + I) 
               VAL = WRK(LCI + J) - (WRK(LCC+I) + 
     +               MVALUE( N, WRK( IPI+(J-1)*N), 
     +               WRK(ICL+(I-1)*N), WRK( ICQ+(I-1)*N*N ), 
     +               WRK( ICURW ), LRS))
               IF ( ABS(VAL) .GT. 1.0D-6 ) WRITE(IOUT,1030)  J, VAL 
             ENDIF
 40        CONTINUE
 41      CONTINUE  
       ENDIF     
       

C  -------------------------------------------------------------------------
C
C  FIND THE NEXT CANDIDATE POINT FOR SAMPLING 
C  BY MINIMIZING THE MODEL OVER TRUST REGION (INTERSECTED WITH FEASIBLE SET)
C
C  -------------------------------------------------------------------------


C
C  SET THE RADIUS TO DELTA
C        
 
       DEL = DELTA

C
C  -------------------  MODIFY THE MODEL  ----------------------------
C  THE MODEL INTERPOLATES A SHIFTED SET OF POINTS, SINCE THE FEASIBLE
C  SET IS NOT SHIFTED, WE MINIMIZE A MODIFIED MODEL, WHICH INTERPOLATES
C  NON-SHIFTED SET OF POINTS OVER NOT-SHIFTED REGION
C  THE PARAMETERS OF THE MODIFIED MODEL ARE STORED IN KMOD, GMOD, HMOD  
C  THE MODIFIED MODEL INTERPOLATES POINTS X_i+X, WHERE X_i ARE INTERP. POINTS,
C  THEN THE MODIFIED MODEL IS COMPUTED AS
C
C                HMOD <-- H
C                GMOD <-- G - H'*X
C                KMOD <-- KAPPA - G'*X - 0.5X*H'*X
C  -------------------------------------------------------------------
C


       KMOD=KAPPA
       DO 51 I=1,N
         GMOD(I)= WRK(IG+I-1)
         KMOD   = KMOD-WRK(IG+I-1)*X(I)
         II=(I-1)*N
         DO 50 J=1,N
           HMOD(I,J)= WRK(IH+II+J-1)
           GMOD(I)  = GMOD(I)-HMOD(I,J)*X(J)
           KMOD     = KMOD+HALF*HMOD(I,J)*X(J)*X(I)
 50      CONTINUE
 51      CONTINUE   

       
       IF ( METHOD .LE. 2 ) THEN
         DO 60 K=1, M
           CCON(K)=WRK(LCC+K)
           KK=(K-1)*N
           DO 56 I=1,N
             LCON(KK+I)= WRK(LCL+KK+I)
             CCON(K)   = CCON(K)-WRK(LCL+KK+I)*X(I)
             II=(I-1)*N
             DO 55 J=1,N
               QCON(KK*N+II+J)= WRK(LCQ+KK*N+II+J)
               LCON(KK+I)  = LCON(KK+I)-QCON(KK*N+II+J)*X(J)
               CCON(K)     = CCON(K)+HALF*QCON(KK*N+II+J)*X(J)*X(I)
 55          CONTINUE
 56        CONTINUE   
 60      CONTINUE 
       ENDIF 
C
C  PRINT OUT MODEL COEFFICIENT IF DEMANDED
C
       IF ( IPRINT .GE. 3 ) THEN
         WRITE( IOUT, 8050 )
         WRITE( IOUT, 8051 )
         DO 62  I = 1, N
           WRITE(IOUT, 8052)
           DO 61 J = 1, N
             WRITE(IOUT, 8053) HMOD(I,J)
 61        CONTINUE
 62      CONTINUE
         WRITE( IOUT, 8054 )
         DO 63 J = 1, N
           WRITE(IOUT, 8055) GMOD(J)
 63      CONTINUE
         WRITE( IOUT, 8056 ) KMOD
         WRITE( IOUT, 8060 ) 
       ENDIF
C
C  SET STARTING POINT FOR MINIMIZATION TO THE SHIFTED BASE POINT
C  AND SHIFT IT BACK TO ITS ORIGINAL POSITION
C  ( WE HAVE TO DO IT SINCE THE FEASIBLE SET IS NOT SHIFTED )
C

 70    CALL DCOPY ( N, WRK(IBASE), 1, WRK(IK), 1 )
       CALL UNSHFT( N, X, WRK(IK) )



C
C  MINIMIZE THE MODIFIED MODEL 
C

       IF ( IPRINT .GE. 2 ) WRITE( IOUT, 8065 ) 
       IF ( METHOD .LE. 2 ) THEN
         CALL MINTR (N         , WRK( IK ), MVAL        , DEL  , LB, 
     +               UB        , A        , LDA         , NCLIN,M+NCNLN,
     +               WRK(ICURW), LRS      , IWRK(ICURIW), LIS  , INFORM,
     +               1    )
       ELSE 
         CALL MINTR (N         , WRK( IK ), MVAL        , DEL  , LB, 
     +               UB        , A        , LDA         , NCLIN, NCNLN ,
     +               WRK(ICURW), LRS      , IWRK(ICURIW), LIS  , INFORM,
     +               METHOD    )
       ENDIF
C
C  WARNING!! BE CAREFULL, DON'T USE WRK(ICURW) INTIL FMERIT IS CALLED
C  THE CURRENT VALUES OF THE CONSTRAINED ARE STORED THERE
C

C
C  IF NO FEASIBLE POINT CAN BE FOUND FOR THE MODELS OF  CONSTRAINTS
C  THEN RUN MINTR AGAIN MINIMIZING UNCONSTRAINED MERIT FUNCTION
C

       IF ( METHOD .EQ. 2 .AND. INFORM.EQ. 3 ) THEN 
         USEMER=.TRUE.
         CALL MINTR (N         , WRK( IK ), MVAL        , DEL  , LB, 
     +               UB        , A        , LDA         , NCLIN, NCNLN ,
     +               WRK(ICURW), LRS      , IWRK(ICURIW), LIS  , INFORM,
     +               2         )
       ELSE
         USEMER=.FALSE.
         IF ( INFORM .EQ. 3 ) INFORM = 0
       ENDIF
C
C  IF NO FEASIBLE POINT FOUND, THEN CONSIDER REDUCTION TO BE ZERO
C

       IF (INFORM .EQ. 2 ) THEN
         VAL = FBASE
         GOTO 81
       ENDIF 

C
C  IF THE MINIMIZATION FAILED, THEN RECORD THE BEST POINT AND EXIT
C
       IF (INFORM.NE.0) THEN
         CALL BESTPT(N, M, X, FX, C, WRK(IP), WRK(IV), WRK(IOB),
     +               WRK(IC), NQ)
         IT    = ITER
         EXIT=-7
         RETURN
       ENDIF
C
C  COMPUTE THE PREDICTED VALUE OF THE MERIT FUNCTION
C
       IF ( METHOD .EQ. 1 .OR. (METHOD .EQ. 2 .AND. .NOT. USEMER) ) THEN
         CALL FMERIT(M, VAL, MVAL+KMOD, WRK(ICURW), 
     .               UB(N+NCLIN+NCNLN+1), LB(N+NCLIN+NCNLN+1), PP, 
     .               METHOD )
       ELSE
         VAL=MVAL+KMOD
       ENDIF
C
C  SHIFT THE NEW POINTS
C 
       CALL SHIFT ( N, X, WRK(IK))

C
C  COMPUTE THE DISTANCE FROM THE NEW POINT TO THE BASE
C
       DO 80 I=1,N
         WRK(LCURW+I)=WRK(LK+I)-WRK(LBASE+I)
 80    CONTINUE

       SNORM  = DNRMNF( N, WRK( ICURW ) )

C
C  COMPUTE PREDICTED REDUCTION
C
 81    PRERED = FBASE - VAL

C
C  IF THE MODEL REDUCTION IS SMALL, THEN WE DO NOT SAMPLE FUNCTION
C  AT THE NEW POINT. WE THEN WILL TRY TO IMPROVE THE MODEL.
C
       NOISEF = HALF * MAX( ANOISE, ABS( FBASE ) ) * RNOISE

       IF ( PRERED .LT. MAX( NOISEF,DELMIN*1.0D-2) .OR.
     +      PRERED .LT. DELMIN*ABS(FBASE) .AND. SNORM .LT. DELMIN .OR.
     +      PRERED .LT. CNSTOL*ABS(FBASE)*1.0D-3 ) THEN
         IF ( IPRINT .GE. 2 ) WRITE( IOUT, 8067 ) PRERED
         RHO   = -THOUS
C
C  SET INDICATORS THAT NO NEW POINT WAS ADDED TO INTERPOLATION SET
C  AND NO OTHER POINT WAS COMPUTED TO IMPROVE GEOMETRY 
C
         IFADD = 0
         IFXG  = 0

C
C  IF THE MODEL REDUCTION IS SUFFICIENT, THEN SAMPLE THE FUNCTION
C  AT THE NEW POINT
C
       ELSE
C
C  RECORD THE NEW POINT INTO THE SAMPLE SET
C
         CALL DCOPY(N, WRK(IK),1,WRK(INQ+N),1)
C
C  IF THE NUMBER OF FUNCTION EVALUATIONS EXCEEDED THE LIMIT, RECORD
C  THE BEST POINT AND EXIT
C
         NF = NF + 1
         IF ( NF .GT. MAXNF ) THEN
           CALL BESTPT(N, M, X, FX, C, WRK(IP), WRK(IV), WRK(IOB),
     +                 WRK(IC), NQ)
           IT   = ITER
           EXIT = 1
           RETURN
         ENDIF

C
C  COMPUTE THE FUNCTION VALUE AT THE NEW POINT
C
         CALL UNSHFT(N, X, WRK(IK))
         IF ( SCALE .NE. 0 ) CALL UNSCL( N, WRK(IK), SCAL )
         CALL FUN( N, M, WRK( IK ), WRK(IOB+NQ), WRK(IC + NQ*M), IFERR )
         IF ( SCALE .NE. 0 ) CALL SCL( N, WRK(IK), SCAL )
C
C  IF FUNCTION COMPUTATION FAILS, THEN REDUCE THE RADIUS FOR 
C  MINIMIZATION TO HALF OF THE DISTANCE BETWEEN THE NEW POINT 
C  AND THE BASE. REPEAT THE MINIMIZATION
C
         IF (IFERR) THEN
           DEL  = SNORM/2
           IF ( DEL .GE. DELMIN ) THEN 
             GOTO 70
           ELSE
             RHO   = -THOUS
             IFADD = 0
             IFXG  = 0
             GOTO 170
           ENDIF
         ENDIF
C
C  IF FUNCTION VALUE IS COMPUTED, THEN RECORD THE VALUE AND DO
C  APPROPRIATE BOOKKEEPING AND UPDATES
C
         CALL FMERIT(M, FNQ, WRK(IOB+NQ), WRK(IC + NQ*M), 
     .               UB(N+NCLIN+NCNLN+1), LB(N+NCLIN+NCNLN+1), PP,
     +               METHOD )
         NQ     = NQ + 1
         LNQ    = LP + ( NQ - 1 ) * N
         INQ    = LNQ + 1
         LPNTS  = LPNTS+N
         LVALUE = LVALUE+1
         LCONVL = LCONVL+M
         IMPR   = 10
         WRK ( LV + NQ ) = FNQ
         NEXT            = NQ
         WRK ( LD + NQ ) = SNORM
     
C  -----------------------------------------------------------
C  COMPUTE RHO = ACHIEVED REDUCTION VS. PREDICTED REDUCTION
C  -----------------------------------------------------------

         RHO = ( FBASE - FNQ ) / PRERED
         IF ( IPRINT .GE. 2 ) WRITE( IOUT, 8080 ) RHO

C  ------------------------------------------------------------
C  COMPUTE RELATIVE ACHIEVED REDUCTION AND CHECK IF IT IS SMALL
C  ------------------------------------------------------------
         VAL = ( FBASE - FNQ ) /(ONE+ABS(FBASE))
         IF ( ( VAL .GT. 0 ).AND. ( VAL .LT. WHENSTOP).AND.
     +        ( RHO .GT. RHOMIN )) THEN
           STOPCNT=STOPCNT+1
         ELSE
           STOPCNT=0
         ENDIF        

C  -----------------------------------------------------------
C  IF MODEL AGREEMENT IS VERY GOOD, ATTEMPT A JUMP
C  -----------------------------------------------------------

C
C  A JUMP IS DONE BY MINIMIZING THE SAME MODEL OVER A LARGER
C  TRUST REGION. IT IS ONLY DONE IF THE MINIMIZATION WITH RADIUS
C  DELTA LANDED ON THE TRUST REGION BOUND
C

         IF ( RHO .GE. RHOJMP  .AND. RHO .LE. TWO - RHOJMP 
     +            .AND. ABS(SNORM-DELTA).LT.CNSTOL ) THEN
           IF ( IPRINT .GE. 2 ) WRITE( IOUT, 8085 )
           THETA = ZERO
           DO 90 I = 1, NQ - 1
C
C  FOR ALL SAMPLE POINTS (EXCEPT LAST) WHICH ARE NOT IN INTERPOLATION
C  AND ARE FURTHER THAT DELTA AWAY  FROM THE BASE COMPUTE THE AGREEMENT
C  BETWEEN THE MODEL AND FUNCTION
C
             IF ( IWRK( LSP2IN+ I ) .EQ. 0   .AND. 
     +            WRK(  LD + I ) .GE. DELTA ) THEN
               IBEG = LP + (I-1) * N 
C
C  COMPUTE THE PREDICTED VALUE OF THE MERIT FUNCTION
C
               MVAL= MVALUE( N, WRK( IBEG+1 ), WRK( IG ),
     +                       WRK( IH ), WRK( ICURW), LRS )
               IF ( METHOD .LE. 2  ) THEN
                 CALL FMERIT(M, VAL, MVAL+KAPPA, WRK(ICI+(I-1)*M), 
     +                       UB(N+NCLIN+NCNLN+1), LB(N+NCLIN+NCNLN+1),
     +                       PP, METHOD )
               ELSE
                 VAL = MVAL + KAPPA
               ENDIF
               VAL=VAL-FBASE
               IF ( ABS(VAL).GT.MCHEPS ) THEN
                 RHOW = ( WRK( LV + I ) - FBASE ) / VAL
C
C  IF FOR A GIVEN POINT THE AGREEMENT IF GOOD, THEN WE TRY TO JUMP 
C  AT LEAST AS FAR AS THIS POINT FROM THE BASE
C
                 IF ( RHOW .GE. ONE - RHOMIN .AND.
     +                RHOW .LE. ONE + RHOMIN      ) THEN
                   THETA = MAX( THETA, WRK( LD + I ) )
                 ENDIF 
               ENDIF                  
             ENDIF
  90       CONTINUE
 
C
C  MAKE SURE THAT THE SIZE OF THE JUMP DOES NOT EXCEED THE LIMIT
C          
           THETA = MIN( THETA, MAXJMP * DELTA )
           IF ( IPRINT .GE. 2 ) WRITE( IOUT, 8090 ) THETA
             
           DEL = THETA

C
C  IF THE POSSIBLE SIZE OF THE JUMP BIG ENOUGH (SO IT COVERS AREA LARGER
C  THAN THE AREA THAT WOULD BE COVERED BY THE NEXT ITERATION) THEN
C  COMPUTE THE JUMP
C
 100       IF ( DEL .GT. SNORM + RATIO * DELTA) THEN

C
C  SET INITIAL POINT TO THE BASE POINT AND SHIFT IT TO ORIGINAL POSITION
C
             CALL DCOPY(N, WRK(IBASE), 1, WRK( IK), 1)        
             CALL UNSHFT(N, X, WRK(IK) )
C
C  CALL MINIMIZATION
C
             IF ( METHOD .LE. 2 ) THEN 
               CALL MINTR( N           , WRK(IK), MVAL      , DEL, 
     +                     LB          , UB     , A         , LDA,
     +                     NCLIN       , M      , WRK(ICURW), LRS, 
     +                     IWRK(ICURIW), LIS    , INFORM    , 1  )   
             ELSE 
               CALL MINTR( N           , WRK(IK), MVAL      , DEL, 
     +                     LB          , UB     , A         , LDA,
     +                     NCLIN       , NCNLN  , WRK(ICURW), LRS, 
     +                     IWRK(ICURIW), LIS    , INFORM    , METHOD ) 
             ENDIF
             CALL SHIFT(N, X, WRK(IK) )
             IF ( INFORM .EQ. 3 ) INFORM=0
             IF ( INFORM .EQ. 2 ) GOTO 115
C
C  COMPUTE THE PREDICTED VALUE OF THE MERIT FUNTION
C
             IF ( METHOD .LE. 2 ) THEN
               CALL FMERIT(M, VAL, MVAL+KMOD, WRK(ICURW),
     +          UB(N+NCLIN+NCNLN+1), LB(N+NCLIN+NCNLN+1), PP,
     +          METHOD )
             ELSE
               VAL=MVAL+KMOD
             ENDIF
C
C  IF MINIMIZATION HAD FAILED, THEN RECORD THE BEST POINT AND EXIT
C
             IF (INFORM.NE.0) THEN
               CALL BESTPT(N, M, X, FX, C, WRK(IP), WRK(IV), WRK(IOB),
     +                 WRK(IC), NQ)
               IT   = ITER
               EXIT = -7
               RETURN
             ENDIF

C
C  COMPUTE THE DISTANCE FROM THE NEW POINT TO THE BASE
C
             DO 110 I=1,N
               WRK(LCURW+I)=WRK(LK+I)-WRK(LBASE+I)
 110         CONTINUE
             STNORM  = DNRMNF( N, WRK( ICURW ) )
             IF ( IPRINT .GE. 2 ) WRITE( IOUT, 8092 ) STNORM
C
C  IF THE ACTUAL JUMP IS LARGE ENOUGH THEN SAMPLE THE FUNCTION VALUE
C  AT THE NEW POINT
C
             IF ( STNORM .GT.  SNORM + RATIO * DELTA ) THEN
               CALL DCOPY(N, WRK(IK), 1, WRK( INQ + N ), 1)
               NF = NF + 1
C
C  IF THE NUMBER OF FUNCTION CALLS EXCEEDS MAXIMUM, THEN RECORD THE
C  BEST POINT AND EXIT
C
               IF ( NF .GE. MAXNF ) THEN
                 CALL BESTPT(N, M, X, FX, C, WRK(IP), WRK(IV), WRK(IOB),
     +                       WRK(IC), NQ)
                 IT   = ITER
                 EXIT = 1
                 RETURN
               ENDIF

C
C  COMPUTE THE FUNCTION VALUE 
C
               CALL UNSHFT(N, X, WRK(IK))
               IF ( SCALE .NE. 0 ) CALL UNSCL( N, WRK(IK), SCAL )
               CALL FUN( N, M, WRK( IK ), WRK(IOB+NQ), WRK(IC + NQ*M), 
     +                   IFERR )
               IF ( SCALE .NE. 0 ) CALL SCL( N, WRK(IK), SCAL )
C
C  IF THE FUNCTION COMPUTATION FAILS THEN REDUCE THE SIZE OF THE JUMP
C  AND TRY TO COMPUTE A NEW JUMP
C
               IF ( IFERR ) THEN 
                 DEL=STNORM/2
                 IF ( DEL .GE. DELTA ) THEN 
                   GOTO 100
                 ELSE
                   GOTO 115
                 ENDIF
               ENDIF
 
C
C  IF THE FUNCTION VALUE IS COMPUTED THEN RECORD IT AND DO APPROPRIATE
C  BOOKKEEPING AND UPDATES
C
               IF ( IPRINT .GE. 2 ) WRITE( IOUT, 8095 )
               CALL FMERIT(M, FT, WRK(IOB+NQ), WRK(IC + NQ*M), 
     +              UB(N+NCLIN+NCNLN+1), LB(N+NCLIN+NCNLN+1), 
     +              PP, METHOD )
               NQ     = NQ + 1
               LNQ    = LP + ( NQ - 1 ) * N
               INQ    = LNQ + 1
               LPNTS  = LPNTS + N
               LVALUE = LVALUE + 1
               LCONVL = LCONVL + M
               WRK( LV + NQ )     = FT
               IWRK( LSP2IN+ NQ ) = 0
               WRK ( LD + NQ )    = STNORM

C
C  IF THE NEW POINT IS BETTER THAN HE PREVIOUS ONE
C  THE TAKE THE NEW POINTS AND NEXT TO BE INCLUDED IN
C  HE INTERPOLATION SET
C
                  
               IF ( FT .LT.  FNQ ) THEN
               
C
C  COMPUTE 'RHO' - ACHIEVED REDUCTION VS. PREDICTED REDUCTION
C
                 RHO   = ( FT - FBASE ) / (VAL - FBASE)
                 NEXT  = NQ
                 SNORM = STNORM
                 IMPR  = 20
                 IF ( IPRINT .GE. 2 ) WRITE( IOUT, 8097 ) RHO
               ENDIF
             ENDIF
           ENDIF
         ENDIF
C  ------------------------------------------------------------------
C         END OF JUMPING ATTEMPT
C  ------------------------------------------------------------------
        
C  ------------------------------------------------------------------
C
C  UPDATE THE INTERPOLATION SET
C
C  ------------------------------------------------------------------

C
C  SET FLAGS TO INDICATE THAT NOTHING HAS BEEN DONE TO THE MODEL YET
C     
 115     IFADD = 0
         IFXG  = 0
         INEXT =(NEXT-1)*N

C
C  IF THE IMPROVEMENT IN FUNCTION VALUE IS SUFFICIENT WE INCLUDE
C  THE NEXT POINT IN THE MODEL
C

         IF ( RHO .GT. RHOMIN ) THEN
           IF ( IPRINT .GE. 2 ) WRITE( IOUT, 8100 ) 
C
C  IF THE MODEL IS INCOMPLETE, THEN WE ADD THE NEXT POINT TO THE
C  MODEL ( IF EFFORT >= 4, THEN IT WILL BE DONE LATER )
C
           IF ( NIND .LT. DD .AND. EFFORT .LT. 4 ) THEN
C
C  DETERMINE THE INDEX OF LAST POLYNOMIAL IN THE CURRENT BLOCK
C
             IF ( NIND . LE. N ) THEN
               IPEND = N + 1
             ELSE
               IPEND = DD
             ENDIF
C
C  TRY TO FIND A POLYNOMIAL WHICH PRODUCES ACCEPTABLE PIVOT 
C  TO INCLUDE THE NEW POINT IN INTERPOLATION.
C  WE MAY NEED TO CHECK ALL 'AVAILABLE' POLYNOMIALS IN CURRENT BLOCK
C 
             DO 120 IPOLY = NIND+1, IPEND
C
C  UPDATE THE NEXT NEWTON POLYNOMIAL, SO THAT IT IS 'ORTHOGONAL'
C  TO THE CURRENT INTERPOLATION SET
C

               CALL NEXTNP( IPOLY, WRK(INP), WRK(IPI), NIND, N, 
     +                      LPOLY, LPTINT )
C
C  EVALUATE NEXT POLYNOMIAL AT THE NEW POINT
C

               CALL EVALX( VAL, WRK(IP+INEXT), WRK(INP), IPOLY,
     +                     N  , LPOLY )
C
C  IF THE VALUE (THE PIVOT) IS ACCEPTABLE, ACCEPT CURRENT POLYNOMIAL
C  AS THE NEXT AND DO NOT CHECK OTHER POLYNOMIALS.
C
                
               IF (ABS(VAL) .GT. PIVTHR ) GO TO 130

 120         CONTINUE
             GO TO 140
C
C  PLACE THE IPOLY-TH POLYNOMIAL IN THE PLACE OF NIND+1-ST
C  POLYNOMIAL, BY SWAPPING THEM IN ARRAY POLY
C
 130         IF ( IPOLY .NE. NIND+1 ) THEN
               CALL SWAPNP( N, NIND+1, IPOLY, WRK(INP), LPOLY )
             ENDIF
C
C  PERFORM THE APPROPRIATE UPDATES TO THE SET OF THE FIRST NIND
C  NEWTON POLYNOMIALS, WHICH ARE REQUIRED TO INCLUDE THE NEW
C  POINT-POLYNOMIAL PAIR IN THE INTERPOLATION
C
             CALL PTREPL( WRK(IP+INEXT), NIND+1, VAL, WRK(IPI), 
     +                    WRK(INP)     , NIND  , N  , LPOLY   , LPTINT )
C
C  RECORD THE NEW POINT IN THE INTERPOLATION SET
C
             CALL DCOPY(N, WRK(IP+INEXT), 1, WRK(IPI+NIND*N), 1)
             NIND              = NIND+1
             WRK(LVI+NIND)     = WRK(LV+NEXT)
             IWRK(LSP2IN+NEXT) = 1
             IWRK(LIN2SP+NIND) = NEXT
             WRK(LPIV+NIND)    = VAL
             IFADD             = 1
C
C  IF EFFORT = 4 THEN WE CHANGE THE BASE TO THE NEW BEST POINT
C  ( OTHERWISE THE BASE WILL BE CHANGED LATER )
C
             IF ( EFFORT. EQ. 4 ) THEN
               BASE              = NIND 
               LBASE = LPI + ( BASE - 1 ) * N
               IBASE = LBASE + 1
               OBASE = BASE
               CALL GETDIS( N, NQ, 0 , WRK( IP ) , IWRK(LIN2SP+BASE), 
     +                      WRK( ID ), WRK(ICURW), LRS )
             ENDIF
             IF ( IPRINT .GE. 2 ) WRITE( IOUT, 8110 ) VAL
           ENDIF


 140       OLDNQ = NQ          

C
C  IF THE MODEL IS FULL OR THE PIVOT VALUE FOR ADDING THE POINT
C  IS TOO SMALL, THEN TRY TO INCLUDE THE NEW POINT BY REPLACING
C  ANOTHER POINT BY IT. IF SUCCEED, UPDATE THE NEWTON POLYNOMIALS
C  ( IF EFFORT >= 4, THEN IT WILL BE DONE LATER )
C

           IF ( IFADD.EQ.0 ) THEN
C
C  IF THE PIVOT IS TOO SMALL AND THE MODEL IS NOT FULLY LINEAR
C  SET A FLAG TO MAKE SURE THAT WE TRY TO ADD A 'GEOMETRY' POINT
C  LATER IN 'IMPMOD' PROCEDURE
C
             IF (.NOT. LINEAR) IFXG=1
             IPOLY=BASE
             CALL PTEXCH( WRK(INP), WRK(IP+INEXT), WRK(IPI), IPOLY , 
     +                    NIND    , N            , LPOLY   , LPTINT,  
     +                    PIVTHR  , WRK(IPIV)    , VAL     , FAIL  )

C
C  IF WE FIND A POINT TO BE REPLACED BY THE NEW POINT, THEN
C  RECORD THE EXCHANGE IN INTERPOLATION SET
C
             IF (.NOT.FAIL) THEN
               CALL DCOPY(N, WRK(IP+INEXT), 1 , 
     +                    WRK(IPI+(IPOLY-1)*N), 1)
               WRK(LVI+IPOLY)     = WRK(LV+NEXT)
               WRK(LPIV+IPOLY)    = VAL*WRK(LPIV+IPOLY)
               IWRK(LIN2SP+IPOLY) = NEXT
               IWRK(LSP2IN+NEXT)  = 1
               IWRK(LSP2IN+IWRK(LIN2SP+IPOLY))=0
               IFADD              = 1
C
C  IF EFFORT = 4 THEN WE CHANGE THE BASE TO THE NEW BEST POINT
C  ( OTHERWISE THE BASE WILL BE CHANGED LATER )
C
               IF ( EFFORT. EQ. 4 ) THEN
                 BASE              = IPOLY 
                 LBASE = LPI + ( BASE - 1 ) * N
                 IBASE = LBASE + 1
                 OBASE = BASE
                 CALL GETDIS( N, NQ, 0 , WRK( IP ) , IWRK(LIN2SP+BASE), 
     +                        WRK( ID ), WRK(ICURW), LRS )
               ENDIF
               IF ( IPRINT .GE. 2 ) WRITE( IOUT, 8120 ) IPOLY,  VAL
             ENDIF
           ENDIF
C
C  IF THE MODEL REDUCTION IS NOT SUFFICIENTLY GOOD, WE STILL
C  TRY TO ADD THE NEW POINT (SINCE IT CONTAINS NEW INFORMATION)
C

         ELSE
           IF ( IPRINT .GE. 2 ) WRITE( IOUT, 8105 )
C
C  IF THE MODEL IS INCOMPLETE TRY ADDING THE NEW POINT
C  IF THE EFFORT >= 3 TRY TO DO IT LATER
C

           IF ( NIND.LT.DD .AND. EFFORT .LT. 3 ) THEN
C
C  DETERMINE THE INDEX OF LAST POLYNOMIAL IN THE CURRENT BLOCK
C
             IF ( NIND . LE. N ) THEN
               IPEND = N + 1
             ELSE
               IPEND = DD
             ENDIF
C
C  TRY TO FIND A POLYNOMIAL WHICH PRODUCES ACCEPTABLE PIVOT 
C  TO INCLUDE THE NEW POINT IN INTERPOLATION.
C  WE MAY NEED TO CHECK ALL 'AVAILABLE' POLYNOMIALS IN CURRENT BLOCK
C 
             DO 150 IPOLY = NIND+1, IPEND
C
C  UPDATE THE NEXT NEWTON POLYNOMIAL, SO THAT IT IS 'ORTHOGONAL'
C  TO THE CURRENT INTERPOLATION SET
C

               CALL NEXTNP( IPOLY, WRK(INP), WRK(IPI), NIND, N, 
     +                      LPOLY, LPTINT )
C
C  EVALUATE NEXT POLYNOMIAL AT THE NEW POINT
C

               CALL EVALX( VAL, WRK(IP+INEXT), WRK(INP), IPOLY,
     +                     N  , LPOLY )
C
C  IF THE VALUE (THE PIVOT) IS ABOVE A CERTAIN THRESHOLD
C  (WHICH MAY BE SET HIGHER THAN PIVOT THRESHOLD, TO IMPOSE
C  STRICTER GEOMETRY REQUIREMENTS ON  A POINT WHICH GIVES POOR
C  REDUCTION)  THEN ADD THE POINT TO THE INTERPOLATION SET,
C  UPDATING OTHER NEWTON POLYNOMIALS
C                
               IF (ABS(VAL) .GT. ADDTHR ) GO TO 160

 150         CONTINUE
C
C  IF THERE IS NO POLYNOMIAL WHICH GIVES ACCEPTABLE PIVOT, THEN
C  MOVE ON TO MODEL IMPROVEMENT
 
             GO TO 170
C
C  PLACE THE IPOLY-TH POLYNOMIAL IN THE PLACE OF NIND+1-ST
C  POLYNOMIAL, BY SWAPPING THEM IN ARRAY POLY
C
 160         IF (IPOLY .NE. NIND+1) THEN
               CALL SWAPNP(N, NIND+1, IPOLY, WRK(INP), LPOLY)
             ENDIF


C
C  ADD THE POINT TO THE INTERPOLATION SET,
C  UPDATING OTHER NEWTON POLYNOMIALS
C


             CALL PTREPL( WRK(IP+INEXT), NIND+1, VAL, WRK(IPI), 
     +                    WRK(INP)     , NIND  , N  , LPOLY   , LPTINT)

C
C  DO APPROPRIATE UPDATES
C
             CALL DCOPY(N, WRK(IP+INEXT), 1, WRK(IPI+NIND*N), 1)
             NIND              = NIND+1
             WRK(LVI+NIND)     = WRK(LV+NEXT)
             IWRK(LSP2IN+NEXT) = 1
             IWRK(LIN2SP+NIND) = NEXT
             WRK(LPIV+NIND)    = VAL
             IFADD             = 1
             IF ( IPRINT .GE. 2 ) WRITE( IOUT, 8110 ) VAL
           ENDIF
       
C
C  END OF ATTEMPT OF INCLUDING A NEW POINT
C
         ENDIF
       ENDIF
 170   CONTINUE

       IF ( IPRINT .GE. 3 ) THEN
         WRITE( IOUT, 8170 )
         DO 175 I = 1, NIND
           WRITE( IOUT, 8055 ) WRK(IPIV + I - 1)
 175     CONTINUE
       ENDIF 
C
C  IF THE IMPROVEMENT OF THE MODEL IS REQUIRED, SET IMPR TO 0
C 
       IF ( (IFADD .EQ. 0) .OR. (IFXG .EQ. 1) ) IMPR = 0
C
C  IF THE NEW POINT HASN'T BEEN ADDED, BUT IT GIVES REDUCTION, THEN
C  SET A VALUE OF IMPR, SO THAT THIS POINT IS SET TO BASE IN 'IMPMOD' 
C 
       IF ( IFADD.EQ.0 .AND. RHO.GT. RHOMIN ) IMPR=-NEXT
C
C  TRY TO IMPROVE THE MODEL (BY FINDING ANOTHER POINT OR  RECOMPUTING
C  THE BASIS OF NEWTON POLYNOMIALS AND THE INTERPOLATION SET IF:
C      1. NOTHING WAS ADDED TO THE POINT
C      2. IF A NEW 'GEOMETRY' POINT IS REQUIRED 
C      3. THE MODEL IS TOO OLD, THAT IS THE CURRENT BASE IS LAYER*DELTA
C         AWAY FROM THE FIRST INTERPOLATION POINT
C      4. IF THE EFFORT LEVEL REQUIRES INTERPOLATION TO BE RECOMPUTED      
C
       IF (  IMPR .LE. 0                            .OR. 
     +      (WRK(LD+IWRK(IIN2SP)) .GT. LAYER*DELTA) .OR.
     +      (EFFORT .EQ. 3 .AND. RHO .LE. RHOMIN)   .OR.
     +       EFFORT .EQ. 4 ) THEN

C
C  CHECK IF THE NUMBER IF FUNCTION CALLS REACHED MAXIMUM
C  IF IT DOES RECORD THE BEST POINT AND EXIT
C
         IF ( NF .GE. MAXNF ) THEN
           CALL BESTPT(N, M, X, FX, C, WRK(IP), WRK(IV), WRK(IOB),
     +                 WRK(IC), NQ)
           IT   = ITER
           EXIT = 1
           RETURN
         ENDIF
C
C  COMPLETE THE INTERPOLATION WITH DUMMY ELEMENTS IF THE LINEAR
C  BLOCK REACHES THE MAXIMUM POSSIBLE SIZE (MORE DETAILS ABOVE)
C

         IF ( NEQCON .GT. 0 .AND. NIND .EQ. N + 1 - NEQCON ) THEN
           CALL COMPLT( WRK(INP), WRK(IVI), WRK(IPIV), N, NIND, LPOLY )
         ENDIF              
         IF ( IPRINT .GE. 2 ) WRITE( IOUT, 8140 )
         CALL IMPMOD( WRK(INP)    , WRK(IPI)    , WRK(IVI)    , WRK(IP), 
     +                WRK(IV)     , WRK(IOB)    , WRK(IC)     , 
     +                IWRK(ISP2IN), IWRK(IIN2SP), N, M   ,
     +                NQ          , NIND        , BASE        , PIVTHR , 
     +                XCHTHR      , WRK(IPIV)   , WRK(ID)     , DELTA  ,
     +                X           , A           , LDA         , NCLIN  , 
     +                NCNLN       , LB          , UB          , SCALE  ,
     +                SCAL        , NF          , MAXNF       , IMPR   ,
     +                PP          , NEQCON      ,
     +                WRK(ICURW+1), LRS         , IWRK(ICURIW), LIS    ,
     +                DELMIN      , METHOD      )

C
C  CHECK IF THE NUMBER IF FUNCTION CALLS EXCEEDS MAXIMUM
C  IF IT DOES RECORD THE BEST POINT AND EXIT
C
         IF ( NF .GE. MAXNF ) THEN
           CALL BESTPT(N, M, X, FX, C, WRK(IP), WRK(IV), WRK(IOB),
     +                 WRK(IC), NQ)
           IT   = ITER
           IT   = ITER
           EXIT = 1
           RETURN
         ENDIF


C
C  IF THE BASIS CONSISTS ONLY OF A SINGLE POLYNOMIAL (CONSTANT TERM)
C  THEN SOMETHING IS WRONG. WE RETURN AND PRINT A MESSAGE      
C

         IF (NIND.LT.2) THEN
           IF ( IPRINT .GE. 0 ) WRITE(IOUT, 2060)
           CALL BESTPT(N, M, X, FX, C, WRK(IP), WRK(IV), WRK(IOB),
     +                 WRK(IC), NQ)
           IT    = ITER
           EXIT  = -9
           RETURN
         ENDIF
C
C  IF THE NUMBER OF SAMPLE POINTS INCREASED AFTER APPLYING IMPMOD
C  WE UPDATE THE POINTERS TO THE LAST POINT ACCORDINGLY
C

         IF ( OLDNQ .NE. NQ ) THEN
           LNQ   = LP  + (NQ-1)*N
           INQ   = LNQ + 1
           OLDNQ = NQ
         ENDIF
       ENDIF

C
C  IF THE MODEL WAS CHANGED EITHER BY ADDING A NEW "GOOD" POINT
C  OR BY THROWING AWAY OLD POINTS OR IF THE MODEL WAS NOT LINEAR
C  WE KEEP 'OKGEOM' EQUAL TO FALSE TO INDICATE THAT REDUCING TRUST 
C  REGION IS NOT NECESSARY, SINCE THE MODEL HAS IMPROVED. OTHERWISE
C  SET 'OKGEOM' TO TRUE, SINCE THE MODEL WAS OK AND TR REDUCTION 
C  MAY BE NEEDED. IMPR=0 MEANS THAT THE MODEL HAS NOT IMPROVED EVEN THOUGH
C  THE MODEL WAS NOT FULLY LINEAR. THIS CAN HAPPEN IN "DEGENERATE"
C  CONSTRAINED  CASES
C
       IF ((( IMPR .GE. 4  .OR. RHO .EQ. - THOUS) .AND. LINEAR) 
     +       .OR. IMPR .EQ. 0 ) OKGEOM=.TRUE.


C  --------------------------------------------------------------
C
C  UPDATING TRUST REGION RADIUS
C
C  --------------------------------------------------------------


C
C  IF THE REDUCTION IF GOOD, INCREASE DELTA ACCORDING TO 
C  THE STEP TAKEN BY TRUST REGION  MINIMIZATION  
C
       IF ( RHO .GE. RHOINC .OR. IMPR .EQ. 20 ) THEN
         IF ( IPRINT .GE. 2 ) WRITE( IOUT, 8130 )
         DELTA = MAX( DELTA, MIN( DELMAX, RATIO*SNORM ))

C
C  IF THE REDUCTION WAS BAD, AND THE MODEL WAS ALREADY GOOD ENOUGH
C  THEN REDUCE DELTA
C
       ELSEIF ( RHO .LT. RHOMIN .AND. OKGEOM ) THEN
         IF ( IPRINT .GE. 2 ) WRITE( IOUT, 8150 )
         DELTHR =  RATIO  * DELMIN
         DELTA = MAX( DELTA / RATIO,  DELTHR )
       ENDIF

C
C  IF DELTA IS JUST ABOVE THE MINIMUM THRESHOLD, MAKE SURE THAT IT
C  DOES NOT GET DECREASED UNTIL WE ARE SURE THAT WE CANNOT GET
C  FURTHER IMPROVEMENT
C

       IF ( DELTA .LE. RATIO * DELMIN + 10*MCHEPS .AND. 
     +      RHO  .LT. RHOMIN  ) THEN
         IF ( IPRINT .GE. 2 ) WRITE( IOUT, 8160 )
         NSTOP = NSTOP + 1
         IF (  NSTOP .GE. 5 .AND. OKGEOM ) THEN
           DELTA = HALF * DELMIN
         ENDIF
       ELSE
         NSTOP = 0
       ENDIF

 200  CONTINUE

      CALL BESTPT( N, M, X, FX, C, WRK(IP), WRK(IV), WRK(IOB),
     +             WRK(IC), NQ )
      IT   = ITER
      EXIT = 2
      RETURN


2000  FORMAT(/, ' DFO: *** ERROR: LWRK IS TOO SMALL!' /
     +         ' DFO:        ADDITIONAL SPACE OF ',I8 ,' REQUIRED')
2020  FORMAT(/, ' DFO: *** ERROR: LIWRK IS TOO SMALL!' /
     +         ' DFO:        ADDITIONAL SPACE OF ',I8 ,' REQUIRED')
2060  FORMAT('DFO: CANNOT FIND TWO POINTS TO BUILD A MODEL!', /
     +        '    THE PIVOT THRESHOLD MAY BE TOO HIGH')
1000  FORMAT('DFO: ', 1X,' it ', 1X, ' nf ', 1X, '    value     ', 1X,
     +        '     rho      ', 1X, '     delta   ', 1X,'impr',
     +        1X, 'nind', 1X, 'neqc'  )
1010  FORMAT( 'DFO:', 1X, I4, 1X, I4, 1X, D14.7, 1X, D14.7, 1X, D14.7, 
     +         1X, I4, 1X, I4, 1X, I4  )
1020  FORMAT('DFO: INEXACT NEWTON POLYNOMIAL', 1X, I4, 1X,'AT POINT:', 
     +              I4,1X, 'ERROR IS:', D12.6)
1030  FORMAT('DFO: INEXACT MODEL AT POINT:', 1X, I4,  1X,'ERROR IS:', 
     +             D12.6)
8000  FORMAT( /, ' DFO: computing initial interpolation set')
8010  FORMAT( /, ' DFO: ************starting iteration ', I4,'******')
8020  FORMAT( /, ' DFO: base point = ',I4,' out of ', I4,/ )
8030  FORMAT( /, ' DFO: convergence test: DELTA = ', D14.7 )
8040  FORMAT( /, ' DFO: building the new model' )
8050  FORMAT( /, ' DFO: **********the model coefficients:********* ' )
8051  FORMAT( /, ' quadratic terms:' )
8052  FORMAT( / )
8053  FORMAT( D14.7 )
8054  FORMAT( /, ' linear terms:',/ )
8055  FORMAT( D14.7, 1X )
8056  FORMAT( /, ' constant term: ', /, D14.7,/ )
8060  FORMAT( /, ' DFO: **********end of model coefficients:********')
8065  FORMAT( /, ' DFO: trust region minimization' )
8067  FORMAT( /, ' DFO: PRERED=', d14.7,' is too small',/
     +           ' DFO:       a new points is not produced' )
8070  FORMAT( /, ' DFO: sufficient reduction: take the step' )
8080  FORMAT( /, ' DFO: ared / prered = ', D14.7 )
8085  FORMAT( /, ' DFO: attempt a jump' )
8090  FORMAT( /, ' DFO: attempt jump distance = ', D14.7 )
8092  FORMAT( /, ' DFO: actual jump distance = ', D14.7 )
8095  FORMAT( /, ' DFO: a new point produced by jump' )
8097  FORMAT( /, ' DFO: jump accepted, new prered/ared=', D14.7 )
8100  FORMAT( /, ' DFO: the new point gives good reduction' )
8105  FORMAT( /, ' DFO: the new point does not give good reduction' )
8110  FORMAT( /, ' DFO: the new points is added to the model,',/
     +           ' DFO:       pivot=', D14.7 )
8120  FORMAT( /, ' DFO: the new point replaced ',I4,'-th point,',/
     +           ' DFO:       pivot=', D14.7  )
8140  FORMAT( /, ' DFO: improve the geometry' )
8130  FORMAT( /, ' DFO  : increase the trust region radius' )
8150  FORMAT( /, ' DFO  : decrease the trust region radius' )
8160  FORMAT( /, ' DFO  : trust region radius is at the lower bound',/
     +           ' DFO:       stopping count = ', I2 )
8170  FORMAT( /, ' DFO: relative pivots of the interpolation set:',/)
      END








**************************************************************

*    NEXT SUBROUTINE

**************************************************************


      SUBROUTINE BESTPT( N, M, X, FX, C, POINTS, VALUES, OBJVAL,
     +                   CONVAL, NQ )
C  ----------------------------------------------------------
C
C  THIS SUBROUTINE SELECTS THE POINT WITH THE BEST VALUE FROM
C  ARRAY 'POINTS' WITH VALUE 'VALUES'. THE POINT IS RECORDED 
C  IN X AND THE VALUE - IN FX.
C
C  ----------------------------------------------------------
      

C
C  SUBROUTINE PARAMETERS
C

      INTEGER          N, NQ, M
      DOUBLE PRECISION X(N), FX, POINTS(NQ*N), VALUES(NQ), 
     +                 CONVAL(NQ*M), C(M), OBJVAL(NQ)

C
C  COMMON VARIABLES
C
      INTEGER          IOUT  , IPRINT
      DOUBLE PRECISION MCHEPS, CNSTOL
      COMMON / DFOCM / IOUT  , IPRINT, MCHEPS, CNSTOL
      SAVE / DFOCM /

C
C  LOCAL VARIABLES
C  
      INTEGER          I, IMIN
      DOUBLE PRECISION VAL

C
C  SUBROUTINE CALLED
C
C  BLAS:      DCOPY
C

     
      VAL   = VALUES(NQ)
      CALL UNSHFT( N, X, POINTS((NQ-1)*N + 1) )
      IMIN = NQ
      DO 10 I = NQ-1, 1, -1
        CALL UNSHFT( N, X, POINTS((I-1)*N + 1) )
        IF ( VALUES(I) .LT. VAL - MCHEPS ) THEN
          VAL  = VALUES(I)
          IMIN = I
        ENDIF
 10   CONTINUE 
      FX   = OBJVAL(IMIN) 
      CALL DCOPY(  N, POINTS((IMIN-1)*N+1), 1, X, 1 )
      CALL DCOPY(  M, CONVAL((IMIN-1)*M+1), 1, C, 1)

      RETURN 
      END


**************************************************************

*    NEXT SUBROUTINE

**************************************************************


      SUBROUTINE SCL(N, X, SCAL)
C  **********************************************************
C  THIS SUBROUTINE SCALES (BY DOING ENTREE-WISE MULTIPLICATION) 
C  VECTOR X BY VECTOR 1/SCAL
C  **********************************************************
      INTEGER          N
      DOUBLE PRECISION X(N), SCAL(N)
      INTEGER          I

      DO 10 I=1,N
        X(I)=X(I)/SCAL(I)
 10   CONTINUE

      RETURN 
      END



**************************************************************

*    NEXT SUBROUTINE

**************************************************************

  
      SUBROUTINE UNSCL(N, X, SCAL)
C  **********************************************************
C  THIS SUBROUTINE SCALES (BY DOING ENTREE-WISE MULTIPLICATION) 
C  VECTOR X BY VECTOR SCAL
C  **********************************************************
      INTEGER          N
      DOUBLE PRECISION X(N), SCAL(N)
      INTEGER          I

      DO 10 I=1,N
        X(I)=X(I)*SCAL(I)
 10   CONTINUE

      RETURN  
      END



**************************************************************

*    NEXT SUBROUTINE

**************************************************************


      SUBROUTINE COMPLT( POLY, VALINT, PIVVAL, N, NIND, LPOLY)

C  *********************************************************** 
C  THIS SUBROUTINE COMPLETES THE LINEAR BLOCK OF THE INTERPOLATION
C  WITH DUMMY ELEMENTS, SO THAT THEY ARE NEVER CONSIDERED AND
C  NEVER HAVE EFFECT ON INTERPOLATION, AND SO THAT THE LINEAR
C  BLOCK HAS SIZE N + 1, I.E., IS COMPLETE.
C
C  PARAMETERS
C  
C    POLY   (INPUT/OUTPUT)  NEWTON FUNDAMENTAL POLYNOMAILS
C
C    VALINT (INPUT/OUTPUT)  VALUES OF INTERPOLATION POINTS
C
C    PIVVAL (INPUT/OUTPUT)  PIVOT VALUES OF INTERPOLATION POINTS
C
C     N     (INPUT)         PROBLEM DIMENSION
C
C    NIND   (INPUT/OUTPUT)  NUMBER OF INTERPOLATION POINTS/POLYNOMIALS
C  ***********************************************************


C
C  SUBROUTINE PARAMETERS
C

      INTEGER          N, NIND, LPOLY
      DOUBLE PRECISION POLY(LPOLY), VALINT(N+1), PIVVAL(N+1)

C
C  LOCAL VARIABLES
C
      INTEGER          I, IBEG, IEND
      DOUBLE PRECISION ZERO, HUGE
      PARAMETER       (ZERO = 0.0D0, HUGE = 1.0D20)

C
C  SET ALL POLYNOMIALS FROM NIND+1-ST TILL N+1-ST TO ZERO
C

      IBEG = 2 + (N+1)*(NIND-1) 
      IEND = 1 + (N+1)*N
      DO 10 I=IBEG, IEND
        POLY(I)=ZERO
 10   CONTINUE 
C
C  SET THE CORRESPONDING FUNCTION VALUES TO HUGE (SO THEY WILL NOT
C  BE PICKED UP AS THE  SMALLEST CURRENT VALUE), AND PIVOT VALUE TO
C  HUGE, SO THAT THEY WILL NOT BE REPLACED LATER
C 
      DO 20 I = NIND+1, N+1
        VALINT(I)=HUGE
        PIVVAL(I)=HUGE
 20   CONTINUE 
C
C  SET NING TO THE SIZE OF FULLY LINEAR MODEL
C 
      NIND = N+1
      RETURN
      END




**************************************************************

*    NEXT SUBROUTINE

**************************************************************

      SUBROUTINE INTDIM( X    , N , NCLIN , NCNLN , NEQCON, A   , LDA ,  
     +                   LB   , UB, PIVTHR, WRK   , LWRK  , IWRK, LIWRK)

C  ******************************************************************
C  THIS SUBROUTINE IDENTIFIES THE NUMBER OF LINEARLY INDEPENDENT 
C  EQUALITY CONSTRAINTS OF THE FEASIBLE SET. A CONSTRAINT IS CONSIDERED
C  EQUALITY IF THE UPPER AND LOWER BOUND DIFFER BY LESS THAT PIVTHR.
C  TO FIND THIS NUMBER WE CONSTRUCT THE JACOBIAN MATRIX OF EQUALITY
C  CONSTRAINTS AND THEN COMPUTE ITS QR DECOMPOSITION AND COUND THE
C  NUMBER OF ZEROS IN THE DIAGONAL OF R.
C  THIS NUMBER OF LIN. IND. EQUAL. IS COMPUTED AT A GIVEN POINT, SINCE
C  IN THE PRESENCE OF NONLINEAR CONSTRAINTS IT CAN BE DIFFERENT 
C  (IN THEORY)
C
C   PARAMETERS
C
C    X      (INPUT)  CURRENT POINT
C
C    N      (INPUT)  DIMENSION OF THE PROBLEM
C
C    NCLIN  (INPUT)  NUMBER OF LINEAR CONSTRAINTS
C    
C    NCNLN  (INPUT)  NUMBER OF NONLINEAR CONSTRAINTS
C
C    NEQCON (OUTPUT) THE NUMBER OF LINEAR INDEP. EQUALITY CONSTR.
C
C    A      (INPUT)  MATRIX OF LINEAR CONSTRAINTS
C
C    LDA    (INPUT)  LEADING DIMENSION OF MATRIX A 
C                   
C    LB     (INPUT)  ARRAY OF LOWER BOUNDS
C
C    UB     (INPUT)  ARRAY OF UPPER BOUNDS
C
C    PIVTHR (INPUT)  PIVOT THRESHOLD
C
C    WRK    (INPUT)  WORKING REAL SPACE ARRAY
C
C    IWRK   (INPUT)  WORKING INTEGER SPACE ARRAY
C  *******************************************************************

C
C  SUBROUTINE PARAMETERS
C
      DOUBLE PRECISION A(LDA*N) , LB(N + NCLIN + NCNLN),
     +                 PIVTHR   , UB(N + NCLIN + NCNLN),
     +                 WRK(LWRK), X(N) 

      INTEGER          N, NCLIN, NCNLN, NEQCON, LDA,  LWRK, LIWRK, 
     +                 IWRK(LIWRK)


C
C  COMMON VARIABLES
C
      INCLUDE 'dfo_model_inc.f'

      INTEGER          IOUT  , IPRINT
      DOUBLE PRECISION MCHEPS, CNSTOL
      COMMON / DFOCM / IOUT  , IPRINT, MCHEPS, CNSTOL
      SAVE / DFOCM /
C
C  LOCAL VARIABLES
C

      INTEGER          INFO, I, J, M, IC, ICJAC, LCJAC, LDMAT, IMAT,
     +                 LMAT, ICURW, LRS, LDCJ


      DOUBLE PRECISION ZERO, ONE
      PARAMETER      ( ZERO = 0.0D0, ONE =1.0D0 ) 

      INTRINSIC        ABS, MAX


C
C     SUBROUTINES AND FUNCTIONS CALLED:
C
C       APPLICATION:       RZRVEC, IZRVEC, FUNCON, 
C       LAPACK     :       DGEQPF
C       FORTRAN SUPPLIED:  ABS, MAX
C       BLAS:              DCOPY 
C

      LDCJ  = MAX(NCNLN, 1)

C
C  PARTITION THE MEMEORY
C

C
C  POINTER TO ARRAY OF NONLINEAR CONSTRAINTS VALUES
C
      IC    = 1
C
C  POINTER TO ARRAY WITH THE JACOBIAN OF NONLINEAR CONSTRAINTS
C
      ICJAC = IC    + LDCJ
C
C  POINTER TO MATRIX CONTAINING THE JACOBIAN  OF EQUALITY CONSTRAINTS
C
      IMAT  = ICJAC + LDCJ * N
C
C  POINTER TO CURRENT WORKING ARRAY
C
      ICURW = IMAT  +(NCLIN + NCNLN + N) * N
C
C  CHECK IF MEMORY IS SUFFICIENT
C
      LRS = LWRK - ICURW + 1
      IF ( LRS .LT. 4*N ) THEN
        IF ( IPRINT .GE. 0 ) WRITE( IOUT, 1000 ) - LRS - 4*N + 1 
        STOP
      ENDIF

      IF ( LIWRK .LT. N ) THEN
        IF ( IPRINT .GE. 0 ) WRITE( IOUT, 1100 ) - LIWRK - N + 1 
        STOP
      ENDIF
C
C  SET SOME AUXILARY POINTERS
C
      LMAT  = IMAT - 1
      LCJAC = ICJAC - 1
      LDMAT = N + NCLIN + NCNLN

      
      CALL RZRVEC( WRK(IMAT), LDMAT*N )
C
C  IF THERE ARE NONLINEAR CONSTRAINTS, COMPUTE THEIR JACOBIAN
C
      IF ( NCNLN .GT. 0 ) THEN 
         USEMERIT=.TRUE.
         CALL FUNCON(2, NCNLN  , N, LDCJ   , IWRK,
     +               X, WRK(IC), WRK(ICJAC), 1     )
         USEMERIT=.FALSE.
      ENDIF

C  ------------------------------------------
C  CONSTRUCT JACOBIAN OF EQUALITY CONSTRAINTS
C  ------------------------------------------

C
C  IF THE ARE ANY FIXED VARIABLES INCLUDE CORRESPONDING ROW
C  OF IDENTITY IN THE JACOBIAN
C   
      M = 0 
      DO 20 I=1, N
        IF ( UB(I)-LB(I) .LT. PIVTHR ) THEN
          DO 10 J = 1, N
            M = M + 1
            IF ( J .EQ. I ) THEN
              WRK( LMAT + (J-1)*LDMAT + I )= ONE
            ELSE
              WRK( LMAT + (J-1)*LDMAT + I )= ZERO
            ENDIF
 10       CONTINUE
        ENDIF
 20   CONTINUE

C
C  CHECK IF THE THERE ARE EQUALITIES AMONG LINEAR CONSTRAINTS,
C  FOR EACH EQUALITY WRITE CORRESPONDING ROW OF THE JACOBIAN
C

      DO 30 I=1 ,  NCLIN
        IF ( UB(I + N) - LB(I + N) .LT. PIVTHR ) THEN
          M = M + 1
          CALL DCOPY(N, A( I ), LDA, WRK(LMAT + M), LDMAT) 
        ENDIF
 30   CONTINUE

C
C  CHECK IF THE THERE ARE EQULITIES AMONG NONLINEAR CONSTRAINTS,
C  FOR EACH EQUALITY WRITE CORRESPONDING ROW OF THE JACOBIAN
C

      DO 40 I=1, NCNLN
        IF ( UB(I + N + NCLIN)-LB(I + N + NCLIN) .LT. PIVTHR ) THEN
          M = M + 1
          CALL DCOPY( N, WRK(LCJAC + I), LDCJ, WRK(LMAT + M), LDMAT )
        ENDIF
 40   CONTINUE
       
      CALL IZRVEC( IWRK, N )
      
      
C
C  IF THE NUMBER OF EQUALITIES IS BIGGER THAN NUMBER OF VARIABLES
C  PRINT AN ERROR MESSAGE  AND QUIT (THE NUMBER OF LINEARLY INDEP.
C  CONSTRAINTS MAY STILL BE LESS THAN N, WE HAVE TO THINK HOW TO
C  TAKE CARE OF IT BETTER.
C

      IF ( M .GT. N ) THEN
        IF ( IPRINT .GE. 0 ) WRITE( IOUT, 1200 ) M, N
        STOP
      ENDIF
C
C  COMPUTE THE "QR" FACTORIZATION
C
      CALL DGEQPF( M, N, WRK(IMAT), LDMAT, IWRK, WRK(ICURW), 
     +             WRK(ICURW + N) , INFO )

C
C  LOOK THROUGH THE DIAGONAL ELEMENTS OF "R", FOR EACH ZERO FOUND
C  REDUCE THE NUMBER OF LIN. IND. CONSTRAINTS BY 1.
C
      NEQCON = M
      DO 50 I = M, 1, -1
        IF ( ABS(WRK( LMAT + (I-1)*LDMAT + I )) .LT. 100*MCHEPS ) 
     +       NEQCON = NEQCON - 1
 50   CONTINUE


      RETURN
1000  FORMAT(/ ' INTDIM: *** ERROR: LWRK IS TOO SMALL!' /
     +         '             ADDITIONAL SPACE OF ',I8 ,' REQUIRED')
1100  FORMAT(/ ' INTDIM: *** ERROR: LIWRK IS TOO SMALL!' /
     +         '             ADDITIONAL SPACE OF ',I8 ,' REQUIRED')
1200  FORMAT(/ ' INTDIM: *** ERROR: NUMBER OF EQUALITY CONSTRAINTS!',
     +    I8,/ '             IS BIGGER THAN THE DIMENSION  ',I8 )
      END  






