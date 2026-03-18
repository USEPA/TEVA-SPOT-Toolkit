      SUBROUTINE DFO( N     , NX   , X     , LDX, FX, CONX,
     +                IFINIV, M    , C     , 
     +                NCLIN , NCNLN, LB    , UB , A   , LDA , 
     +                XNAMES, PNAME, CNAMES, IT , NF  , INFO,
     +                MAXIT,  MAXNF, STPCRTR, DELMIN, STPTHR,
     +                CNSTOLP, DELTA, PP, SCALE, IOUTP , IPRINTP)

C
C  ****************************************************************
C  THIS SUBROUTINE MINIMIZES A NONLINEAR OBJECTIVE FUNCTION
C  SUBJECT TO LINEAR AND (POSSIBLY) NONLINEAR CONSTRAINTS
C  AND SIMPLE BOUNDS, WITHOUT USING DERIVATIVES OF 
C  THE OBJECTIVE FUNCTION AND NONLINEAR CONSTRAINTS.
C
C  THE PROBLEM IS CONSIDERED TO BE OF THE FORM
C                        
C                           MIN F( X )
C         
C        S.T.
C                               / X  \
C                        LB <= ( AX   ) <= UB
C                               \C(X)/
C
C  THIS PROGRAM IS BASED ON USING QUADRATIC INTERPOLATION OF THE
C  OBJECTIVE FUNCTION AND CONSTRAINTS IN COMBINATION WITH TRUST 
C  REGION FRAMEWORK
C
C  PARAMETERS
C  ==========
C
C  (INPUT) 
C
C    X         : ARRAY OF NX  INITIAL POINTS, GIVEN BY THE USER
C                'X' HAS TO CONTAIN AT LEAST ONE STARTING POINT
C                IF NO FUNCTION VALUES ARE PROVIDED FOR THE POINTS
C                IN 'X', THEN ALL BUT THE FIRST POINT ARE IGNORED.
C
C    LDX       : LEADING DIMENSION OF ARRAY 'X' (LDX>= N)
C
C    FX        : ARRAY OF FUNCTION VALUES AT THE INITIAL POINTS
C                OF LENGTH AT LEAST 1 ( MAY BE DUMMY IF NO VALUES  
C                ARE PROVIDED )
C    CONX      : ARRAY OF INITIAL VALUES OF THE DERIVTIVE FREE CONSTRAINTS
C 
C
C    NX        : NUMBER OF INITIAL POINTS FOR WHICH THE VALUE IS
C                PROVIDED. IF NO SUCH POINTS PROVIDED, THEN NX=1
C
C    IFINIV    : LOGICAL VARIABLE, .TRUE. IS INITIAL VALUES FOR
C                NX INITIAL POINTS ARE PROVIDED, .FALSE. OTHERWISE
C
C    N         : PROBLEM DIMENSION
C
C    NCLIN     : NUMBER F LINEAR CONSTRAINTS
C
C    NCNLN     : NUMBER OF NONLINEAR CONSTRAINTS
C
C    M         : NUMBER OF DERIVATIVE FREE CONSTRAINTS
C
C    LB        : ARRAY OF LOWER BOUNDS OF LENGTH >= (N+NCLIN+NCNLN)
C
C    UB        : ARRAY OF UPPER BOUNDS OF LENGTH >= (N+NCLIN+NCNLN)
C
C    A         : MATRIX OF LINEAR CONSTRAINTS,( DIMENSIONS: LDA X N)
C
C    LDA       : LEADING DIMENSION OF MATRIX A, HAS TO BE >= MAX(1, NCLIN)
C
C
C    XNAMES    : ARRAY OF STRINGS OF CHARACTERS, CONTAINING
C                NAMES OF THE VARIABLES
C
C    PNAME     : STRING OF CHARACTERS CONTAINING THE NAME OF THE PROBLEMS
C
C    CNAMES    : ARRAY OF STRINGS OF CHARACTERS, CONTAINING NAMES
C                OF CONSTRAINTS
C    MAXIT     : MAXIMUM NUMBER OF ITERATIONS ALLOWED
C   
C    MAXNF     : MAXIMUM NUMBER OF FUNCTION EVALUATIONS
C
C    STPCRTR   : STOPPING CRITERIA (1-BY MIN DELTA, 2-BY SLOW PROGRESS)
C    
C    DELMIN    : MINIMUM TRUST REGION RADIUS
C
C    STPTHR    : THRESHOLD FOR SLOW PROGRESS
C
C    CNSTOLP   : FEASIBILITY TOLERANCE FOR "EASY" CONSTRAINTS
C
C    DELTA     : INITIAL TRUST REGION RADIUS
C
C    PP        : THE PENALTY PARAMETER
C    
C    SCALE     : SCALING, 0-NO SCALING, 1-SCALE INITIAL POINT TO UNIT VECTOR
C                2- SCALE THE BOX TO UNIT BOX
C    IOUTP     : OUTPUT DEVICE, 6=SCREEN
C 
C    IPRINTP   : LEVEL OF PRINT <0 - SILENT MODE, 0 - NO OUTPUT, 
C                BESIDES FINAL OUTPUT AND SOME WARNINGS
C                AND ERROR MESSAGES, 1- SUMMARY OF EACH ITERATION
C                AND SOME WARNINGS, 2,3 - PROGRESSIVELY MORE OUTPUT.
C
C    (OUTPUT)
C
C     X        : THE OPTIMAL (OR BEST FOUND)  POINT 
C
C     FX       : THE VALUE OF OBJECTIVE FUNCTION AT X
C  
C     C        : THE ARRAY OF VALUES OF THE DERIVATIVE FREE CONSTRAITNS
C                AT X
C
C     INFO     : THE EXIT INFORMATION:
C                0     SUCCESSFUL MINIMIZATION
C                1     TOO MANY FUNCTION EVALUATIONS
C                2     TOO MANY ITERATIONS
C               -1     ILLEGAL VALUE OF AN INPUT PARAMETER
C               -2     ILLEGAL VALUE OF LDA OR LDX
C               -3     REAL WORKSPACE IS TOO SMALL
C               -4     INTEGER WORKSPACE IS TOO SMALL
C               -5     INCONSISTENT BOUNDS
C               -6     FUNCTION VALUE CANNOT BE COMPUTED
C                      AT INITIAL POINT
C               -7     MINIMIZATION OVER TRUST REGION FAILED
C               -8     MAXNF IS TOO SMALL OR T0O LARGE
C               -9     CANNOT BUILD A 2 POINT MODEL
C     IT       : THE NUMBER OF ITERATIONS
C
C     NF       : THE NUMBER OF FUNCTION EVALUATIONS
C
C  ********************************************************************
C
C     Programming:  K. Scheinberg, 1998-2003.
C
C  ********************************************************************
C

