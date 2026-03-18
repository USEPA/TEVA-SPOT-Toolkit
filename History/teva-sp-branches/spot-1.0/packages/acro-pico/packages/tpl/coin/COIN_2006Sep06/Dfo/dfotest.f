
      PROGRAM DFOTEST

*  K. Scheinberg
*  2003.
*
      INTEGER          NMAX   , MAXFUN, LWRK  , LIWRK , DDMAX , NCLMAX, 
     +                 NCNMAX , LMAX  , RSMAX0, RSMAX1, RSMAX2, RSMAX3,
     +                 RSMAX4 , ISMAX0, ISMAX1, ISMAX2, NFPMAX, NXMAX ,
     +                 MMAX
      PARAMETER      ( NMAX   =  30 ,  NCLMAX = 10, NCNMAX=10, 
     +                 MAXFUN = 4000,  NXMAX  = 10, MMAX  = 15)
      PARAMETER      ( LMAX   = NMAX+NCLMAX+NCNMAX+MMAX )

      PARAMETER      ( DDMAX  = (NMAX+1)*(NMAX+2)/2)

      PARAMETER      ( NFPMAX = DDMAX*(NMAX+1)*NMAX/2+(NMAX+1)*NMAX+1 )

      PARAMETER      ( RSMAX0 = MAXFUN*(NMAX+2) + DDMAX*(NMAX+2)
     +                          + 2*NMAX + NMAX**2  + NFPMAX)
      PARAMETER      ( RSMAX1 = 3*(LMAX) + (NMAX+1)*NCNMAX 
     +                          + NMAX*NMAX + 2*NMAX + 1 )
      PARAMETER      ( RSMAX2 = 2*NMAX*NMAX + NMAX*NCLMAX +2*NMAX*NCNMAX
     +                          + 20*NMAX + 11*NCLMAX + 21*NCNMAX )
      PARAMETER      ( RSMAX3 = DDMAX*DDMAX + (DDMAX-1)*NMAX + 
     +                          DDMAX-1 )
     
      PARAMETER      ( RSMAX4 = 3*NMAX+NMAX**2 )

      PARAMETER      ( ISMAX0 = MAXFUN + DDMAX )
      PARAMETER      ( ISMAX1 = 3*NMAX + NCLMAX+2*NCNMAX )
      PARAMETER      ( ISMAX2 = LMAX )
      PARAMETER      ( LWRK   = RSMAX0 + RSMAX1 +  RSMAX2 +  RSMAX4+ 
     +                          RSMAX3  )
      PARAMETER      ( LIWRK  = ISMAX0 + ISMAX1 +  ISMAX2 + DDMAX )


      INTEGER          N    , IT , NF, EXIT, INPUT, 
     +                 NCLIN        , NCNLN, LDA, NX, LDX , IOUT ,
     +                 MAJIT        , ITEST, LN, MAXIT, MAXNF, SCALE,
     +                 IPRINT, STPCRTR          

            
      DOUBLE PRECISION X( NMAX*NXMAX ), FX(NXMAX)        ,
     +                 LB(LMAX)       , UB(LMAX)   , ALIN(NCLMAX*NMAX),
     +                 X0( NMAX*NXMAX ), V(MMAX)   , CONX(NXMAX*MMAX),
     +                 PP, DELMIN, DELTA, STPTHR, CNSTOL
      LOGICAL          LINEAR(MMAX), EQUATN(MMAX), EFIRST, LFIRST,
     +                 NVFRST   

      PARAMETER      ( IOUT = 6 )

      CHARACTER * 256    VNAME( NMAX ), PNAME, CNAME(MMAX+NCLMAX+NCNMAX)

      CHARACTER * 256   PRBDAT
      PARAMETER      ( INPUT = 55  )

      INTEGER NTEST
      COMMON  /TEST/    NTEST
      SAVE /TEST/
      CHARACTER*80 VALUE
 
      INTEGER          M
      DOUBLE PRECISION C(MMAX), LDCJ

C
C  SET THE TEST PROBLEM NUMBER
C     
      CALL GETARG(1, VALUE)
      READ(VALUE,*) NTEST
C
C  SET UP THE DATA STRUCTURES AND SOME DATA FOR DFO
C
      CALL SETUP ( IOUT  , N, X, NMAX, NCLMAX, NCNMAX, NCLIN, 
     *             NCNLN, LB , UB, 
     *              ALIN, LDA, M, MMAX,
     *              PNAME, VNAME,  CNAME )

      LDCJ=MAX(1, NCNLN)
      LN= N+NCLIN+NCNLN
      

C
C  SET THE NUMBER OF POINTS PROVIDED AND THEIR DIMENSION
C      
      CALL DCOPY(N, X, 1, X0, 1)
      NX = 1
      LDX= N
      MAXIT=1000
      MAXNF=1000
      STPCRTR=1
      DELMIN=0.001d0
      STPTHR=0.001d0
      CNSTOL=0.00001d0
      DELTA=1d0
      PP=1000d0
      SCALE=0
      IPRINT=1
        DO 5555 ITEST=1,1
          CALL DCOPY(N, X0, 1, X, 1)
           CALL DFO(N    , NX  , X    , LDX  , FX, CONX, .FALSE., M, 
     +              C    , NCLIN, NCNLN, LB, UB  , 
     +              ALIN , LDA , VNAME, PNAME, CNAME, IT, NF  , EXIT, 
     +              MAXIT,  MAXNF, STPCRTR, DELMIN, STPTHR,
     +              CNSTOL, DELTA, PP, SCALE,  IOUT , IPRINT)
           
 5555   CONTINUE  
      STOP
      END
  
C  ***************************************************
C
C  SUBROUTINE COMPUTING INITIAL DATA
C
C  ***************************************************
   

      SUBROUTINE SETUP( IOUT  , N    , X     , NMAX, NCLMAX, NCNMAX   , 
     +                  NCLIN , NCNLN, BL    , BU  , ALIN  , LDA, M, 
     +                  MMAX, PNAME, XNAMES,  GNAMES )
