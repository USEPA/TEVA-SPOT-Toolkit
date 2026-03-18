      SUBROUTINE FUNCON(MODE, NCNLN, N, NROWJ, NEEDC, X, C,
     *                  CJAC, NSTATE)   
 

      INTEGER          MODE, NCNLN, N, NROWJ, NEEDC(*), NSTATE
      
      DOUBLE PRECISION X(N), C(*), CJAC(NROWJ,N)

      INTEGER           I, J, K, NEASY, L, NCONT

      DOUBLE PRECISION  HALF

      PARAMETER        (HALF=0.5D0)
      INCLUDE 'dfo_model_inc.f'

      NEASY=NLIN+NNLN
C
C  IF WE ARE DEALING WITH IPOPT, THEN MODE=1,2,3 INDICATES WHETHER
C  THE VALUES OF THE CONSTRAINTS, THE JACOBIAN OF THE HESSIAN OF THE 
C  LAGRANGIAN IS EXPECTED
C
      IF (USEIPOPT .GT. 0) THEN
        IF ( MODE .EQ. 1) THEN
          DO 5  K=1, NLIN
            C(K)=0.0D0
            DO 6 I=1, N
              C(K)= C(K)+AMAT(K, I)*X(I)
 6          CONTINUE
 5        CONTINUE  
          IF ( NNLN .GT. 0 ) CALL EASYCON(N, X, NNLN, C(NLIN+1))
          IF ( .NOT. USEMERIT) THEN
            DO 10 K = 1, NCON
              L=K+NEASY
              C(L)=CCON(K)
              DO 30 I = 1, N
                C(L)=C(L)+LCON((K-1)*N+I)*X(I)
                DO 20 J = 1, N
                  C(L)=C(L)+HALF*QCON((K-1)*N*N+(I-1)*N+J)*X(I)*X(J)
 20             CONTINUE
 30           CONTINUE
 10         CONTINUE  
          ENDIF
        ELSEIF (MODE. EQ. 2) THEN    
          DO 15  K=1, NLIN
            DO 16 I=1, N
              CJAC(K,I)= AMAT(K, I)
 16          CONTINUE
 15       CONTINUE  
          IF ( NNLN .GT. 0 ) CALL EASYJAC(N, X, NNLN, NROWJ, 
     .                       CJAC(NLIN+1,1))
          IF ( .NOT. USEMERIT) THEN
            DO 60 K = 1, NCON
              L=K+NEASY
              DO 50 I = 1, N
                CJAC(L, I)=LCON((K-1)*N+I)
                DO 40 J = 1, N
                  CJAC(L, I)=CJAC(L, I)+QCON((K-1)*N*N+(I-1)*N+J)*X(J)
 40             CONTINUE
 50           CONTINUE
 60         CONTINUE 
          ENDIF 
         ELSE IF ( MODE .EQ. 3 ) THEN
          DO 65  K=1, N
            DO 66 I=1, N
              CJAC(K,I)= HMOD(K,I)
 66          CONTINUE
 65        CONTINUE  
          DO 70 K = NLIN + 1, NEASY
            CALL EASYHESS(K, N, X, NNLN, NROWJ, CJAC, C(K))
 70       CONTINUE  
          IF ( .NOT. USEMERIT) THEN
            DO 100 K = 1, NCON
              L=K+NEASY
              DO 90 I = 1, N
                DO 80 J = 1, N
                  CJAC(J, I)=CJAC(J, I)+C(L)*QCON((K-1)*N*N+(I-1)*N+J)
 80             CONTINUE
 90           CONTINUE
 100        CONTINUE 
          ENDIF           
         ENDIF
       ELSE
        CALL EASYCON(N, X, NNLN, C(1))
        CALL EASYJAC(N, X, NNLN, NROWJ, CJAC)
        IF ( .NOT. USEMERIT ) THEN
          DO 110 K = 1, NCON
            L=K+NNLN
            C(L)=CCON(K)
            DO 130 I = 1, N
            C(L)=C(L)+LCON((K-1)*N+I)*X(I)
            CJAC(L, I)=LCON((K-1)*N+I)
            DO 120 J = 1, N
              C(L)=C(L)+HALF*QCON((K-1)*N*N+(I-1)*N+J)*X(I)*X(J)
              CJAC(L, I)=CJAC(L, I)+QCON((K-1)*N*N+(I-1)*N+J)*X(J)
 120        CONTINUE
 130      CONTINUE
 110     CONTINUE       
       ENDIF
      ENDIF   
      RETURN
      END