C
C  SUBROUTINE PARAMETERS
C
      INTEGER          N , M , NX  , LDX  , NCLIN, NCNLN, LDA, 
     +                 IT, NF, INFO

      DOUBLE PRECISION X(LDX*NX), FX(NX) , LB(*),
     +                 UB(*) , A(LDA*N), 
     +                 C(*), CONX(*)        

      LOGICAL          IFINIV

      CHARACTER*256     XNAMES(N), PNAME, CNAMES(*)
C
C  COMMON VARIABLES
C
C
      INCLUDE 'dfo_model_inc.f'
C
C  PROCESS CONTROL PARAMETERS
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
C  LENGTH OF ARRAYS
C

      INTEGER          LPOLY, LPNTS, LVALUE, LPTINT, LVLINT, LCONVL
      COMMON /RPART/   LPOLY, LPNTS, LVALUE, LPTINT, LVLINT, LCONVL
      SAVE /RPART/

C
C  LOCAL VARIABLES AND SOME USER DEFINED PARAMETERS
C
      INTEGER          LWRK , LIWRK, DDMAX , LMAX  , RSMAX0, RSMAX1, 
     +                 RSMAX2, RSMAX3,
     +                 RSMAX4 , ISMAX0, ISMAX1, ISMAX2, NFPMAX

      PARAMETER      ( LMAX   = NVARMX+NLINMX+NNLNMX+NCONMX )

      PARAMETER      ( DDMAX  = (NVARMX+1)*(NVARMX+2)/2)

      PARAMETER      ( NFPMAX = DDMAX*(NVARMX+1)*NVARMX/2+(NVARMX+1)*
     +                          NVARMX+1 )

      PARAMETER      ( RSMAX0 = NFUNMX*(NVARMX+3+NCONMX) + 
     +                          DDMAX*(NVARMX+3) + 2*NVARMX + NVARMX**2 
     +                        + NVARMX**2*NCONMX+NCONMX+NFPMAX)
      PARAMETER      ( RSMAX1 = 3*(LMAX) + (NVARMX+1)*(NNLNMX+NCONMX)
     +                          + NVARMX*NVARMX + 2*NVARMX + 1 )
      PARAMETER      ( RSMAX2 = 2*NVARMX*NVARMX + NVARMX*NLINMX +
     +                        2*NVARMX*(NNLNMX+NCONMX) 
     +                      + 20*NVARMX + 11*NLINMX+21*(NNLNMX+NCONMX)) 
      PARAMETER      ( RSMAX3 = DDMAX*DDMAX + (DDMAX-1)*NVARMX + 
     +                          4*DDMAX-1 )
     
      PARAMETER      ( RSMAX4 = 3*NVARMX+NVARMX**2 )

      PARAMETER      ( ISMAX0 = NFUNMX + DDMAX )
      PARAMETER      ( ISMAX1 = 3*NVARMX + NLINMX+2*NNLNMX )
      PARAMETER      ( ISMAX2 = LMAX )
      PARAMETER      ( LWRK   = RSMAX0 + RSMAX1 +  RSMAX2 +  RSMAX4+ 
     +                          RSMAX3  )
      PARAMETER      ( LIWRK  = ISMAX0 + ISMAX1 +  ISMAX2 + DDMAX )