C
C  TEST PROBLEMs FOR DFO:
C  SETUP NUMBER OF VARIABLES, A STARTING POINT AND BOUNDS FOR 
C  PROBLEMS FROM HOCK AND SCHITTKOWSKI
C
C  NTEST = 1 --  HOCK AND SCHITTKOWSKI # 16
C  
C  NTEST = 2 --  HOCK AND SCHITTKOWSKI # 113
C
C  NTEST = 3 --  HOCK AND SCHITTKOWSKI # 112
C
C  NTEST = 4 --  ROSENBROCK BANANA FUNCTION
C
C  NTEST = 5 --  HOCK AND SCHITTKOWSKI # 111
C
C  NTEST = 6 --  HOCK AND SCHITTKOWSKI # 26
C
C  NTEST = 7 --  HOCK AND SCHITTKOWSKI # 50
C
C  NTEST = 8 --  HOCK AND SCHITTKOWSKI # 112
C
      INTEGER            IOUT  , N     , NMAX, NCLIN, NCNLN,
     +                   NCLMAX, NCNMAX, MMAX, M, LDA, J

      CHARACTER * 256     XNAMES( NMAX ), PNAME, 
     +                   GNAMES( MMAX )

      DOUBLE PRECISION   X( NMAX ), BL( NMAX+NCLMAX+NCNMAX+MMAX ), 
     +                   BU(NMAX+NCLMAX+NCNMAX+MMAX), 
     +                   ALIN(NCLMAX, NMAX)
C
C  COMMON VARIABLE FOR THE TEST NUMBER
C
      INTEGER           NTEST
      COMMON  /TEST/    NTEST
      SAVE /TEST/
C
C  LOCAL VARIABLES
C
      DOUBLE PRECISION  OPTVAL
      INTEGER           I
C
C  PROBLEM DIMENSION
C
      IF ( NTEST .EQ. 1 ) THEN
        N=2
        OPTVAL=0.25
      ELSEIF ( NTEST .EQ. 2 ) THEN
        N=10
        OPTVAL=24.30621
      ELSEIF ( NTEST  .EQ. 3 ) THEN
        N=10
        OPTVAL=-47.76065
      ELSEIF ( NTEST  .EQ. 4 ) THEN
        N=2
        OPTVAL=0.0
      ELSEIF ( NTEST  .EQ. 5 ) THEN
        N=10
        OPTVAL=-47.75997
      ELSEIF ( NTEST  .EQ. 6 ) THEN
        N=3
        OPTVAL=0.0
      ELSEIF ( NTEST  .EQ. 7 ) THEN
        N=5
        OPTVAL=0.0
      ELSEIF ( NTEST  .EQ. 8 ) THEN
        N=10
        OPTVAL=-47.76065   
C      ELSEIF ( NTEST .EQ. 9 ) THEN
C        N=2
C        OPTVAL=0.0
C      ELSEIF ( NTEST .EQ. 10 ) THEN
C        N=9
C        OPTVAL=0.0
C      ELSEIF ( NTEST  .EQ. 11 ) THEN
C        N=10
C        OPTVAL=-47.75997
C      ELSEIF ( NTEST .EQ. 12 ) THEN
C        N=10
C        OPTVAL=24.30621
      ELSE
         IF ( IOUT .GT. 0 ) THEN
            WRITE( IOUT, 2100 )
         END IF
         STOP        
      ENDIF

C
C  CHECK IF THE MAXIMUM ALLOWED DIMENSION IS BIG ENOUGH
C
      IF ( N .GT. NMAX ) THEN
         IF ( IOUT .GT. 0 ) THEN
            WRITE( IOUT, 2000 ) 'X   ', 'NMAX  ', N - NMAX
         END IF
         STOP
      END IF



C
C  NUMBER OF CONSTRAINT (LINEAR AND NONLINEAR) 
C

         NCNLN=0
      IF ( NTEST .EQ. 1 ) THEN
         M=2
         NCLIN=0
      ELSEIF ( NTEST .EQ. 2 ) THEN
         M=5
         NCLIN=3
      ELSEIF ( NTEST .EQ. 3 ) THEN
         M=0
         NCLIN=3
      ELSEIF ( NTEST .EQ. 4 ) THEN
         NCLIN=0
         M=0
      ELSEIF ( NTEST .EQ. 5 ) THEN
         NCLIN=0
         M=3
      ELSEIF ( NTEST .EQ. 6 ) THEN
         NCLIN=0
         M=1
      ELSEIF ( NTEST .EQ. 7 ) THEN
         NCLIN=3
         M=0
      ELSEIF ( NTEST .EQ. 8 ) THEN
         NCLIN=0
         M=3
      ELSEIF ( NTEST .EQ. 9 ) THEN
         NCLIN=0
         M=0
      ELSEIF ( NTEST .EQ. 10 ) THEN
         NCLIN=0
         M=0
      ELSEIF ( NTEST .EQ. 11 ) THEN
         NCLIN=0
         M=1
         NCNLN=2
      ELSEIF ( NTEST .EQ. 12 ) THEN
         M=3
         NCLIN=3
         NCNLN=2
      ENDIF
      LDA = MAX(1, NCLMAX)    
C
C  CHECK THAT THE NUMBER OF CONSTRAINTS DO NOT EXCEED MAXIMUM
C
      IF ( NCLIN .GT. NCLMAX ) THEN
         IF ( IOUT .GT. 0 ) THEN
            WRITE( IOUT, 2000 ) 'ALIN  ', 'NCLMAX  ', NCLIN - NCLMAX
         END IF
         STOP
      END IF

      IF ( NCNLN .GT. NCNMAX ) THEN
         IF ( IOUT .GT. 0 ) THEN
            WRITE( IOUT, 2000 ) 'WRK  ', 'NCLMAX  ', NCNLN - NCNMAX
         END IF
         STOP
      END IF


