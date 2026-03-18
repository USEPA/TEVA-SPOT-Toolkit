      SUBROUTINE FUNOBJ(MODE, N, X, OBJF, OBJGRD, NSTATE)
          
      DOUBLE PRECISION OBJF, X(N), OBJGRD(N)
      
      INTEGER          MODE, N, NSTATE, I, J, K

      DOUBLE PRECISION  HALF, VAL

      PARAMETER        (HALF=0.5D0)
C
C  IF IPOPT FLAG IS >0 THEN WE ARE CALLED BY IPOPT AND THEN MODE=1 MEANS
C  THAT THE OBJECTIVE FUNCTION VALUE IS EXPACTED. IF MODE = 2 THEN
C  THE GRADIENT OF THE OBJECTIVE IS REQUIRED. IF IPOPT =< 0 THEN
C  NPSOL IS CALLING, THE COMPUTE OBJF AND OBJGRAD AT ONCE.
C
      INCLUDE 'dfo_model_inc.f'

      IF ( USEIPOPT .GT. 0 ) THEN 
        IF (MODE .EQ. 1 ) THEN      
          OBJF=0
          DO 10 I=1,N
            OBJF=OBJF+GMOD(I)*X(I)
            DO 20 J=1,N
              OBJF=OBJF+HALF*HMOD(I,J)*X(J)*X(I)
 20         CONTINUE
 10       CONTINUE
          IF ( USEMERIT ) THEN
            DO 170 K=1,NCON
            VAL=CCON(K)
            DO 180 I=1,N
              VAL=VAL+LCON((K-1)*N+I)*X(I)
              DO 190 J=1,N
                VAL=VAL+HALF*QCON((K-1)*N*N+(I-1)*N+J)*X(I)*X(J)
 190          CONTINUE
 180        CONTINUE
            IF ( VAL .LT. CONL(K) ) THEN 
              OBJF = OBJF + PENPAR * ( CONL(K) - VAL )
            ELSEIF ( VAL .GT. CONU(K) ) THEN 
              OBJF = OBJF + PENPAR * ( VAL - CONU(K) )
            ENDIF
 170        CONTINUE  
          ENDIF
        ELSE
          DO 30 I=1,N
            OBJGRD(I)=GMOD(I)
            DO 40 J=1,N
              OBJGRD(I)=OBJGRD(I)+HMOD(I,J)*X(J)
 40         CONTINUE
 30       CONTINUE
          IF ( USEMERIT ) THEN
            DO 270 K=1,NCON
              VAL=CCON(K)
              DO 280 I=1,N
                VAL=VAL+LCON((K-1)*N+I)*X(I)
                DO 290 J=1,N
                  VAL=VAL+HALF*QCON((K-1)*N*N+(I-1)*N+J)*X(I)*X(J)
 290            CONTINUE
 280          CONTINUE
              IF ( VAL .LT. CONL(K) ) THEN 
                DO 200 I=1,N
                  OBJGRD(I)=OBJGRD(I)-PENPAR*LCON((K-1)*N+I)
                  DO 210 J=1,N
                    OBJGRD(I)=OBJGRD(I)-PENPAR*QCON((K-1)*N*N+(I-1)*N+J)
     .              *X(J)
 210              CONTINUE
 200            CONTINUE
              ELSEIF ( VAL .GT. CONU(K) ) THEN 
                DO 220 I=1,N
                  OBJGRD(I)=OBJGRD(I)+PENPAR*LCON((K-1)*N+I)
                  DO 230 J=1,N
                    OBJGRD(I)=OBJGRD(I)+PENPAR*QCON((K-1)*N*N+(I-1)*N+J)
     .              *X(J)
 230              CONTINUE
 220            CONTINUE
              ENDIF
 270        CONTINUE
          ENDIF
        ENDIF  
      ELSE
        OBJF=0
        DO 50 I=1,N
          OBJF=OBJF+GMOD(I)*X(I)
          OBJGRD(I)=GMOD(I)
          DO 60 J=1,N
            OBJF=OBJF+HALF*HMOD(I,J)*X(J)*X(I)
            OBJGRD(I)=OBJGRD(I)+HMOD(I,J)*X(J)
 60       CONTINUE
 50     CONTINUE
        IF ( USEMERIT ) THEN
          DO 70 K=1,NCON
            VAL=CCON(K)
            DO 80 I=1,N
              VAL=VAL+LCON((K-1)*N+I)*X(I)
              DO 90 J=1,N
                VAL=VAL+HALF*QCON((K-1)*N*N+(I-1)*N+J)*X(I)*X(J)
 90           CONTINUE
 80         CONTINUE
            IF ( VAL .LT. CONL(K) ) THEN 
              OBJF = OBJF + PENPAR * ( CONL(K) - VAL )
              DO 100 I=1,N
                OBJGRD(I)=OBJGRD(I)-PENPAR*LCON((K-1)*N+I)
                DO 110 J=1,N
                  OBJGRD(I)=OBJGRD(I)-PENPAR*QCON((K-1)*N*N+(I-1)*N+J)
     .            *X(J)
 110            CONTINUE
 100          CONTINUE
            ELSEIF ( VAL .GT. CONU(K) ) THEN 
              OBJF = OBJF + PENPAR * ( VAL - CONU(K) )
              DO 120 I=1,N
                OBJGRD(I)=OBJGRD(I)+PENPAR*LCON((K-1)*N+I)
                DO 130 J=1,N
                  OBJGRD(I)=OBJGRD(I)+PENPAR*QCON((K-1)*N*N+(I-1)*N+J)
     .            *X(J)
 130            CONTINUE
 120          CONTINUE
            ENDIF
 70       CONTINUE  
        ENDIF
      ENDIF
      RETURN
      END