C
C  ALLOCATE THE WORKING SPACE
C 
C    WRK       : REAL WORKSPACE ARRAY OF SIZE LWRK
C                THE REQUIRED LENGTH OF LWRK IS ROUGHLY APPROXIMATED 
C                FROM ABOVE  BY:
C                (2N+26)(N+NCLIN+NCNLN+M)+(2NCNLN+2M+6)*N+(MAXNF+NX)(N+2)+
C                [(N+1)**2 (N+2)(N+14)]   
C 
C 
C    IWRK      : INTEGER WORKSPACE ARRAY OF SIZE LIWRK
C                THE REQUIRED LENGTH OF INTEGER SPACE IS
C                4(N+NCLIN+NCNLN) + (MAXNF+NX) + (N+1)(N+2)
      INTEGER          IWRK(LIWRK)
      DOUBLE PRECISION WRK(LWRK)


      INTEGER          MAXNF  , MAXIT , VARNT , IOSPEC, ISCAL,
     +                 IPAR(8), BADBND, SCALE , I     , J    ,
     +                 ICURW  , LENW  , LSCAL , MJIT  , NMJIT,
     +                 IP     , IV    , IOB   , IC    , ID   ,
     +                 LP     , LV    , LOB   , LC    , LD   ,
     +                 INFORM , BASE  , NQ    , METHOD, STPCRTR,
     +                 IPRINTP, IOUTP 
     
      DOUBLE PRECISION DELMIN  , LOWBND, ANOISE , RNOISE, PIVTHR, 
     +                 ADDTHR  , XCHTHR, RPAR(8), DELTA , DELMAX,
     +                 NOISE(2), AVGSC , VAL, PP, STPTHR, CNSTOLP
  
      LOGICAL          IFERR 
C
C  PARAMETERS
C   
      DOUBLE PRECISION ONE, ZERO
      PARAMETER      ( ONE = 1.0D0, ZERO = 0.0D0 )
C
C  SUBROUTINES CALLED: 
C
C  APPLICATIONS: DFOSLV, PTINIT, FUN, SCL, UNSCL
C  BLAS        : DDOT
C
C
C  SET PARAMETERS FOR /MERIT/ COMMON BLOCK
C 
      NCON=M
      NLIN=NCLIN
      NNLN=NCNLN
C
C  DEFINE THE MINIMIZATION PARAMETERS THAT ARE NOT DEFINED BY THE USER
C
C   MAXMIMUM TR RADIUS
      DELMAX=10*DELTA
C   LOWER BOUND ON FUNCTION VALUE
      LOWBND=-1.0D0
C   ABSOLUTE NOISE
      ANOISE=0.0D0
C   RELATIVE NOISE
      RNOISE=0.0D0
C   THRESHHOLD FOR POISEDNESS
      PIVTHR=0.001D0
C   THRESHOLD FOR IMPROVING POISEDNESS BY ADDING POINT
      ADDTHR=100D0
C   THRESHOLD FOR IMPROVING POISEDNESS BY REPLACING POINT
      XCHTHR=1000D0
C   MINIMUM NUMBER OF POINTS IN A MODEL 
      NPMIN = 2
C   LAYER*DELTA IS THE MAXIMUM DISTANCE TO THE BASE ALLOWED
C   FOR THE INTERPOLATION POINTS
      LAYER = 6