C
C  SET THE STARTING POINT
C
      IF ( NTEST .EQ. 1 ) THEN
        X(1)=-2.0D0
        X(2)=1.0D0
      ELSEIF ( NTEST .EQ. 2) THEN
        X(1)=2.0D0
        X(2)=3.0D0
        X(3)=5.0D0
        X(4)=5.0D0
        X(5)=1.0D0
        X(6)=2.0D0
        X(7)=7.0D0
        X(8)=3.0D0
        X(9)=6.0D0
        X(10)=10.0D0
      ELSEIF ( NTEST .EQ. 3) THEN
        DO 30 I = 1, N
          X(I)=0.1D0
 30     CONTINUE  
      ELSEIF ( NTEST .EQ. 4) THEN
        X(1)=-1.2D0
        X(2)=1.0D0
      ELSEIF ( NTEST .EQ. 5) THEN
        DO 50 I = 1, N
          X(I)=-2.3D0
 50     CONTINUE        
      ELSEIF ( NTEST .EQ. 6) THEN
        X(1)=-2.6D0
        X(2)=2D0
        X(3)=2D0
      ELSEIF ( NTEST .EQ. 7) THEN
        X(1)=35D0
        X(2)=-31D0
        X(3)=11D0
        X(4)=5D0
        X(5)=-5D0
      ELSEIF ( NTEST .EQ. 8) THEN
        DO 80 I = 1, N
          X(I)=0.1D0
 80     CONTINUE     
      ELSEIF ( NTEST .EQ. 9 ) THEN
        X(1)=0.5
        X(2)=0.5
      ELSEIF ( NTEST .EQ. 10 ) THEN
        X(1)=0.5
        X(2)=0.5
        X(3)=0.5
        X(4)=-0.5
        X(5)=-0.5
        X(6)=-0.5
        X(7)=0.5
        X(8)=-0.5
        X(9)=0.5
      ELSEIF ( NTEST .EQ. 11) THEN
        DO 90 I = 1, N
          X(I)=-2.3D0
 90     CONTINUE  
      ELSEIF ( NTEST .EQ. 12) THEN
        X(1)=2.0D0
        X(2)=3.0D0
        X(3)=5.0D0
        X(4)=5.0D0
        X(5)=1.0D0
        X(6)=2.0D0
        X(7)=7.0D0
        X(8)=3.0D0
        X(9)=6.0D0
        X(10)=10.0D0
      ENDIF

C
C  SET THE BOUNDS
C
      IF ( NTEST .EQ. 1 ) THEN 
        BL(1)=-0.5D0
        BL(2)=-1.0D20
        BL(3)= 0.0D0
        BL(4)= 0.0D0
        BU(1)=0.5D0
        BU(2)=1.0D0
        BU(3)=1.0D20
        BU(4)=1.0D20
      ELSEIF ( NTEST .EQ. 2 ) THEN
        DO 200 I=1,N
          BL(I)=-1.0D20 
          BU(I)=1.0D20 
 200    CONTINUE
          BL(N+1)=-105.0D0
          BL(N+2)=0.0D0
          BL(N+3)=-12.0D0
          BU(N+1)=1.0D20
          BU(N+2)=1.0D20
          BU(N+3)=1.0D20
        DO 210 I = 1, M
          BL(N+NCLIN+I)=0.0D0 
          BU(N+NCLIN+I)=1.0D20 
 210    CONTINUE
      ELSEIF ( NTEST .EQ. 3 ) THEN
        DO 300 I=1,N
          BL(I)=1.0D-6
          BU(I)=1.0D20 
 300    CONTINUE
        BL(11)=2.0D0 
        BU(11)=2.0D0 
        BL(12)=1.0D0 
        BU(12)=1.0D0 
        BL(13)=1.0D0 
        BU(13)=1.0D0 
      ELSEIF ( NTEST .EQ. 4 ) THEN
        BL(1)=-1.0d20
        BL(2)=-1.0d20
        BU(1)=1.0d20
        BU(2)=1.0d20
      ELSEIF ( NTEST .EQ. 5 ) THEN
        DO 500 I=1,N
          BL(I)=-1.0D2
          BU(I)=1.0D2
 500    CONTINUE
        BL(11)=2.0D0 
        BU(11)=2.0D0 
        BL(12)=1.0D0 
        BU(12)=1.0D0 
        BL(13)=1.0D0 
        BU(13)=1.0D0 
      ELSEIF ( NTEST .EQ. 6 ) THEN
        DO 600 I=1,N
          BL(I)=-1.0D20
          BU(I)=1.0D20
 600    CONTINUE
          BL(4)=0.0D0
          BU(4)=0.0D0 
      ELSEIF ( NTEST .EQ. 7 ) THEN
        DO 700 I=1,N
          BL(I)=-1.0D20
          BU(I)=1.0D20
 700    CONTINUE
        DO 710 I=1,N
          BL(I+N)=6.0D0
          BU(I+N)=6.0D0
 710    CONTINUE
      ELSEIF ( NTEST .EQ. 8 ) THEN
        DO 800 I=1,N
          BL(I)=1.0D-6
          BU(I)=1.0D20 
 800    CONTINUE
        BL(11)=2.0D0 
        BU(11)=2.0D0 
        BL(12)=1.0D0 
        BU(12)=1.0D0 
        BL(13)=1.0D0 
        BU(13)=1.0D0 
      ELSEIF ( NTEST .EQ. 9 ) THEN
        BL(1)=-1.0D20
        BU(1)=1.0D20
        BL(2)=-1.0D20
        BU(2)=1.0D20
      ELSEIF ( NTEST .EQ. 10 ) THEN
        BL(1)=-1.0D0
        BU(1)=1.0D0
        BL(2)=-1.0D0
        BU(2)=1.0D0
        BL(3)=-1.0D0
        BU(3)=1.0D0
        BL(4)=-1.0D0
        BU(4)=1.0D0
        BL(5)=-1.0D0
        BU(5)=1.0D0
        BL(6)=-1.0D0
        BU(6)=1.0D0
        BL(7)=-1.0D0
        BU(7)=1.0D0
        BL(8)=-1.0D0
        BU(8)=1.0D0
        BL(9)=-1.0D0
        BU(9)=1.0D0
      ELSEIF ( NTEST .EQ. 11 ) THEN
        DO 900 I=1,N
          BL(I)=-1.0D2
          BU(I)=1.0D2
 900    CONTINUE
        BL(11)=2.0D0 
        BU(11)=2.0D0 
        BL(12)=1.0D0 
        BU(12)=1.0D0 
        BL(13)=1.0D0 
        BU(13)=1.0D0 
      ELSEIF ( NTEST .EQ. 12 ) THEN
        DO 1200 I=1,N
          BL(I)=-1.0D20 
          BU(I)=1.0D20 
 1200   CONTINUE
          BL(N+1)=-105.0D0
          BL(N+2)=0.0D0
          BL(N+3)=-12.0D0
          BU(N+1)=1.0D20
          BU(N+2)=1.0D20
          BU(N+3)=1.0D20
        DO 1210 I = 1, NCNLN+M
          BL(N+NCLIN+I)=0.0D0 
          BU(N+NCLIN+I)=1.0D20 
 1210   CONTINUE
      ENDIF
