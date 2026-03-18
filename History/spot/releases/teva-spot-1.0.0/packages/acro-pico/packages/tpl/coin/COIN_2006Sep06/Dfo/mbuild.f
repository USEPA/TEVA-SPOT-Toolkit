      SUBROUTINE MDBLD( KAPPA , G   , H   , N    , NIND , PNTINT,  
     +                  VALINT, POLY, BASE, VARNT, LPOLY, LPTINT,  
     +                  NEQCON, WRK , LWRK, IWRK , LIWRK )  


*    
*  ***********************************************************************
*  THIS FUNCTION BUILDS A QUADRATIC INTERPOLATION
*  FUNCTION FOR THE SUPPLIED 'NIND' DATA POINTS THAT ARE
*  GIVEN IN ARRAY 'PNTINT' WITH VALUES IN ARRAY 'VALINT'.
*
*                     T         T
*      M(X)= KAPPA + G X + 0.5*X H X
*
*  IF 'VARNT' EQUALS 1 AND THE MODEL IS INCOMPLETE QUADRATIC
*  THEN MINIMUM FROBENIUS MODEL IS BUILT, OTHERWISE THE MODEL
*  IS BUILT FROM A LINEAR COMBINATION OF CONSTRUCTED NEWTON 
*  FUNDAMENTAL POLYNOMIALS. 
*
*  PARAMETERS:
*  
*   N      (INPUT)  DIMENSION OF THE PROBLEM
* 
*   NIND   (INPUT)  NUMBER OF INTERPOLATION POINTS
*
*   PNTINT (INPUT)  LIST OF  'NIND' DATA POINTS. THE I-TH POINT OCCUPIES
*                   POSITIONS ( I - 1 ) * N + 1 TO I * N.
*   VALINT (INPUT)  VALUES OF THE OBJECTIVE FUNCTION AT THE 'NIND'
*                   DATA POINTS CONTAINED IN PNTINT.
*   POLY   (INPUT)  THE ARRAY CONTAINING COEFFICIENTS OF NEWTON FUNDAMENTAL
*                   POLYNOMIALS (AS COMPUTED BY 'NBUILD')
*   BASE   (INPUT)  THE INDEX (IN PNTINT) OF THE CURRENT BASE (BEST)  POINT.
*
*   VARNT  (INPUT)  VARNT=1 -- FROBENIUS MODEL IS BUILT, VARNT=2 -- SUBQUADRATIC
*                   MODEL IS BUILT                    
*   KAPPA  (OUTPUT) THE CONSTANT TERM OF THE INTERPOLATION MODEL.
*
*   G      (OUTPUT) VECTOR OF THE LINEAR TERMS OF THE  INTERPOLATION MODEL.
*
*   H      (OUTPUT) MATRIX OF QUADRATIC TERMS OF THE  INTERPOLATION MODEL.
*
*   WRK    (INPUT)  REAL WORKSPACE OF LENGTH LWRK.
*
*   IWRK   (INPUT)  INTEGER WORKSPACE OF LENGTH LIWRK.
*
*  **************************************************************************
*

      INTEGER          N,       NIND, LWRK  , LIWRK, BASE, NEQCON,
     +                 IWRK( LIWRK ), LPTINT, LPOLY, VARNT



      DOUBLE PRECISION PNTINT( LPTINT ), KAPPA, G( N ), H( N, N ),
     +                 VALINT( NIND )  , WRK( LWRK )  , POLY(LPOLY)



      INTEGER          IOUT, IPRINT

      DOUBLE PRECISION MCHEPS, CNSTOL

      COMMON /DFOCM/   IOUT, IPRINT, MCHEPS, CNSTOL
      SAVE /DFOCM/
*
*  SUBROUTINES AND FUNCTIONS CALLED:  
*
*  APPLICATION:  MBLDMF, MBLDNP 
*


*
*  LOCAL VARIABLES
*

      INTEGER          ILAFLA, IMATR, IX, IVX, LENW, ICURW, DD
 
      LOGICAL          FROBEN, FAIL


      DD=(N+1)*(N+2)/2


*
*  SEE IF FROBENIUS MODEL SHOULD BE COMPUTED
*

      FROBEN=.FALSE.
      IF (VARNT.EQ.1 .AND. NIND .GT. N+1 .AND. NIND.LT.DD) FROBEN=.TRUE.