C   EFFORT LEVEL. 1,2 AND 3 INCREASING LEVELS OF LIN. ALGEBRA
      EFFORT = 1
C   METHOD FOR BULSING AN INCOMPLETE INTERP. MODEL. 1- MIN. FROBENIUS
C                                    2- INCOMPLETE MONOMIALS
      VARNT = 1
C   NUMBER OF MAJOR ITERATIONS
      NMJIT = 1
C   METHOD FOR HANDLING CONSTRAINTS. 1 - MODEL CONSTR, 2-SAME WITH SAFEGUARD,
C          3, 4 PENALTY FUNCTIONS
C
C    IF USING IPOPT DO NOT USE OPTIONS 2 OR 3 FOR NOW!!!
C
      METHOD=1
C   RECORD SOME OTHER PARAMETERES INTO COMMON BLOCK VARIABLES
      IOUT=IOUTP
      IPRINT=IPRINTP
      CNSTOL=CNSTOLP
C
C  STORE THE MINIMIZATION PARAMETERS, OBTAINED FROM SPECIFICATION FILE
C
   
      MCHEPS     = 1.5D-16
      IPAR( 1 )    = MAXIT
      IPAR( 2 )    = MAXNF
      IPAR( 3 )    = NPMIN
      IPAR( 4 )    = LAYER
      IPAR( 5 )    = EFFORT
      IPAR( 6 )    = VARNT
      IPAR( 7 )    = METHOD
      IPAR( 8 )    = STPCRTR
      RPAR( 1 )    = DELTA
      RPAR( 2 )    = DELMAX
      RPAR( 3 )    = DELMIN
      RPAR( 4 )    = PIVTHR
      RPAR( 5 )    = ADDTHR
      RPAR( 6 )    = XCHTHR
      RPAR( 7 )    = LOWBND
      RPAR( 8 )    = STPTHR
      NOISE( 1 )   = ANOISE
      NOISE( 2 )   = RNOISE

C  ----------------------------------
C  CHECK IF THE INITIAL DATA IS VALID
C  ----------------------------------
 

C
C  CHECK IF THE DIMENSIONS ARE POSITIVE
C
      IF ( N .LE. 0 .OR. N .GT. NVARMX ) THEN
         IF (IPRINT .GE. 0 ) WRITE( IOUT, 1000 ) N
         INFO = -1
         RETURN
      ENDIF

      IF ( M .LT. 0 .OR. M .GT. NCONMX ) THEN
         IF (IPRINT .GE. 0 ) WRITE( IOUT, 1005 ) M
         INFO = -1
         RETURN
      ENDIF

      IF ( NCLIN .LT. 0  .OR. NCLIN .GT. NLINMX) THEN
         IF (IPRINT .GE. 0 ) WRITE( IOUT, 1010 ) NCLIN
         INFO = -1
         RETURN
      ENDIF

      IF (  NCNLN .LT. 0 .OR. NCNLN .GT. NNLNMX) THEN
         IF (IPRINT .GE. 0 ) WRITE( IOUT, 1020 ) NCNLN
         INFO = -1
         RETURN
      ENDIF
      IF ( MAXNF .GT. NFUNMX-NX ) THEN
         IF (IPRINT .GE. 0 ) WRITE( IOUT, 1110 ) NFUNMX
         INFO = -8
         RETURN
      ENDIF
      IF ( MAXNF .LE. 0 ) THEN 
         IF (IPRINT .GE. 0 ) WRITE( IOUT, 1050 ) 2
         INFO=-8
         RETURN
      ENDIF
C
C  CHECK IF THE  BOUNDS ARE CONSISTENT
C
      BADBND=0
      DO 10 I=1,N+NCLIN+NCNLN+M
        IF (LB( I ) .GT. UB( I )+MCHEPS) BADBND=1
 10   CONTINUE
      IF (BADBND.EQ.1) THEN
        IF (IPRINT .GE. 0 ) WRITE(IOUT, 1030)
        INFO=-5
        RETURN
      ENDIF
    


C
C  CHECK IF DIMENSION OF LINEAR CONSTRAINT MATRIX IS SATISFACTORY
C

      IF ( LDA .LT. MAX(NCLIN,1) ) THEN 
         IF (IPRINT .GE. 0 ) WRITE(IOUT, 1060) MAX(NCLIN,1)
         INFO = -2              
         RETURN
      ENDIF