C
C  MATRIX OF LINEAR CONSTRAINTS
C
      IF ( NTEST .EQ. 1 ) THEN
        CONTINUE
      ELSEIF ( NTEST .EQ. 2 ) THEN
        DO 230 I = 1, N
          DO 240 J = 1, NCLIN
            ALIN(J, I)=0.0D0
 240      CONTINUE
 230    CONTINUE  
        ALIN(1,1)=-4.0D0
        ALIN(1,2)=-5.0D0
        ALIN(1,7)=3.0D0
        ALIN(1,8)=-9.0D0
        ALIN(2,1)=-10.0D0
        ALIN(2,2)=8.0D0
        ALIN(2,7)=17.0D0
        ALIN(2,8)=-2.0D0
        ALIN(3,1)=8.0D0
        ALIN(3,2)=-2.0D0
        ALIN(3,9)=-5.0D0
        ALIN(3,10)=2.0D0
      ELSEIF ( NTEST .EQ. 3 ) THEN
        DO 330 I = 1, N
          DO 340 J = 1, NCLIN
            ALIN(J, I)=0.0D0
 340      CONTINUE
 330    CONTINUE  
        ALIN(1,1)=1.0D0
        ALIN(1,2)=2.0D0
        ALIN(1,3)=2.0D0
        ALIN(1,6)=1.0D0
        ALIN(1,10)=1.0D0
        ALIN(2,4)=1.0D0
        ALIN(2,5)=2.0D0
        ALIN(2,6)=1.0D0
        ALIN(2,7)=1.0D0
        ALIN(3,3)=1.0D0
        ALIN(3,7)=1.0D0
        ALIN(3,8)=1.0D0
        ALIN(3,9)=2.0D0
        ALIN(3,10)=1.0D0
      ELSEIF ( NTEST .EQ. 4 ) THEN
        CONTINUE
      ELSEIF ( NTEST .EQ. 5 ) THEN
        CONTINUE
      ELSEIF ( NTEST .EQ. 6 ) THEN
        CONTINUE
      ELSEIF ( NTEST .EQ. 7 ) THEN
        DO 730 I = 1, N
          DO 740 J = 1, NCLIN
            ALIN(J, I)=0.0D0
 740      CONTINUE
 730    CONTINUE  
        ALIN(1,1)=1.0D0
        ALIN(1,2)=2.0D0
        ALIN(1,3)=3.0D0
        ALIN(2,2)=1.0D0
        ALIN(2,3)=2.0D0
        ALIN(2,4)=3.0D0
        ALIN(3,3)=1.0D0
        ALIN(3,4)=2.0D0
        ALIN(3,5)=3.0D0
      ELSEIF ( NTEST .EQ. 8 ) THEN
        CONTINUE
      ELSEIF ( NTEST .EQ. 9 ) THEN
        CONTINUE
      ELSEIF ( NTEST .EQ. 10 ) THEN
        CONTINUE
      ELSEIF ( NTEST .EQ. 11 ) THEN
        CONTINUE
      ELSEIF ( NTEST .EQ. 12 ) THEN
        DO 830 I = 1, N
          DO 840 J = 1, NCLIN
            ALIN(J, I)=0.0D0
 840      CONTINUE
 830    CONTINUE  
        ALIN(1,1)=-4.0D0
        ALIN(1,2)=-5.0D0
        ALIN(1,7)=3.0D0
        ALIN(1,8)=-9.0D0
        ALIN(2,1)=-10.0D0
        ALIN(2,2)=8.0D0
        ALIN(2,7)=17.0D0
        ALIN(2,8)=-2.0D0
        ALIN(3,1)=8.0D0
        ALIN(3,2)=-2.0D0
        ALIN(3,9)=-5.0D0
        ALIN(3,10)=2.0D0
      ENDIF

C
C  OBTAIN NAMES
C
      CALL NAMES( N, M, PNAME, XNAMES, GNAMES )

C
C  PRINT THE EXPECTED OPTIMAL VALUE
C 
      WRITE(IOUT, 3000) PNAME, OPTVAL

      RETURN

 2000 FORMAT( /, ' ** Program USETUP: array length ', A6, ' too small.',
     *        /, ' -- Miminimization abandoned.',
     *        /, ' -- Increase the parameter ', A6, ' by at least ', I8,
     *           ' and restart.'  )
 2100 FORMAT( /, ' ** Program DFOTES: invalid test problem number;' ,
     *        /, '1 to 8 are valid.')
 3000 FORMAT( ' STARTING TEST PROBLEM ', A10, 
     *        /, ' OPTIMAL VALUE SHOULD BE ABOUT ', D12.4 )
      END


C  ***************************************************
C
C  SUBROUTINE FOR NAMES
C
C  ***************************************************


      SUBROUTINE NAMES( N, M, PNAME, VNAMES, GNAMES )
      INTEGER N, M
      CHARACTER * 256 PNAME, VNAMES( N ), GNAMES( * )
C
C  COMMON VARIABLE FOR THE TEST NUMBER
C
      INTEGER           NTEST
      COMMON  /TEST/    NTEST
      SAVE /TEST/