*
*  PARTITION REAL WORKSPACE
*

      IF ( FROBEN ) THEN
        IMATR  = 1
        IX     = IMATR+NIND**2
        IVX    = IX+(NIND-1)*N
        ICURW  = IVX+NIND-1
        LENW   = LWRK-ICURW+1
      ELSE 
        ILAFLA = 1
        ICURW  = ILAFLA+NIND
        LENW   = LWRK-ICURW+1
      ENDIF

*
*  CHECK IF THE WORKING SPACE IS SUFFICIENT
*

      IF ( LENW .LT. 1 ) THEN
         IF ( IPRINT .GE. 0 ) WRITE( IOUT, 1100 ) -LENW+1
         STOP
      ENDIF

      IF ( LIWRK .LT. 1 ) THEN
         IF ( IPRINT .GE. 0 )  WRITE( IOUT, 1200 ) -LIWRK+1
         STOP
      ENDIF

      FAIL = .TRUE.
C
C  IF APPLICABLE, BUILD MINIMUM FROBENIUS MODEL
C

      IF ( FROBEN ) THEN
        CALL MBLDMF( KAPPA  , G ,   H , PNTINT    , VALINT    , BASE,
     +               WRK(IX), WRK(IVX), WRK(IMATR), N         , NIND,    
     +               NEQCON , LPTINT  , FAIL      , WRK(ICURW), LENW,
     +               IWRK   , LIWRK   )
      ENDIF



C
C  IF MINIMUM FROBENIUS MODEL WAS NOT BUILT, THEN BUILD MODEL BASED ON 
C  NEWTON FUNDAMENTAL POLYNOMIALS
C
      
      IF ( FAIL ) THEN

        ILAFLA = 1
        CALL MBLDNP( KAPPA  ,  G,  H, PNTINT, VALINT, POLY  , 
     +               WRK(ILAFLA),  N, NIND  , NEQCON, LPTINT, LPOLY )

      ENDIF          
             
      RETURN
*
*  NON-EXECUTABLE STATEMENTS
*

 1100  FORMAT( ' MDBLD: *** ERROR: LWRK TOO SMALL!' /
     +        '             IT SHOULD BE AT LEAST ',I5 )
 1200  FORMAT( ' MDBLD: *** ERROR: LIWRK TOO SMALL!' /
     +        '             IT SHOULD BE AT LEAST ', I5 )
      END
*


********************************************************************************

*    NEXT SUBROUTINE

********************************************************************************

      SUBROUTINE MBLDNP( KAPPA, G, H   , PNTINT, VALINT, POLY, 
     *                   LAFLA, N, NIND, NEQCON, LPTINT, LPOLY )



*    
*  ***********************************************************************
*  THIS FUNCTION BUILDS A QUADRATIC INTERPOLATION
*  FUNCTION FOR THE SUPPLIED 'NIND' DATA POINTS THAT ARE
*  GIVEN IN ARRAY 'PNTINT' WITH VALUES IN ARRAY 'VALINT'.
*
*                     T         T
*      M(X)= KAPPA + G X + 0.5*X H X
*
*  THE MODEL  IS BUILT AS A LINEAR COMBINATION OF 'NIND'  NEWTON 
*  FUNDAMENTAL POLYNOMIALS STORED IN ARRAY 'POLY'. 
*
*  PARAMETERS:
*  
*   N      (INPUT)  DIMENSION OF THE PROBLEM
*
*   NIND   (INPUT)  NUMBER OF INTERPOLATION POINTS
* 
*   PNTINT (INPUT)  LIST OF  'NIND' DATA POINTS. THE I-TH POINT OCCUPIES
*                   POSITIONS ( I - 1 ) * N + 1 TO I * N.
*   VALINT (INPUT)  VALUES OF THE OBJECTIVE FUNCTION AT THE 'NIND'
*                   DATA POINTS CONTAINED IN PNTINT.
*   POLY   (INPUT)  THE ARRAY CONTAINING COEFFICIENTS OF NEWTON FUNDAMENTAL
*                   POLYNOMIALS (AS COMPUTED BY 'NBUILD')
*   LAFLA           AN AUXILIARY ARRAY OF LENGTH 'N' FOR FINITE DIFFERENCES
*
*   NEQCON (INPUT)  NUMBER OF LINEARLY INDEP. EQUALITY CONSTRAINTS
* 
*   KAPPA  (OUTPUT) THE CONSTANT TERM OF THE INTERPOLATION MODEL.
*
*   G      (OUTPUT) VECTOR OF THE LINEAR TERMS OF THE  INTERPOLATION MODEL.
*
*   H      (OUTPUT) MATRIX OF QUADRATIC TERMS OF THE  INTERPOLATION MODEL.
*
*  **************************************************************************
*


       INTEGER           NIND, N, NEQCON, LPTINT, LPOLY

       DOUBLE PRECISION LAFLA(NIND) , POLY(LPOLY), PNTINT(LPTINT),
     *                  VALINT(NIND), KAPPA, G(N), H(N,N)
                       

