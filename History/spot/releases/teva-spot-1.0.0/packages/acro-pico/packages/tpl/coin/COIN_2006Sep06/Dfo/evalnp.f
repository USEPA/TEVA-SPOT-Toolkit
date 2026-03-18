       SUBROUTINE EVALNP(VAL, POINTS, J, POLY, I, N, LPNT, LPOLY)

C
C  *****************************************************************
C  THIS SUBROUTINE COMPUTES THE VALUE OF THE I-TH NEWTON FUNDAMENTAL
C  (STORED IN ARRAY 'POLY') AT THE J-TH POINT IN ARRAY 'POINTS'
C
C  (IF THE POLYNOMIAL IS QUADRATIC WE COMPUTE THE VALUE BY THE HORNER'S
C   RULE)
C
C  PARAMETERS
C
C  VAL    (OUTPUT) VALUE OF THE I-TH POLYNOMIAL AT THE J-TH POINT
C 
C  POINTS (INPUT)  THE ARRAY OF POINTS
C
C  J      (INPUT)  THE INDEX OF THE EVALUATION POINT
C        
C  N      (INPUT)  PROBLEM DIMENSION
C
C  POLY   (INPUT)  ARRAY OF NEWTON FUNDAMENTAL POLYNOMIALS
C          
C  I      (INPUT)  INDEX OF THE EVALUATED POLYNOMIAL
C
C  LPNT   (INPUT)  LENGTH OF THE ARRAY 'POINTS'
C  
C  LPOLY  (INPUT)  LENGTH OF THE ARRAY 'POLY'
C  *****************************************************************
C


       DOUBLE PRECISION POINTS(LPNT), POLY(LPOLY), VAL

       INTEGER          I, J, LPNT, LPOLY, N


C
C  LOCAL VARIABLES
C

       INTEGER          II, IPOINT, JJ , K,  L,  NP1
        
       DOUBLE PRECISION V

       NP1    = N+1

C
C  THE POINTER TO WHERE ELEMENTS OF J-TH POINT BEGIN IN ARRAY 'POINTS'
C
       IPOINT = (J-1)*N


       IF (I.EQ.1) THEN
C
C  IF THE I-TH POLYNOMIAL IS A  CONSTANT
C
         VAL = POLY(1)
       ELSE IF (I.LE.NP1) THEN
C
C  IF THE I-TH POLYNOMIAL IS LINEAR
C
         K    = ( I - 2 )*( NP1 ) + 2
         VAL  = POLY(K)
C
C  ADD UP THE TERMS
C

         DO 10 L=1,N
           VAL = VAL + POLY( K + L )*POINTS( IPOINT + L )
 10      CONTINUE
       ELSE
C         
C  IF THE I-TH POLYNOMIAL IS QUADRATIC
C

         K   = ( I - NP1 - 1 )*(N+2)*NP1/2 + N*NP1 + 2
         VAL = POLY( K )
         JJ  = K + NP1
         DO 30 L=1,N
           V = POLY( K + L )
           DO 20 II=L, N
             V  = V + POLY( JJ )*POINTS( IPOINT + II )
             JJ = JJ + 1
 20        CONTINUE
           VAL=VAL+V*POINTS( IPOINT + L )
 30      CONTINUE
       ENDIF
  
       RETURN
       END
           


************************************

*         NEXT SUBROUTINE          *

************************************


           
       SUBROUTINE EVALX(VAL, X, POLY, I, N, LPOLY)


C
C  *****************************************************************
C  THIS SUBROUTINE COMPUTES THE VALUE OF THE I-TH NEWTON FUNDAMENTAL
C  (STORED IN ARRAY 'POLY') AT A GIVEN POINT X
C
C  IF THE POLYNOMIAL IS QUADRATIC WE COMPUTE THE VALUE BY THE HORNER'S
C  RULE
C
C  PARAMETERS
C
C  VAL    (OUTPUT) VALUE OF THE I-TH POLYNOMIAL AT THE J-TH POINT
C 
C  X      (INPUT)  THE EVALUATION POINT
C        
C  N      (INPUT)  PROBLEM DIMENSION
C
C  POLY   (INPUT)  ARRAY OF NEWTON FUNDAMENTAL POLYNOMIALS
C          
C  I      (INPUT)  INDEX OF THE EVALUATED POLYNOMIAL
C
C  LPNT   (INPUT)  LENGTH OF THE ARRAY 'POINTS'
C  
C  LPOLY  (INPUT)  LENGTH OF THE ARRAY 'POLY'
C  *****************************************************************
C


       DOUBLE PRECISION X(N), POLY(LPOLY), VAL

       INTEGER          I, LPOLY, N


C
C  LOCAL VARIABLES
C

       INTEGER          II, J , K,  L,  NP1

       DOUBLE PRECISION V
       NP1    = N+1

       IF (I.EQ.1) THEN
C
C  IF THE I-TH POLYNOMIAL IS A  CONSTANT
C
         VAL = POLY(1)
       ELSE IF (I.LE.NP1) THEN
C
C  IF THE I-TH POLYNOMIAL IS LINEAR
C

         K   = ( I - 2 )*( NP1 ) + 2
         VAL = POLY(K) 
         DO 10 L=1,N
           VAL = VAL + POLY( K + L )*X( L )
 10     CONTINUE
       ELSE
C         
C  IF THE I-TH POLYNOMIAL IS QUADRATIC
C
         K   = ( I - NP1 - 1 )*(N+2)*NP1/2 + N*NP1 + 2
         J   = K + NP1
         VAL = POLY( K )
         DO 30 L=1,N
           V = POLY( K + L )  
           DO 20 II=L,N
             V = V + POLY( J )*X( II )
             J = J + 1
 20        CONTINUE
           VAL=VAL+V*X( L )
 30      CONTINUE
       ENDIF
       RETURN
       END
           