C
C  SETTING NAMES FOR VARIABLES AND PROBLEM
C
      IF ( NTEST .EQ. 1 ) THEN
        PNAME='HS16'
        VNAMES(1)='X1'
        VNAMES(2)='X2'
        GNAMES(1)='CON1'
        GNAMES(2)='CON2'
      ELSEIF ( NTEST .EQ. 2 ) THEN
        PNAME='HS113'
        VNAMES(1)='X1'
        VNAMES(2)='X2'
        VNAMES(3)='X3'
        VNAMES(4)='X4'
        VNAMES(5)='X5'
        VNAMES(6)='X6'
        VNAMES(7)='X7'
        VNAMES(8)='X8'
        VNAMES(9)='X9'
        VNAMES(10)='X10'
        GNAMES(1)='CON1'
        GNAMES(2)='CON2'
        GNAMES(3)='CON3'
        GNAMES(4)='CON4'
        GNAMES(5)='CON5'
      ELSEIF ( NTEST .EQ. 3 ) THEN
        PNAME='HS112'
        VNAMES(1)='X1'
        VNAMES(2)='X2'
        VNAMES(3)='X3'
        VNAMES(4)='X4'
        VNAMES(5)='X5'
        VNAMES(6)='X6'
        VNAMES(7)='X7'
        VNAMES(8)='X8'
        VNAMES(9)='X9'
        VNAMES(10)='X10'
        GNAMES(1)='CON1'
        GNAMES(2)='CON2'
        GNAMES(3)='CON3'
      ELSEIF ( NTEST .EQ. 4 ) THEN
        PNAME='ROSENBR'
        VNAMES(1)='X1'
        VNAMES(2)='X2'
      ELSEIF ( NTEST .EQ. 5 ) THEN
        PNAME='HS111'
        VNAMES(1)='X1'
        VNAMES(2)='X2'
        VNAMES(3)='X3'
        VNAMES(4)='X4'
        VNAMES(5)='X5'
        VNAMES(6)='X6'
        VNAMES(7)='X7'
        VNAMES(8)='X8'
        VNAMES(9)='X9'
        VNAMES(10)='X10'
        GNAMES(1)='CON1'
        GNAMES(2)='CON2'
        GNAMES(3)='CON3'
      ELSEIF ( NTEST .EQ. 6 ) THEN
        PNAME='HS26'
        VNAMES(1)='X1'
        VNAMES(2)='X2'
        VNAMES(3)='X3'
        GNAMES(1)='CON1'
      ELSEIF ( NTEST .EQ. 7 ) THEN
        PNAME='HS50'
        VNAMES(1)='X1'
        VNAMES(2)='X2'
        VNAMES(3)='X3'
        VNAMES(4)='X4'
        VNAMES(5)='X5'
        GNAMES(1)='CON1'
        GNAMES(2)='CON2'
        GNAMES(3)='CON3'
      ELSEIF ( NTEST .EQ. 8 ) THEN
        PNAME='HS112'
        VNAMES(1)='X1'
        VNAMES(2)='X2'
        VNAMES(3)='X3'
        VNAMES(4)='X4'
        VNAMES(5)='X5'
        VNAMES(6)='X6'
        VNAMES(7)='X7'
        VNAMES(8)='X8'
        VNAMES(9)='X9'
        VNAMES(10)='X10'
        GNAMES(1)='CON1'
        GNAMES(2)='CON2'
        GNAMES(3)='CON3'    
      ELSEIF ( NTEST .EQ. 9 ) THEN
        PNAME='RANDOM-NOISE'
        VNAMES(1)='X1'
        VNAMES(2)='X2'
      ELSEIF ( NTEST .EQ. 10 ) THEN
        PNAME='UGLY-PROBLEM'
        VNAMES(1)='X1'
        VNAMES(2)='X2'
        VNAMES(3)='X3'
        VNAMES(4)='X1'
        VNAMES(5)='X2'
        VNAMES(6)='X3'
        VNAMES(7)='X1'
        VNAMES(8)='X2'
        VNAMES(9)='X3'
      ELSEIF ( NTEST .EQ. 11 ) THEN
        PNAME='HS111'
        VNAMES(1)='X1'
        VNAMES(2)='X2'
        VNAMES(3)='X3'
        VNAMES(4)='X4'
        VNAMES(5)='X5'
        VNAMES(6)='X6'
        VNAMES(7)='X7'
        VNAMES(8)='X8'
        VNAMES(9)='X9'
        VNAMES(10)='X10'
        GNAMES(1)='CON1'
        GNAMES(2)='CON2'
        GNAMES(3)='CON3'
      ELSEIF ( NTEST .EQ. 12 ) THEN
        PNAME='HS113'
        VNAMES(1)='X1'
        VNAMES(2)='X2'
        VNAMES(3)='X3'
        VNAMES(4)='X4'
        VNAMES(5)='X5'
        VNAMES(6)='X6'
        VNAMES(7)='X7'
        VNAMES(8)='X8'
        VNAMES(9)='X9'
        VNAMES(10)='X10'
        GNAMES(1)='CON1'
        GNAMES(2)='CON2'
        GNAMES(3)='CON3'
        GNAMES(4)='CON4'
        GNAMES(5)='CON5'
      ENDIF     
      RETURN
      END


