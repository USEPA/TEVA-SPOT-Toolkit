      SUBROUTINE FUNMRT(MODE, N, X, OBJF, OBJGRD, NSTATE)
          
      DOUBLE PRECISION OBJF, X(N), OBJGRD(N), VAL
      
      INTEGER          MODE, N, NSTATE, I, J, K

      DOUBLE PRECISION  HALF

      PARAMETER        (HALF=0.5D0)

      IF (MODE.NE.2) THEN
C  GIVE A WARNING
      
      ENDIF
      
      OBJF=0
      DO 10 I=1,N
        OBJF=OBJF+GMOD(I)*X(I)
        OBJGRD(I)=GMOD(I)
        DO 20 J=1,N
          OBJF=OBJF+HALF*HMOD(I,J)*X(J)*X(I)
          OBJGRD(I)=OBJGRD(I)+HMOD(I,J)*X(J)
 20     CONTINUE
 10   CONTINUE

      DO 30 K=1,NCON
        VAL=CCON(K)
        DO 40 I=1,N
        VAL=VAL+LCON((K-1)*N+I)*X(I)
          DO 50 J=1,N
            VAL=VAL+HALF*QCON((K-1)*N*N+(I-1)*N+J)*X(I)*X(J)
 50       CONTINUE
 40     CONTINUE
        IF ( VAL .LT. CONL(K) ) THEN 
          OBJF = OBJF + PENPAR * ( CONL(K) - VAL )
          DO 60 I=1,N
            OBJGRD(I)=OBJGRD(I)-PENPAR*LCON((K-1)*N+I)
            DO 70 J=1,N
              OBJGRD(I)=OBJGRD(I)-PENPAR*QCON((K-1)*N*N+(I-1)*N+J)*X(J)
 70         CONTINUE
 60       CONTINUE
        ELSEIF ( VAL .GT. CONU(K) ) THEN 
          OBJF = OBJF + PENPAR * ( VAL - CONU(K) )
          DO 80 I=1,N
            OBJGRD(I)=OBJGRD(I)+PENPAR*LCON((K-1)*N+I)
            DO 90 J=1,N
              OBJGRD(I)=OBJGRD(I)+PENPAR*QCON((K-1)*N*N+(I-1)*N+J)*X(J)
 90         CONTINUE
 80       CONTINUE
        ENDIF
 30   CONTINUE        

      RETURN
      END





