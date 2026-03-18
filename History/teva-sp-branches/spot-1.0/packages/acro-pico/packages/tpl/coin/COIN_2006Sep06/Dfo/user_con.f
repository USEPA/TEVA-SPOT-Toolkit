      SUBROUTINE EASYCON(N, X, NCNLN, C)   
 

      INTEGER           NCNLN, N
      
      DOUBLE PRECISION X(N), C(*)

      INTEGER           I, J
C
C  COMMON VARIABLE FOR THE TEST NUMBER
C
      INTEGER           NTEST
      COMMON  /TEST/    NTEST
      SAVE /TEST/
C
C  CAN BE A DUMMY ROUTINE. IF CONSTRAINTS ARE PRESENT SHOULD COMPUTE
C  THE VALUES OF THE CONSTRAINTS AT POINT X AND STORE IT IN C
C
      IF ( NTEST .EQ. 11 ) THEN
        IF ( NCNLN .GT. 0 ) THEN
          DO 50 I=1, NCNLN
              IF(I.EQ.1) THEN
                C(1)=EXP(X(1))+2*EXP(X(2))+2*EXP(X(3))+EXP(X(6))+
     +               EXP(X(10))
              ENDIF
              IF(I.EQ.2) THEN
                C(2)=EXP(X(4))+2*EXP(X(5))+EXP(X(6))+EXP(X(7))
              ENDIF
 50       CONTINUE
        ENDIF
      ELSEIF (NTEST .EQ. 12 ) THEN
        IF ( NCNLN .GT. 0 ) THEN
                C(1)=-3*(X(1)-2)**2-4*(X(2)-3)**2-2*X(3)**2+
     +                7*X(4)+120
                C(2)=-5*X(1)**2-8*X(2)
     +                - (X(3)-6)**2+2*X(4)+ 40
        ENDIF         
      ENDIF 
      RETURN
      END

      SUBROUTINE EASYJAC(N, X, NCNLN, NROWC, CJAC)   
      INTEGER           NTEST
      COMMON  /TEST/    NTEST 
      SAVE /TEST/

      INTEGER           NCNLN, N, NROWC
      
      DOUBLE PRECISION X(N), CJAC(NROWC,N)

      INTEGER           I, J
C
C  CAN BE A DUMMY ROUTINE. IF CONSTRAINTS ARE PRESENT SHOULD COMPUTE
C  THE JACOBIAN OF THE CONSTRAINTS AT POINT X AND STORE IT IN CJAC
C  SO THAT THE FIST ROW IS THE GRADIENT OF THE FIRST CONSTRAINT
C
      IF ( NTEST .EQ. 11 ) THEN
        DO 50 I=1, NCNLN
              DO 55 J = 1, N
                CJAC(I, J) = 0.0D0
 55           CONTINUE  
              IF(I.EQ.1) THEN
                CJAC(1,1)=EXP(X(1))
                CJAC(1,2)=2*EXP(X(2))    
                CJAC(1,3)=2*EXP(X(3))
                CJAC(1,6)=EXP(X(6))
                CJAC(1,10)=EXP(X(10))    
              ENDIF
              IF(I.EQ.2) THEN
                CJAC(2,4)=EXP(X(4))
                CJAC(2,5)=2*EXP(X(5))  
                CJAC(2,6)=EXP(X(6))
                CJAC(2,7)=EXP(X(7))   
              ENDIF
              IF(I.EQ.3) THEN
                CJAC(3,3)=EXP(X(3))
                CJAC(3,7)=EXP(X(7))  
                CJAC(3,8)=EXP(X(8))
                CJAC(3,9)=2*EXP(X(9))
                CJAC(3,10)=EXP(X(10)) 
              ENDIF
 50     CONTINUE
      ELSEIF (NTEST .EQ. 12 ) THEN
        IF ( NCNLN .GT. 0 ) THEN
         DO 65 J = 1, N
           CJAC(1, J) = 0.0D0
 65      CONTINUE  
         DO 66 J = 1, N
           CJAC(2, J) = 0.0D0
 66      CONTINUE  
          CJAC(1,1)=-6D0*(X(1)-2d0)
          CJAC(1,2)=-8D0*(X(2)-3d0)
          CJAC(1,3)=-4d0*x(3)
          CJAC(1,4)=7d0
          CJAC(2,1)=-10D0*X(1)
          CJAC(2,2)=-8D0
          CJAC(2,3)=-2d0*(x(3)-6d0)
          CJAC(2,4)=2d0
        ENDIF         
      ENDIF 
      RETURN
      END

      SUBROUTINE EASYHESS(K, N, X, NCNLN, NROWC, CHESS, LAMBDA)   
      INTEGER           NTEST
      COMMON  /TEST/    NTEST
      SAVE /TEST/
      INTEGER           NCNLN, N, NROWC, K
      
      DOUBLE PRECISION X(N), CHESS(NROWC,N), LAMBDA

      INTEGER           I, J
C
C  CAN BE A DUMMY ROUTINE. IF CONSTRAINTS ARE PRESENT SHOULD COMPUTE
C  THE HESSIAN OF THE K-TH CONSTRAINTS AT POINT X AND ADD IT TO
C  EXISTING CHESS WITH COEFFICIENT LAMBDA 
C      CHESS=CHESS+LAMBDA*HESS(C_K(X))

      IF ( NTEST .EQ. 11 ) THEN
        IF ( K.EQ. 1) THEN 
          CHESS(1,1)=CHESS(1,1)+EXP(X(1))*LAMBDA
          CHESS(2,2)=CHESS(2,2)+2*EXP(X(2))*LAMBDA
          CHESS(3,3)=CHESS(3,3)+EXP(X(3))*(2*LAMBDA)
          CHESS(6,6)=CHESS(6,6)+EXP(X(6))*(LAMBDA)
          CHESS(10,10)=CHESS(10,10)+EXP(X(10))*(LAMBDA)  
        ELSE IF ( K.EQ. 2) THEN   
          CHESS(4,4)=CHESS(4,4)+EXP(X(4))*LAMBDA
          CHESS(5,5)=CHESS(5,5)+2*EXP(X(5))*LAMBDA
          CHESS(6,6)=CHESS(6,6)+EXP(X(6))*LAMBDA
          CHESS(7,7)=CHESS(7,7)+EXP(X(7))*(LAMBDA)
        ENDIF
      ELSEIF (NTEST .EQ. 12 ) THEN
        IF ( NCNLN .GT. 0 ) THEN
         IF ( K .EQ. 1 ) THEN
          CHESS(1,1)=CHESS(1,1)-LAMBDA*6D0
          CHESS(2,2)=CHESS(2,2)-LAMBDA*8D0
          CHESS(3,3)=CHESS(3,3)-LAMBDA*4D0
         ELSEIF ( K.EQ. 2) THEN 
          CHESS(1,1)=CHESS(1,1)-LAMBDA*10D0
          CHESS(3,3)=CHESS(3,3)-LAMBDA*2D0
        ENDIF 
       ENDIF           
      ENDIF  
      RETURN
      END