*
*  SUBROUTINES AND FUNCTIONS CALLED:
*
*    APPLICATION:       FD,  GTERMS,  
*    BLAS       :       DCOPY
*

      

*
*  COMPUTE FINITE DIFFERENCES FROM NEWTON POLYNOMIALS, POINTS AND VALUES
*

          CALL FD(LAFLA, PNTINT, VALINT, POLY, N, NIND, NEQCON, LPTINT, 
     +            LPOLY)

*
*  COMPUTE TERMS OF THE MODEL FROM FINITE DIFFERENCES, PNTINT AND NEWTON
*  POLYNOMIALS
*

          CALL MTERMS(KAPPA , G   , H, LAFLA , POLY , 
     +                PNTINT, NIND, N, NEQCON, LPOLY, LPTINT)

       

        RETURN
        END






********************************************************************************

*    NEXT SUBROUTINE

********************************************************************************




       SUBROUTINE MBLDMF( KAPPA, G, H, PNTINT, VALINT, BASE, Y   , VY  ,
     +                    MATR , N, Q, NEQCON, LPNT  , FAIL, WRK , LWRK, 
     +                    IWRK , LIWRK )




*    
*  ***********************************************************************
*  THIS FUNCTION BUILDS A QUADRATIC INTERPOLATION
*  FUNCTION FOR THE SUPPLIED 'Q' DATA POINTS THAT ARE
*  GIVEN IN ARRAY 'PNTINT' WITH VALUES IN ARRAY 'VALINT'.
*
*                     T         T
*      M(X)= KAPPA + G X + 0.5*X H X
*
*  THE MODEL IS INCOMPLETE QUADRATIC ( 'Q' IS LESS THAN (N+1)(N+2)/2 )
*  AND 'H' HAS THE SMALLEST  FROBENIUS NORM AMOUNG ALL HESSIANS OF ALL 
*  POSSIBLE QUADRATIC MODELS   SATISFYING THE INTERPOLATION CONDITION
*  FOR 'PNTINT' AND 'VALINT'.
*
*  PARAMETERS:
*  
*   N      (INPUT)  DIMENSION OF THE PROBLEM
* 
*   Q      (INPUT)  NUMBER OF INTERPOLATION POINTS
*
*   PNTINT (INPUT)  LIST OF  'NIND' DATA POINTS. THE I-TH POINT OCCUPIES
*                   POSITIONS ( I - 1 ) * N + 1 TO I * N.
*   VALINT (INPUT)  VALUES OF THE OBJECTIVE FUNCTION AT THE 'NIND'
*                   DATA POINTS CONTAINED IN PNTINT.
*   BASE   (INPUT)  THE INDEX (IN PNTINT) OF THE CURRENT BASE (BEST) POINT.
* 
*   KAPPA  (OUTPUT) THE CONSTANT TERM OF THE INTERPOLATION MODEL.
*
*   G      (OUTPUT) VECTOR OF THE LINEAR TERMS OF THE  INTERPOLATION MODEL.
*
*   H      (OUTPUT) MATRIX OF QUADRATIC TERMS OF THE  INTERPOLATION  MODEL.
*
*  FAIL    (OUTPUT) INDICATES IF THE THE SUBROUTINE HAS FAILED.
*  
*   Y               AUXILIARY ARRAY OF SHIFTED POINTS
*
*   VY              AUXILIARY ARRAY OF SHIFTED VALUES
*
*   MATR            AUXILIARY MATRIX QxQ
*
*   WRK    (INPUT)  REAL WORKSPACE OF LENGTH LWRK.
*
*   IWRK   (INPUT)  INTEGER WORKSPACE OF LENGTH LIWRK.
*
*  **************************************************************************
*


      INTEGER          N, Q, LPNT, BASE, LWRK, IWRK(LWRK), LIWRK, NEQCON

      DOUBLE PRECISION G(N), H(N,N), KAPPA, PNTINT(LPNT), VALINT(Q),
     *                 MATR(Q,Q)   , WRK(LIWRK), Y(LPNT-N), VY(Q-1)
      LOGICAL          FAIL

      DOUBLE PRECISION DDOT

      EXTERNAL         DDOT

      INTRINSIC        ABS

      INTEGER          IOUT, IPRINT

      DOUBLE PRECISION MCHEPS, CNSTOL

      COMMON /DFOCM/   IOUT, IPRINT, MCHEPS, CNSTOL

   