C
C  CHECK IF LEADING DIMENSION OF INITIAL SET 'X' IS SATISFACTORY
C

      IF ( LDX .LT. N ) THEN 
         IF (IPRINT .GE. 0 ) WRITE(IOUT, 1080) N
         INFO = -2              
         RETURN
      ENDIF

C
C  CHECK IF OTHER DIMENSION OF INITIAL SET 'X' IS POSITIVE
C

      IF ( NX .LE. 0 ) THEN 
         IF (IPRINT .GE. 0 ) WRITE(IOUT, 1090) 
         INFO = -2              
         RETURN
      ENDIF
C
C  CHECK IF OTHER INPUT PARAMETERS ARE VALID
C
      IF ( STPCRTR .GT. 2 .OR. STPCRTR .LT. 1 .OR. DELMIN.LT. 0 .OR.
     +     STPTHR .LT. 0 .OR. CNSTOLP .LT. 0 .OR. DELTA .LT. 0 ) THEN
         IF (IPRINT .GE. 0 ) WRITE(IOUT, 1100) 
         INFO = -1           
         RETURN
      ENDIF

C
C  PRINT OUT SOME SPECIFICATIONS INFO
C


      IF ( IPRINT .GE. 2 ) THEN
        IF ( VARNT .EQ.1 ) THEN
          WRITE ( IOUT, 4000 )
        ELSE
          WRITE ( IOUT, 4020 )
        ENDIF
      ENDIF
C
C  REAL SPACE ALLOCATION
C

C
C  SET THE POINTERS TO SPACE FOR SCALING COEFFICIENTS
C
      ISCAL = 1
C
C  POINTER TO  ARRAY OF ALL SAMPLE POINTS
C
      IP  = ISCAL + N
C
C  POINTER TO MERIT FUNCTION VALUES AT SAMPLE POINTS
C
      IV  = IP + ( MAXNF + NX ) * N
C
C  POINTER TO OBJECTIVE FUNCTION VALUES AT SAMPLE POINTS
C
      IOB = IV + ( MAXNF + NX ) 
C
C  POINTER TO CONSTRAINT  FUNCTION VALUES AT SAMPLE POINTS
C
      IC  = IOB + ( MAXNF + NX ) 
C
C  POINTER TO DISTANCES BETWEEN CURRENT BASE AND  SAMPLE POINTS
C
      ID  = IC + MAX(1,( MAXNF + NX )*M)
C
C  CHECK IF REAL SPACE IS SUFFICIENT
C
      ICURW = ID + ( MAXNF + NX )
      LENW  = LWRK - ICURW + 1
      IF ( LENW .LT. 0 ) THEN
        WRITE( IOUT, 1070 ) LWRK
        INFO = -3
        RETURN
      ENDIF
      LSCAL = ISCAL - 1
C
C INITIALIZE NF TO ZERO.
C IF NO INITIAL POINT WITH FUNCTION VALUE IS PROVIDED, COMPUTE
C THE FUNCTION VALUE AT THE FIRST POINT AND SET NF = 1
C
      NF = 0
      IF ( .NOT. IFINIV ) THEN
        CALL FUN( N, M, X, WRK(IOB), WRK(IC), IFERR )
        IF ( IFERR ) THEN
          IF (IPRINT .GE. 0 ) WRITE(IOUT, 1040)
          INFO = -6
          RETURN
        ENDIF
        FX(1)=WRK(IOB)
        CALL DCOPY(M, WRK(IC), 1,C, 1)
        NF = 1
        NX = 1
      ENDIF
C
C  CHECK IF THE INITIAL NUMBER OF INTERPOLATION POINTS
C  IS NOT LARGER THAN MAXIMUM NUMBER OF FUNCTION EVALUATIONS
C  
      IF ( NF .GE. MAXNF ) THEN 
         IF (IPRINT .GE. 0 ) WRITE( IOUT, 1120 )
         INFO=1
         IT=1
         SCALE =0
         GOTO 48
      ENDIF

C
C  IF THERE IS SOME INTIAL INFORMATION, THEN WE RECORD IT
C  IN ARRAYS WRK(IOB) AND WRK(IC) FOR FUTURE CONVINIENT HANDLING.
C
 
      IF ( IFINIV ) THEN
        CALL DCOPY( NX, FX, 1, WRK(IOB), 1 )
        CALL DCOPY( NX*M, CONX, 1, WRK(IC), 1 )
      ENDIF