C  ***************************************************
C
C  SUBROUTINE FOR CONSTRAINTS
C
C  ***************************************************
C
C
C      SUBROUTINE FUNEASYCON(MODE, NCNLN, N, NROWJ, NEEDC, X, C,
C     *                  CJAC, NSTATE)   
CC
CC  CONSTRAINTS OF THE 7 TEST PROBLEMS 
CC
C      INTEGER          MODE, NCNLN, N, NROWJ, NEEDC(NCNLN), NSTATE, I, J
C      
C      DOUBLE PRECISION X(N), C(NROWJ), CJAC(NROWJ,N)
CC
CC  COMMON VARIABLE FOR THE TEST NUMBER
CC
C      INTEGER           NTEST
C      COMMON  /TEST/    NTEST
C      IF ( NTEST .EQ. 1 ) THEN 
C        DO 10 I=1, NCNLN
C          IF( NEEDC(I).NE.0 ) THEN
C            IF ( MODE .EQ. 0 .OR. MODE .EQ. 2 )  THEN    
C              IF(I.EQ.1) THEN
C                C(1)=X(1)+X(2)*X(2)
C              ENDIF
C              IF(I.EQ.2) THEN
C                 C(2)=X(1)*X(1)+X(2)
C              ENDIF
C            ENDIF
C            IF ( MODE .EQ. 1 .OR. MODE .EQ. 2 ) THEN
C              IF(I.EQ.1) THEN
C                CJAC(1,1)=1
C                CJAC(1,2)=2*X(2)
C              ENDIF
C              IF(I.EQ.2) THEN
C                CJAC(2,1)=2*X(1)
C                CJAC(2,2)=1      
C              ENDIF
C            ENDIF
C          ENDIF      
C 10     CONTINUE
C      ELSEIF ( NTEST .EQ. 2 ) THEN
C       DO 20 I=1, NCNLN
C          IF( NEEDC(I).NE.0 ) THEN
C            IF ( MODE .EQ. 0 .OR. MODE .EQ. 2 )  THEN    
C              IF(I.EQ.1) THEN
C                C(1)=-3*(X(1)-2)**2-4*(X(2)-3)**2-2*X(3)**2+
C     +                7*X(4)+120
C              ENDIF
C              IF(I.EQ.2) THEN
C                C(2)=-5*X(1)**2-8*X(2)
C     +                - (X(3)-6)**2+2*X(4)+ 40
C              ENDIF
C              IF(I.EQ.3) THEN
C                C(3)=-0.5*(X(1)-8)**2-2*(X(2)-4)**2-3*X(5)**2+
C     +                X(6) + 30
C              ENDIF
C              IF(I.EQ.4) THEN
C                C(4)=-X(1)**2-2*(X(2)-2)**2+2*X(1)*X(2)-
C     +                14*X(5)+6*X(6)
C              ENDIF
C              IF(I.EQ.5) THEN
C                C(5)=3*X(1)-6*X(2)-12*(X(9)-8)**2+7*X(10)
C              ENDIF
C            ENDIF
C            IF ( MODE .EQ. 1 .OR. MODE .EQ. 2 ) THEN
C              DO 25 J = 1, N
C                CJAC(I, J) = 0.0D0
C 25           CONTINUE  
C              IF(I.EQ.1) THEN
C                CJAC(1,1)=-6*X(1)+12D0
C                CJAC(1,2)=-8*X(2)+24D0    
C                CJAC(1,3)=-4*X(3)
C                CJAC(1,4)=7D0    
C              ENDIF
C              IF(I.EQ.2) THEN
C                CJAC(2,1)=-10D0*X(1)
C                CJAC(2,2)=-8D0  
C                CJAC(2,3)=-2D0*X(3)+12
C                CJAC(2,4)=2D0     
C              ENDIF
C              IF(I.EQ.3) THEN
C                CJAC(3,1)=-X(1)+8D0
C                CJAC(3,2)=-4*X(2)+16D0   
C                CJAC(3,5)=-6*X(5)
C                CJAC(3,6)=1D0   
C              ENDIF
C              IF(I.EQ.4) THEN
C                CJAC(4,1)=-2*X(1)+2D0*X(2)
C                CJAC(4,2)=-4*X(2)+8D0+2*X(1)   
C                CJAC(4,5)=-14D0
C                CJAC(4,6)=6D0  
C              ENDIF
C              IF(I.EQ.5) THEN
C                CJAC(5,1)=3D0
C                CJAC(5,2)=-6D0  
C                CJAC(5,9)=-24D0*X(9)+192
C                CJAC(5,10)=7D0   
C              ENDIF
C            ENDIF
C          ENDIF      
C 20     CONTINUE        
C      ELSEIF ( NTEST .EQ. 3 ) THEN
C        CONTINUE
C      ELSEIF ( NTEST .EQ. 4 ) THEN
C        CONTINUE
C      ELSEIF ( NTEST .EQ. 5 ) THEN
C        CONTINUE
C      ELSEIF ( NTEST .EQ. 6 ) THEN
C        IF( NEEDC(1).NE.0 ) THEN
C          IF ( MODE .EQ. 0 .OR. MODE .EQ. 2 )  THEN    
C                C(1)=(1D0+X(2)**2)*X(1)+X(3)**4-3
C          ENDIF
C          IF ( MODE .EQ. 1 .OR. MODE .EQ. 2 ) THEN
C            CJAC(1,1)=1D0+X(2)**2
C            CJAC(1,2)=2D0*X(1)*X(2)
C            CJAC(1,3)=4*X(3)**3
C          ENDIF
C        ENDIF
C      ELSEIF ( NTEST .EQ. 7 ) THEN 
C        CONTINUE
C      ELSEIF ( NTEST .EQ. 8 ) THEN
C        DO 80 I=1, NCNLN
C          IF( NEEDC(I).NE.0 ) THEN
C            IF ( MODE .EQ. 0 .OR. MODE .EQ. 2 )  THEN    
C              IF(I.EQ.1) THEN
C                C(1)=X(1)+2*X(2)+2*X(3)+X(6)**2+X(10)
C              ENDIF
C              IF(I.EQ.2) THEN
C                C(2)=X(4)+2*X(5)+X(6)**2+X(7)
C              ENDIF
C              IF(I.EQ.3) THEN
C                C(3)=X(3)+X(7)+X(8)+2*X(9)+X(10)
C              ENDIF
C            ENDIF
C            IF ( MODE .EQ. 1 .OR. MODE .EQ. 2 ) THEN
C              DO 85 J = 1, N
C                CJAC(I, J) = 0.0D0
C 85           CONTINUE  
C              IF(I.EQ.1) THEN
C                CJAC(1,1)=1.0d0
C                CJAC(1,2)=2.0d0    
C                CJAC(1,3)=2.0d0
C                CJAC(1,6)=2.0d0*X(6)
C                CJAC(1,10)=1.0d0    
C              ENDIF
C              IF(I.EQ.2) THEN
C                CJAC(2,4)=1.0d0
C                CJAC(2,5)=2.0d0  
C                CJAC(2,6)=2.0d0*X(6)
C                CJAC(2,7)=1.0d0   
C              ENDIF
C              IF(I.EQ.3) THEN
C                CJAC(3,3)=1.0d0
C                CJAC(3,7)=1.0d0 
C                CJAC(3,8)=1.0d0
C                CJAC(3,9)=2.0d0
C                CJAC(3,10)=1.0d0 
C              ENDIF
C            ENDIF
C          ENDIF      
C 80     CONTINUE 
C      ELSEIF ( NTEST .EQ. 9 ) THEN
C        CONTINUE
C      ELSEIF ( NTEST .EQ. 10 ) THEN
C        CONTINUE
C      ELSEIF ( NTEST .EQ. 11 ) THEN
C        DO 50 I=1, NCNLN
C          IF( NEEDC(I).NE.0 ) THEN
C            IF ( MODE .EQ. 0 .OR. MODE .EQ. 2 )  THEN    
C              IF(I.EQ.1) THEN
C                C(1)=EXP(X(1))+2*EXP(X(2))+2*EXP(X(3))+EXP(X(6))+
C     +               EXP(X(10))
C              ENDIF
C              IF(I.EQ.2) THEN
C                C(2)=EXP(X(4))+2*EXP(X(5))+EXP(X(6))+EXP(X(7))
C              ENDIF
C              IF(I.EQ.3) THEN
C                C(3)=EXP(X(3))+EXP(X(7))+EXP(X(8))+2*EXP(X(9))+
C     +               EXP(X(10))
C              ENDIF
C            ENDIF
C            IF ( MODE .EQ. 1 .OR. MODE .EQ. 2 ) THEN
C              DO 55 J = 1, N
C                CJAC(I, J) = 0.0D0
C 55           CONTINUE  
C              IF(I.EQ.1) THEN
C                CJAC(1,1)=EXP(X(1))
C                CJAC(1,2)=2*EXP(X(2))    
C                CJAC(1,3)=2*EXP(X(3))
C                CJAC(1,6)=EXP(X(6))
C                CJAC(1,10)=EXP(X(10))    
C              ENDIF
C              IF(I.EQ.2) THEN
C                CJAC(2,4)=EXP(X(4))
C                CJAC(2,5)=2*EXP(X(5))  
C                CJAC(2,6)=EXP(X(6))
C                CJAC(2,7)=EXP(X(7))   
C             ENDIF
C              IF(I.EQ.3) THEN
C                CJAC(3,3)=EXP(X(3))
C                CJAC(3,7)=EXP(X(7))  
C                CJAC(3,8)=EXP(X(8))
C                CJAC(3,9)=2*EXP(X(9))
C                CJAC(3,10)=EXP(X(10)) 
C              ENDIF
C            ENDIF
C          ENDIF      
C 50     CONTINUE
C      ENDIF
C      RETURN
C      END