*
*  LOCAL VARIABLES
*

      DOUBLE PRECISION ONE, HALF, ZERO

      PARAMETER      ( ONE = 1.0D0,  HALF=0.5D0, ZERO=0.0D0 )

      DOUBLE PRECISION ANORM, RCOND

      INTEGER          I, J, KK, NY, INFO, JBASE 

*
*  SUBROUTINES AND FUNCTIONS CALLED:
*
*    APPLICATION:       RZRMAT, RZRVEC 
*    LAPACK     :       DPOTRF, DPOCON, DTRSV,   
*    BLAS       :       DCOPY , DAXPY , DGER , DGEMV
*    FORTRAN    :       ABS
*



      FAIL = .FALSE.
*
*  CHECK SUFFICIENCY OF THE WORKSPACE
*    

      IF (LWRK. LT. 3*Q) THEN
        IF ( IPRINT .GE. 0 ) WRITE(IOUT, 1000) 3*Q
        STOP
      ENDIF

      IF (LIWRK. LT. Q) THEN
        IF ( IPRINT .GE. 0 ) WRITE(IOUT, 2000) Q
        STOP
      ENDIF 

      
      IF ( IPRINT .GE. 3 )   WRITE(IOUT, 8000)

*
*  SHIFT ALL THE POINT SO THAT THE BASE IS AT THE ORIGIN AND PUT THE
*  POINTS, EXCEPT THE BASE, IN Y, AND THEIR VALUES IN VY. IGNORE THE
*  DUMMY POINTS IF THERE ARE ANY
*

       JBASE=(BASE-1)*N
       NY=0
       DO 5 I = 1,Q
         IF (I .NE. BASE .AND. (I .LE. N+1-NEQCON .OR. I.GT.N+1) ) THEN
           DO 4 J=1,N
             Y(NY*N+J) = PNTINT((I-1)*N+J)-PNTINT(JBASE+J) 
 4         CONTINUE
           NY=NY+1
         ENDIF 
 5     CONTINUE

*
*  FORM THE MATRIX 'MATR' BY FORMULAS: IF P IS THE MATRIX WITH
*  THE INTERPOLATION POINT AS THE COLUMNS THEN
*
*  *******************************
*  *          T            2     *
*  *     M = P P,   M=0.5*M +M   *
*  *                             *
*  *******************************

       DO 20 I=1,NY
         DO 10 J=1,NY
          MATR(I,J)=DDOT( N, Y((I-1)*N+1), 1, Y((J-1)*N+1), 1 )
 10      CONTINUE
 20    CONTINUE

       DO 40 I=1,NY
         DO 30 J=1,NY
          MATR(I,J)=HALF*MATR(I,J)**2 + MATR(I,J)
 30     CONTINUE
 40    CONTINUE


*
*  USING LAPACK ROUTINES FIND AN ESTIMATE OF THE CONDITION NUMBER OF 'MATR'
*  'MATR' IS SYMMETRIC, SO, FIRST, COMPUTE THE LL^T FACTORIZATION IN 'DPOTRF'
*  IF FACTORIZATION FAILS, THEN PRINT A MESSAGE AND QUIT
*   
 


       CALL DPOTRF('U', NY, MATR, Q, INFO)


       IF ( INFO .NE. 0 ) THEN
         IF( IPRINT.GT.2 ) WRITE(IOUT, 4000) 
         FAIL = .TRUE.
         RETURN
       ENDIF
       
       ANORM = ZERO
       DO 46 I = 1, NY
         DO 45 J = I, NY
           ANORM=ANORM+ABS(MATR(I,J))
 45      CONTINUE 
 46    CONTINUE  