C       
C  CHECK IF SCALING AND 'EASY' NONLINEAR CONSTRAINTS ARE NOT USED AT
C  THE SAME TIME (NOT SUPPORTED BY CURRENT VERSION)
C
      IF ( SCALE .NE. 0 .AND. NCNLN .GT. 0 ) THEN
         IF (IPRINT .GE. 0 ) WRITE(IOUT, 5000) 
         SCALE = 0
      ENDIF

C       
C  SCALE THE PROBLEM IF REQUIRED
C

      IF ( SCALE .NE. 0 ) THEN
        AVGSC=ONE
        DO 20 I=1, N
          IF ( SCALE .EQ. 1 ) THEN
            WRK( LSCAL + I ) = ONE + ABS(X(I))
            AVGSC   = AVGSC * WRK( LSCAL + I )
          ELSE 
            WRK( LSCAL + I ) = UB(I) - LB(I)
            IF ( WRK( LSCAL + I). LT. CNSTOL .OR. UB(I) .GE. 1.0D+20 
     +                           .OR. LB(I)  .LE. -1.0D+20 ) THEN
              IF (IPRINT .GE. 0 ) WRITE( IOUT, 5200 )  I
              WRK( LSCAL + I ) = ONE
              AVGSC   = AVGSC * WRK( LSCAL + I )
            ENDIF
          ENDIF
 20     CONTINUE
      ENDIF

C
C  SCALE THE POINT AND THE BOUNDS
C
      IF ( SCALE .NE. 0 ) THEN
        DO 25 I=1,NX
          CALL SCL( N, X((I-1)*N+1), WRK( ISCAL ) )
 25     CONTINUE  
        CALL SCL( N, LB, WRK( ISCAL ) )
        CALL SCL( N, UB, WRK( ISCAL ) )
C
C  SHOULD SCALE THE TRUST REGION RADIUS AS WELL. 
C

        RPAR( 1 ) = RPAR( 1 )/(EXP(LOG(AVGSC)/N))
        RPAR( 2 ) = RPAR( 2 )/(EXP(LOG(AVGSC)/N))
        RPAR( 3 ) = RPAR( 3 )/(EXP(LOG(AVGSC)/N))
        DO 40 I=1,N
          DO 30 J=1,NCLIN
            A(LDA*(I-1)+J)=A(LDA*(I-1)+J)*WRK( LSCAL + I )
 30       CONTINUE  
 40     CONTINUE  
        IF ( IPRINT .GE. 2 ) THEN
          WRITE( IOUT, 8000 )
          DO 45 I = 1, N
            WRITE( IOUT, 8010 ) WRK( LSCAL + I )
 45       CONTINUE
        ENDIF  
      ENDIF
C
C  INITIALIZE: NUMBER OF SAMPLE POINTS
C              LENGTH OF ARRAY OF POINTS AND VALUES
C             

      NQ     = MAX( NX, 2 )
      LVALUE = NQ
      LPNTS  = NQ*N
      LCONVL = MAX(1,NQ*M)

C
C  COMPUTE INITIAL SET OF SAMPLE POINTS POINTS
C

      CALL PTINIT(N         , M      , X      , LDX     , NX     ,     
     +            NQ        , NF     , WRK(IP), WRK(IOB), WRK(IC), 
     +            WRK(ID)   , MAXNF, DELTA  ,              
     +            DELMAX    , PIVTHR*DELTA    , LB    , UB    , A ,
     +            LDA       , NCLIN  , NCNLN  , SCALE , WRK(ISCAL), 
     +            WRK(ICURW), LENW   , IWRK   , LIWRK , INFORM )


C
C  IF A SECOND POINT FOR WHICH FUNCTION VALUE  CAN BE COMPUTED
C  CANNOT BE FOUND, THEN PRINT A MESSAGE AND STOP
C
      IF ( INFORM .EQ. -1 ) THEN
        IF ( IPRINT .GE. 0 ) WRITE(IOUT, 2030)
        INFO = -9
        GOTO 48
      ENDIF
C
C  SOME PARAMETERS DATA PROBLEM IS SET INCORRECTLY
C  IT IS LIKELY THAT SOMETHING IS WRONG WITH CONSTRAINTS
C  PRINT A MESSAGE AND STOP
C

      IF ( INFORM .EQ. 1 ) THEN
        IF ( IPRINT .GE. 0 ) WRITE(IOUT, 2040)
        INFO = -7
        RETURN
      ENDIF
     

