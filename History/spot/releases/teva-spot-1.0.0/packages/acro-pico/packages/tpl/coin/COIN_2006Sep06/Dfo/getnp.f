*
*
*
      SUBROUTINE GETNP( IPOLY, POLY, LPOLY, N, KAPPA, G, H)


*
*     collects  the coefficient of the polynomial IPOLY into
*     doefficient of a quadratic kappa+g'x+0.5 x'H'x


C     INPUT
C     POLY   = Newton polynomials
C     IPOLY  = The index of the polynomial we are processing

C     OUTPUT
C     KAPPA = the constant of the polynomail
C     G     = the linear coeffitients
C     H     = the n by n matrix of quadratic doeffitients
      INTEGER           IPOLY, LPOLY, N

      DOUBLE PRECISION  POLY(LPOLY), KAPPA,  G(N), H(N,N)
*
*     Local variables
*

      DOUBLE PRECISION  ZERO
      PARAMETER        (ZERO=0.0D0)
      INTEGER           I, J, KBEG, K, NP1, DD

      NP1=N+1
      DD=(NP1)*(N+2)/2

      KAPPA=ZERO
      DO 10 I=1,N
        G(I)=ZERO
        DO 20 J=1,N
          H(I,J)=ZERO
 20    CONTINUE
 10   CONTINUE

       
      IF (IPOLY.EQ.1) THEN
        KAPPA=POLY(1)
      ELSEIF (IPOLY .LE. NP1) THEN
        KBEG=2+(IPOLY-2)*(NP1)
        KAPPA=POLY(KBEG)
        DO 30 I=1, N
         G(I)=POLY(KBEG+I)
 30     CONTINUE
      ELSE        
        KBEG=2+N*NP1+(IPOLY-NP1-1)*DD
        KAPPA=POLY(KBEG)
        DO 40 I=1, N
          G(I)=POLY(KBEG+I)
 40     CONTINUE
        K=KBEG+N+1
        DO 50 I=1, N
          H(I,I)=2*POLY(K)
          K=K+1
          DO 60 J=I+1,N
            H(I,J)=POLY(K)
            H(J,I)=POLY(K)
            K=K+1
 60       CONTINUE
 50     CONTINUE
      ENDIF


      RETURN
      END