*
*  FIND THE ESTIMATE OF THE COND. NUMBER, IF FAIL, THEN QUIT
*
       CALL DPOCON('U' , NY  , MATR, Q , ANORM, RCOND, 
     +              WRK, IWRK, INFO)

       IF ( INFO .NE. 0 ) THEN
         IF( IPRINT.GT.2 ) WRITE(IOUT, 5000)  
         FAIL = .TRUE.
         RETURN
       ENDIF
       IF ( IPRINT .GE. 3 )   WRITE(IOUT, 8010) RCOND
*
*  CHECK IF THE CONDITION NUMBER IS NOT TOO BIG
*  IF NOT, THEN FORM THE RIGHT HAND SIDE FOR THE LINEAR SYSTEM
*  AND SOLVE THE SYSTEM USING LAPACK ROUTINES AND FACTORIZATION
*  COMPUTED BY 'DPOTRF'. STORE THE SOLUTION IN 'VY'
*
       IF (RCOND. GE. MCHEPS*1.0D4) THEN
         KK=0
         DO 50 I = 1,Q
           IF (I.NE.BASE .AND. (I .LE. N+1-NEQCON .OR. I.GT.N+1)) THEN
             KK=KK+1
             VY(KK) = VALINT(I)-VALINT(BASE) 
           ENDIF
 50      CONTINUE
         CALL DTRSV('U', 'T', 'N', NY, MATR, Q, VY, 1)
         CALL DTRSV('U', 'N', 'N', NY, MATR, Q, VY, 1)


*
*  FIND THE COEFFICIENT OF THE SHIFTED MODEL BY FORMULAS:
*
*  ************************************ 
*  *     G <- 0,  H <- 0                *
*  *     FOR ALL POINTS P_i IN 'Y'    *
*  *                              T   *
*  *       H <- H + VY(I)*[P_i(P_i) ]  *
*  *                                  *
*  *       G <- G + VY(I)*P_i          *
*  *                                  *
*  ************************************
*

         CALL RZRVEC(G, N)
         CALL RZRMAT(H, N, N)
 
         DO 60 I=1, NY
           CALL DAXPY(N,  VY(I), Y((I-1)*N+1),1, G, 1)
           CALL DGER(N, N, VY(I), Y((I-1)*N+1), 1, Y((I-1)*N+1),
     +               1, H, N)
 60      CONTINUE

*
*  SHIFT THE MODEL BACK TO INTERPOLATE THE ORIGINAL POINTS AND FIND  KAPPA
* 
*  ************************************
*  *  G <- G - H*P_base                *
*  *                               T  *
*  *  KAPPA <- VALINT(BASE)-(P_base) G *
*  ************************************
*
         CALL DGEMV('N',N, N, -ONE, H, N, PNTINT(JBASE+1), 1, ONE, G ,1)
         CALL DCOPY( N, G, 1, WRK, 1 )
         CALL DGEMV('N',N, N, HALF, H, N, PNTINT(JBASE+1), 1, ONE,WRK,1)
         KAPPA=VALINT(BASE)- DDOT(N, WRK, 1, PNTINT(JBASE+1), 1)

       ELSE 
*
*  IF THE CONDITION NUMBER IS TOO LARGE, THEN STOP, SINCE IT SHOULD NOT
*  BE HAPPENING
*
         
         IF (IPRINT.GT.2) WRITE (IOUT,3000) RCOND
         FAIL = .TRUE.
       ENDIF
       RETURN

 1000 FORMAT( ' MBLDMF: *** ERROR: LWRK TOO SMALL !' /
     +        '             IT SHOULD BE AT LEAST ',I5 )
 2000 FORMAT( ' MBLDMF: *** ERROR: LIWRK TOO SMALL !' /
     +        '             IT SHOULD BE AT LEAST ', I5 )

 3000 FORMAT(' MINIMUM FROBENIUS NORM MODEL FAILED!' /
     +        ' TOO ILL-CONDITIONED',  D14.7, /)
 4000 FORMAT(' MINIMUM FROBENIUS NORM MODEL FAILED!' /
     +        ' FACTORIZATION CANNOT BE COMPUTED' )
 5000 FORMAT(' MINIMUM FROBENIUS NORM MODEL FAILED!' /
     +        ' CONDITION NUMBER CANNOT BE COMPUTED' )
 8000 FORMAT( ' MBLDMF: Minimum frobenius norm model computed ',/)
 8010 FORMAT( ' MBLDMF: Condition number: ', D14.7,/ )  
       END