C
C  IF WE COULD NOT FIND AT LEAST TWO FEASIBLE POINTS
C  TO INITIATE THE PROCESS WE STOP AND PRINT A MESSAGE      
C
    
      IF ( INFORM .EQ. 2 ) THEN
        IF ( IPRINT .GE. 0 ) WRITE(IOUT, 2050)
        INFO = -9
        RETURN
      ENDIF
      LP  = 1
      LV  = IV  - IP + 1
      LD  = ID  - IP + 1
      LOB = IOB - IP + 1
      LC  = IC  - IP + 1

      DO 47 MJIT=1, NMJIT

C
C  FIND THE VALUE OF THE MERIT FUNCTION FOR EVERY POINT
C

        CALL VLINIT(N, M, NQ, X , BASE, WRK(IP), WRK(IV), WRK(IOB)  , 
     +              WRK(IC) , UB(N+NCLIN+NCNLN+1), LB(N+NCLIN+NCNLN+1), 
     +              WRK(ID), PP, METHOD,  WRK(ICURW), 
     +              LENW)

        FX(1)=WRK(IV+BASE-1)
C
C  CALL THE DFO SOLVER
C  
C
C  CHECK IF WE RAN OUT OF CUNTION EVALUATIONS
C 
      IF ( INFORM .EQ. -2 ) THEN
        IF ( IPRINT .GE. 0 ) WRITE(IOUT, 1120)
        INFO = 2
        CALL BESTPT( N, M, X, FX, C, WRK(IP), WRK(IV), WRK(IOB),
     +             WRK(IC), NQ )
        GOTO 48
      ENDIF
        CALL DFOSLV( N  , M  , NCLIN, NCNLN     , X     , FX   , 
     +               C  , NQ , BASE , LP        , LV    , LD   , 
     +               LOB, LC , LB    , UB   , 
     +               A  , LDA, SCALE, WRK(ISCAL), PP, INFO ,
     +               IT , NF , NOISE, IPAR      , RPAR  , ICURW,  
     +               WRK(IP) , LWRK-IP+1 , IWRK , LIWRK )

        IF ( NF .GE. MAXNF ) GO TO 48
        IF ( IT .GE. MAXIT ) GO TO 48
        PP = PP * 1.0D1
      
 47   CONTINUE  
C
C  UNSCALE THE PROBLEM IF SCALED
C

 48   CONTINUE

      IF (SCALE .NE. 0) THEN
        CALL UNSCL( N, X, WRK(ISCAL) )
        CALL UNSCL( N, LB, WRK(ISCAL) )
        CALL UNSCL( N, UB, WRK(ISCAL) )
        DO 60 I=1,N
          DO 50 J=1,NCLIN
            A(LDA*(I-1)+J)=A(LDA*(I-1)+J)/WRK( LSCAL + I )
 50       CONTINUE  
 60     CONTINUE  
      ENDIF

C
C  IF DEMANDED, PRINT OUT THE FINAL OUTPUT
C
      IF ( IPRINT .GE. 0 ) THEN
        WRITE ( IOUT, 6000 ) PNAME
        DO 70 I = 1, N
          WRITE( IOUT, 6200 ) XNAMES(I), X( I )
 70     CONTINUE
        DO 80 I = 1, M
          WRITE( IOUT, 6300 ) CNAMES(I), LB(N+NCLIN+NCNLN+I),
     .           C( I ), UB(N+NCLIN+NCNLN+I)
 80     CONTINUE
        WRITE( IOUT, 6400 )  PNAME, N, NF, IT, FX(1), INFO
C       WRITE( IOUT, 6600 )  PNAME, N, NF, IT, FX(1), INFO 
      ENDIF

      RETURN