C  *********************************************************
C
C  ROUTINE COMPUTING FUNCTION VALUE
C
C  *********************************************************
      SUBROUTINE FUN(N, M, X, VAL, C, IFERR)
 
C
C  OBJECTIVE FUNCTION FOR 7  TEST PROBLEM FOR DFO
C
C     
      INTEGER N, M, I
      DOUBLE PRECISION X(N), VAL, C(M), SUM
      LOGICAL          IFERR
C
C  COMMON VARIABLE FOR THE TEST NUMBER
C
      INTEGER           NTEST
      COMMON  /TEST/    NTEST
      SAVE /TEST/
C
C  LOCAL VARIABLES
C
      DOUBLE PRECISION CHS112(10)
      REAL             GENNOR
      EXTERNAL         GENNOR

      INTRINSIC        LOG, EXP, SQRT

      IFERR=.FALSE.
      IF ( NTEST .EQ. 1 ) THEN 
        VAL = 100.0d0*(X(2)-X(1)**2)**2+(1.0d0-X(1))**2
        C(1)=X(1)+X(2)*X(2)
        C(2)=X(1)*X(1)+X(2)
      ELSEIF ( NTEST .EQ. 2 ) THEN
        VAL = X(1)**2 + X(2)**2 + X(1)*X(2) - 14D0*X(1) - 16D0*X(2)+
     +        (X(3)-10)**2 + 4D0*(X(4)-5D0)**2+(X(5)-3D0)**2+
     +        2.0D0*(X(6)-1D0)**2+
     +        5.0D0*X(7)**2+7.0D0*(X(8)-11D0)**2+2D0*(X(9)-10D0)**2+
     +        (X(10)-7)**2+45D0
                C(1)=-3*(X(1)-2)**2-4*(X(2)-3)**2-2*X(3)**2+
     +                7*X(4)+120
                C(2)=-5*X(1)**2-8*X(2)
     +                - (X(3)-6)**2+2*X(4)+ 40
                C(3)=-0.5*(X(1)-8)**2-2*(X(2)-4)**2-3*X(5)**2+
     +                X(6) + 30
                C(4)=-X(1)**2-2*(X(2)-2)**2+2*X(1)*X(2)-
     +                14*X(5)+6*X(6)
                C(5)=3*X(1)-6*X(2)-12*(X(9)-8)**2+7*X(10)
      ELSEIF ( NTEST .EQ. 3 ) THEN
        CHS112(1)=-6.089D0
        CHS112(2)=-17.164D0
        CHS112(3)=-34.054D0
        CHS112(4)=-5.914D0
        CHS112(5)=-24.721D0
        CHS112(6)=-14.986D0
        CHS112(7)=-24.100D0
        CHS112(8)=-10.708D0
        CHS112(9)=-26.662D0
        CHS112(10)=-22.179D0
        SUM = 0.0D0
        VAL = 0.0D0
        DO 30 I=1, N
          SUM = SUM + X(I)
 30     CONTINUE
        DO 31 I=1, N  
          VAL = VAL + X(I)*(CHS112(I)+LOG(X(I)/SUM))
 31     CONTINUE  
      ELSEIF ( NTEST .EQ. 4 ) THEN
        VAL = 100.0d0*(X(2)-X(1)**2)**2+(1.0d0-X(1))**2        
      ELSEIF ( NTEST .EQ. 5 ) THEN
        CHS112(1)=-6.089D0
        CHS112(2)=-17.164D0
        CHS112(3)=-34.054D0
        CHS112(4)=-5.914D0
        CHS112(5)=-24.721D0
        CHS112(6)=-14.986D0
        CHS112(7)=-24.100D0
        CHS112(8)=-10.708D0
        CHS112(9)=-26.662D0
        CHS112(10)=-22.179D0
        SUM = 0.0D0
        VAL = 0.0D0
        DO 50 I=1, N
          SUM = SUM + EXP(X(I))
 50     CONTINUE
        DO 51 I=1, N  
          VAL = VAL + EXP(X(I))*(CHS112(I)+X(I)-LOG(SUM))
 51     CONTINUE  
                C(1)=EXP(X(1))+2*EXP(X(2))+2*EXP(X(3))+EXP(X(6))+
     +               EXP(X(10))
                C(2)=EXP(X(4))+2*EXP(X(5))+EXP(X(6))+EXP(X(7))
                C(3)=EXP(X(3))+EXP(X(7))+EXP(X(8))+2*EXP(X(9))+
     +               EXP(X(10))
      ELSEIF ( NTEST .EQ. 6 ) THEN
        VAL = (X(1)-X(2))**2+(X(2)-X(3))**4
        C(1)=(1D0+X(2)**2)*X(1)+X(3)**4-3
      ELSEIF ( NTEST .EQ. 7 ) THEN
        VAL = (X(1)-X(2))**2+(X(2)-X(3))**2+(X(3)-X(4))**4+
     +        (X(4)-X(5))**2
      ELSEIF ( NTEST .EQ. 8 ) THEN
        CHS112(1)=-6.089D0
        CHS112(2)=-17.164D0
        CHS112(3)=-34.054D0
        CHS112(4)=-5.914D0
        CHS112(5)=-24.721D0
        CHS112(6)=-14.986D0
        CHS112(7)=-24.100D0
        CHS112(8)=-10.708D0
        CHS112(9)=-26.662D0
        CHS112(10)=-22.179D0
        SUM = 0.0D0
        VAL = 0.0D0
        DO 80 I=1, N
          SUM = SUM + X(I)
 80     CONTINUE
        DO 81 I=1, N  
          VAL = VAL + X(I)*(CHS112(I)+LOG(X(I)/SUM))
 81     CONTINUE  
                C(1)=X(1)+2*X(2)+2*X(3)+X(6)**2+X(10)
                C(2)=X(4)+2*X(5)+X(6)**2+X(7)
                C(3)=X(3)+X(7)+X(8)+2*X(9)+X(10)
      ELSEIF ( NTEST .EQ. 9 ) THEN
