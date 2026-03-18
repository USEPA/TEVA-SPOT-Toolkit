       SUBROUTINE RZRMAT(A,N,M)
       DOUBLE PRECISION A(N,M)
       INTEGER          M,N,I,J

       DO 10 I=1,N
         DO 20 J=1,M
           A(I,J)=0.0D0
 20      CONTINUE
 10    CONTINUE
       RETURN
       END


       SUBROUTINE RZRVEC(B,N)
       DOUBLE PRECISION B(N)
       INTEGER          N,I

       DO 10 I=1,N        
           B(I)=0.0D0
 10    CONTINUE
       RETURN
       END



       SUBROUTINE IZRMAT(A,N,M)
        
       INTEGER        A(N,M),M,N,I,J

       DO 10 I=1,N
         DO 20 J=1,M
           A(I,J)=0
 20      CONTINUE
 10    CONTINUE
       RETURN
       END

       SUBROUTINE IZRVEC(B,N)
      
       INTEGER           B(N),N,I

       DO 10 I=1,N        
           B(I)=0
 10    CONTINUE
       RETURN
       END