C
C  NON-EXECUTABLE STATEMENTS
C

 

 1000 FORMAT(/' DFO: *** ERROR: NUMBER OF VARIABLES',/  
     +        ' DFO:            HAS ILLEGAL VALUE:', I6,/ )
 1005 FORMAT(/' DFO: *** ERROR: NUMBER OF GENERAL CONSTRAINTS',/  
     +        ' DFO:            HAS ILLEGAL VALUE:', I6,/)
 1010 FORMAT(/' DFO: *** ERROR: NUMBER OF LINEAR CONSTRAINTS',/  
     +        ' DFO:            HAS ILLEGAL VALUE:', I6,/)
 1020 FORMAT(/' DFO: *** ERROR: NUMBER OF NONLINEAR CONSTRAINTS',/  
     +        ' DFO:            HAS ILLEGAL VALUE:', I6,/ )
 1030 FORMAT(/' DFO: *** ERROR: THE UPPER AND LOWER BOUNDS ARE',/, 
     +        ' DFO:            INCONSISTENT', /)
 1040 FORMAT(/' DFO: *** ERROR: FUNCTION VALUE CANNOT BE OBTAINED',/,
     +        ' DFO:            FOR INITIAL POINT!')
 1050 FORMAT(/' DFO: *** ERROR: MAXNF IS TOO SMALL!' /
     +        ' DFO:            IT SHOULD BE AT LEAST ', I5 )
 1060 FORMAT(/' DFO: *** ERROR: THE LEADING DIMENSION OF ',/, 
     +        ' DFO:            LINEAR CONSTRAINTS SHOULD BE AT LEAST',
     +                          I6,/)
 1070 FORMAT('  DFO: *** ERROR: LWRK IS TOO SMALL EVEN TO START!' /
     +        ' DFO:             IT IS: ',I10 )
 1080 FORMAT(/' DFO: *** ERROR: THE LEADING DIMENSION OF ',/, 
     +        ' DFO:            INITIAL SET "X" SHOULD BE AT LEAST',
     +                          I6,/)
 1090 FORMAT(/' DFO: *** ERROR: THE NUMBER  OF INITIAL ',/, 
     +        ' DFO:            POINTS "X" SHOULD BE POSITIVE')
 1100 FORMAT(/' DFO: *** ERROR: AN INPUT PARAMETER ',/  
     +        ' DFO:            HAS AN ILLEGAL VALUE:', I6,/ )
 1110 FORMAT(/' DFO: *** ERROR: MAXNF OR NX IS TOO BIG!' /
     +        ' DFO:            MAXNF+NX IT SHOULD BE AT MOST ', I5 )
 1120 FORMAT(/' DFO: *** WARNING: MAXNF IS TOO SMALL' /
     +        ' DFO:              TO PERFORM OPTIMIZATION')
 2030 FORMAT(/' DFO: CANNOT COMPUTE THE FUNCTION VALUE FOR INITIAL', /
     +        ' DFO: INTERPOLATION SET OF AT LEAST TWO POINTS')
 2040 FORMAT(/' DFO: SOME PARAMETER OR DATA  IS SET INCORRECTLY', /
     +        ' DFO  CHECK CONSTRAINT COMPUTATIONS')
 2050 FORMAT(/' DFO: CANNOT FIND TWO POINTS TO BUILD INITIAL MODEL!', /
     +        ' DFO: CHECK FEASIBILITY AND ACCURACY REQUIREMENTS')
 3000 FORMAT( 4( D12.4, /), 2( I12, /), 6( D12.4, /), 8(I12, /), D12.4,
     +        /, I12 )
 5000 FORMAT(/' DFO: *** WARNING: SCALING IS NOT SUPPORTED ',/, 
     +        '          FOR PROBLEMS WITH NONLINEAR CONSTRAINTS',/,
     +        '          NO SCALING WILL BE PERFORMED',/)
 5200 FORMAT( ' DFO: *** WARNING:',I4,'-th variable cannot be scaled,'
     +                   ,/, 
     +        '          scaling coefficient is too small or',/,
     +        '          too large',/)
 4000 FORMAT( ' Minimum frobenius norm models will be used', / )
 4020 FORMAT( ' Models based on incomplete Newton Polynomial basis',/
     +        ' will be used',/ )

 6000 FORMAT( /, A10,'- OPTIMAL POINT:', / ) 
 6200 FORMAT( A10, D12.6, / )
 6300 FORMAT( A10, 3( D12.6), /) 
 6400 FORMAT( /, 24('*'), 'DFO: FINAL OUTPUT ', 24('*') //
     +    ' problem                 =      ', A8 ,   /,
     +    ' # variables             =      ', I10,   /,
     +    ' # objective functions   =      ', I10,   /,
     +    ' # iterations            =      ', I10,   /,
     +    ' Final f                 =      ', D15.7, /,
     +    ' Exit code               =      ', I10,   /,
     +     65('*') / )
 6600 FORMAT( A10, I10, 1X, I10, 1X, I10, 1X,  D14.7, 1X, I10 )
 8000 FORMAT( 'SCALING COEFFICIENTS:',/)
 8010 FORMAT( D14.7, 1X )


      END
*