c        val=gennor(0.0, 0.0001)
        VAL=X(1)**2+X(2)**2
      ELSEIF ( NTEST .EQ. 10 ) THEN
        VAL=SQRT((X(1)-X(4))**2+(X(2)-X(5))**2+(X(3)-X(6))**2)
        SUM=EXP((-12.0D0)*LOG(VAL))-2.0D0*EXP((-6.0D0)*LOG(VAL))
        VAL=SQRT((X(1)-X(7))**2+(X(2)-X(8))**2+(X(3)-X(9))**2)
        SUM=SUM+EXP((-12.0D0)*LOG(VAL))-2.0D0*EXP((-6.0D0)*LOG(VAL))
        VAL=SQRT((X(7)-X(4))**2+(X(8)-X(5))**2+(X(9)-X(6))**2)
        SUM=SUM+EXP((-12.0D0)*LOG(VAL))-2.0D0*EXP((-6.0D0)*LOG(VAL))
        VAL=SUM
      ELSEIF ( NTEST .EQ. 11 ) THEN
        CHS112(1)=-6.089D0
        CHS112(2)=-17.164D0
        CHS112(3)=-34.054D0
        CHS112(4)=-5.914D0
        CHS112(5)=-24.721D0
        CHS112(6)=-14.986D0
        CHS112(7)=-24.100D0
        CHS112(8)=-10.708D0
        CHS112(9)=-26.662D0
        CHS112(10)=-22.179D0
        SUM = 0.0D0
        VAL = 0.0D0
        DO 90 I=1, N
          SUM = SUM + EXP(X(I))
 90     CONTINUE
        DO 91 I=1, N  
          VAL = VAL + EXP(X(I))*(CHS112(I)+X(I)-LOG(SUM))
 91     CONTINUE  
c                C(1)=EXP(X(1))+2*EXP(X(2))+2*EXP(X(3))+EXP(X(6))+
c     +               EXP(X(10))
c                C(2)=EXP(X(4))+2*EXP(X(5))+EXP(X(6))+EXP(X(7))
                C(1)=EXP(X(3))+EXP(X(7))+EXP(X(8))+2*EXP(X(9))+
     +               EXP(X(10))
      ELSEIF ( NTEST .EQ. 12 ) THEN
        VAL = X(1)**2 + X(2)**2 + X(1)*X(2) - 14D0*X(1) - 16D0*X(2)+
     +        (X(3)-10)**2 + 4D0*(X(4)-5D0)**2+(X(5)-3D0)**2+
     +        2.0D0*(X(6)-1D0)**2+
     +        5.0D0*X(7)**2+7.0D0*(X(8)-11D0)**2+2D0*(X(9)-10D0)**2+
     +        (X(10)-7)**2+45D0
C                C(1)=-3*(X(1)-2)**2-4*(X(2)-3)**2-2*X(3)**2+
C     +                7*X(4)+120
C                C(2)=-5*X(1)**2-8*X(2)
C     +                - (X(3)-6)**2+2*X(4)+ 40
                C(1)=-0.5*(X(1)-8)**2-2*(X(2)-4)**2-3*X(5)**2+
     +                X(6) + 30
                C(2)=-X(1)**2-2*(X(2)-2)**2+2*X(1)*X(2)-
     +                14*X(5)+6*X(6)
                C(3)=3*X(1)-6*X(2)-12*(X(9)-8)**2+7*X(10)
      ENDIF     
      RETURN
      END


