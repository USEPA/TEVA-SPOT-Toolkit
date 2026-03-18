      DOUBLE PRECISION FUNCTION MVALUE( N, S, G, H, WRK, LWRK )
C 
C  ***********************************************
C     Computes the value of the quadratic model
C
C          S' * G + 0.5 * S' * H * S
C  ***********************************************
C
      INTEGER          N, LWRK
      DOUBLE PRECISION S( N ), G( N ), H( N, N ), WRK( LWRK )

C
C  LOCAL VARIABLES
C

      DOUBLE PRECISION DDOT 

      EXTERNAL         DDOT
C 
C  SUBROUTINES AND FUNCTIONS CALLED:
C
C    BLAS            :       DSYMV, DDOT
C


      CALL DSYMV( 'U', N, 0.5D0, H, N, S, 1, 0.0D0, WRK, 1 )

      MVALUE = DDOT( N, S, 1, G, 1 ) + DDOT( N, S, 1, WRK, 1 )


      RETURN
      END

