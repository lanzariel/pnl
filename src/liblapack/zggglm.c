
#include "pnl/pnl_f2c.h"

/* Subroutine */ int zggglm_(integer *n, integer *m, integer *p, 
	doublecomplex *a, integer *lda, doublecomplex *b, integer *ldb, 
	doublecomplex *d__, doublecomplex *x, doublecomplex *y, doublecomplex 
	*work, integer *lwork, integer *info)
{
/*  -- LAPACK driver routine (version 3.0) --   
       Univ. of Tennessee, Univ. of California Berkeley, NAG Ltd.,   
       Courant Institute, Argonne National Lab, and Rice University   
       June 30, 1999   


    Purpose   
    =======   

    ZGGGLM solves a general Gauss-Markov linear model (GLM) problem:   

            minimize || y ||_2   subject to   d = A*x + B*y   
                x   

    where A is an N-by-M matrix, B is an N-by-P matrix, and d is a   
    given N-vector. It is assumed that M <= N <= M+P, and   

               rank(A) = M    and    rank( A B ) = N.   

    Under these assumptions, the constrained equation is always   
    consistent, and there is a unique solution x and a minimal 2-norm   
    solution y, which is obtained using a generalized QR factorization   
    of A and B.   

    In particular, if matrix B is square nonsingular, then the problem   
    GLM is equivalent to the following weighted linear least squares   
    problem   

                 minimize || inv(B)*(d-A*x) ||_2   
                     x   

    where inv(B) denotes the inverse of B.   

    Arguments   
    =========   

    N       (input) INTEGER   
            The number of rows of the matrices A and B.  N >= 0.   

    M       (input) INTEGER   
            The number of columns of the matrix A.  0 <= M <= N.   

    P       (input) INTEGER   
            The number of columns of the matrix B.  P >= N-M.   

    A       (input/output) COMPLEX*16 array, dimension (LDA,M)   
            On entry, the N-by-M matrix A.   
            On exit, A is destroyed.   

    LDA     (input) INTEGER   
            The leading dimension of the array A. LDA >= max(1,N).   

    B       (input/output) COMPLEX*16 array, dimension (LDB,P)   
            On entry, the N-by-P matrix B.   
            On exit, B is destroyed.   

    LDB     (input) INTEGER   
            The leading dimension of the array B. LDB >= max(1,N).   

    D       (input/output) COMPLEX*16 array, dimension (N)   
            On entry, D is the left hand side of the GLM equation.   
            On exit, D is destroyed.   

    X       (output) COMPLEX*16 array, dimension (M)   
    Y       (output) COMPLEX*16 array, dimension (P)   
            On exit, X and Y are the solutions of the GLM problem.   

    WORK    (workspace/output) COMPLEX*16 array, dimension (LWORK)   
            On exit, if INFO = 0, WORK(1) returns the optimal LWORK.   

    LWORK   (input) INTEGER   
            The dimension of the array WORK. LWORK >= max(1,N+M+P).   
            For optimum performance, LWORK >= M+min(N,P)+max(N,P)*NB,   
            where NB is an upper bound for the optimal blocksizes for   
            ZGEQRF, CGERQF, ZUNMQR and CUNMRQ.   

            If LWORK = -1, then a workspace query is assumed; the routine   
            only calculates the optimal size of the WORK array, returns   
            this value as the first entry of the WORK array, and no error   
            message related to LWORK is issued by XERBLA.   

    INFO    (output) INTEGER   
            = 0:  successful exit.   
            < 0:  if INFO = -i, the i-th argument had an illegal value.   

    ===================================================================   


       Test the input parameters   

       Parameter adjustments */
    /* Table of constant values */
    static doublecomplex c_b2 = {1.,0.};
    static integer c__1 = 1;
    static integer c_n1 = -1;
    
    /* System generated locals */
    integer a_dim1, a_offset, b_dim1, b_offset, i__1, i__2, i__3, i__4;
    doublecomplex z__1;
    /* Local variables */
    static integer lopt, i__;
    extern /* Subroutine */ int zgemv_(char *, integer *, integer *, 
	    doublecomplex *, doublecomplex *, integer *, doublecomplex *, 
	    integer *, doublecomplex *, doublecomplex *, integer *), 
	    zcopy_(integer *, doublecomplex *, integer *, doublecomplex *, 
	    integer *), ztrsv_(char *, char *, char *, integer *, 
	    doublecomplex *, integer *, doublecomplex *, integer *);
    static integer nb, np;
    extern /* Subroutine */ int xerbla_(char *, integer *);
    extern integer ilaenv_(integer *, char *, char *, integer *, integer *, 
	    integer *, integer *, ftnlen, ftnlen);
    extern /* Subroutine */ int zggqrf_(integer *, integer *, integer *, 
	    doublecomplex *, integer *, doublecomplex *, doublecomplex *, 
	    integer *, doublecomplex *, doublecomplex *, integer *, integer *)
	    ;
    static integer nb1, nb2, nb3, nb4, lwkopt;
    static logical lquery;
    extern /* Subroutine */ int zunmqr_(char *, char *, integer *, integer *, 
	    integer *, doublecomplex *, integer *, doublecomplex *, 
	    doublecomplex *, integer *, doublecomplex *, integer *, integer *), zunmrq_(char *, char *, integer *, integer *, 
	    integer *, doublecomplex *, integer *, doublecomplex *, 
	    doublecomplex *, integer *, doublecomplex *, integer *, integer *);
#define b_subscr(a_1,a_2) (a_2)*b_dim1 + a_1
#define b_ref(a_1,a_2) b[b_subscr(a_1,a_2)]


    a_dim1 = *lda;
    a_offset = 1 + a_dim1 * 1;
    a -= a_offset;
    b_dim1 = *ldb;
    b_offset = 1 + b_dim1 * 1;
    b -= b_offset;
    --d__;
    --x;
    --y;
    --work;

    /* Function Body */
    *info = 0;
    np = min(*n,*p);
    nb1 = ilaenv_(&c__1, "ZGEQRF", " ", n, m, &c_n1, &c_n1, (ftnlen)6, (
	    ftnlen)1);
    nb2 = ilaenv_(&c__1, "ZGERQF", " ", n, m, &c_n1, &c_n1, (ftnlen)6, (
	    ftnlen)1);
    nb3 = ilaenv_(&c__1, "ZUNMQR", " ", n, m, p, &c_n1, (ftnlen)6, (ftnlen)1);
    nb4 = ilaenv_(&c__1, "ZUNMRQ", " ", n, m, p, &c_n1, (ftnlen)6, (ftnlen)1);
/* Computing MAX */
    i__1 = max(nb1,nb2), i__1 = max(i__1,nb3);
    nb = max(i__1,nb4);
    lwkopt = *m + np + max(*n,*p) * nb;
    work[1].r = (doublereal) lwkopt, work[1].i = 0.;
    lquery = *lwork == -1;
    if (*n < 0) {
	*info = -1;
    } else if (*m < 0 || *m > *n) {
	*info = -2;
    } else if (*p < 0 || *p < *n - *m) {
	*info = -3;
    } else if (*lda < max(1,*n)) {
	*info = -5;
    } else if (*ldb < max(1,*n)) {
	*info = -7;
    } else /* if(complicated condition) */ {
/* Computing MAX */
	i__1 = 1, i__2 = *n + *m + *p;
	if (*lwork < max(i__1,i__2) && ! lquery) {
	    *info = -12;
	}
    }
    if (*info != 0) {
	i__1 = -(*info);
	xerbla_("ZGGGLM", &i__1);
	return 0;
    } else if (lquery) {
	return 0;
    }

/*     Quick return if possible */

    if (*n == 0) {
	return 0;
    }

/*     Compute the GQR factorization of matrices A and B:   

              Q'*A = ( R11 ) M,    Q'*B*Z' = ( T11   T12 ) M   
                     (  0  ) N-M             (  0    T22 ) N-M   
                        M                     M+P-N  N-M   

       where R11 and T22 are upper triangular, and Q and Z are   
       unitary. */

    i__1 = *lwork - *m - np;
    zggqrf_(n, m, p, &a[a_offset], lda, &work[1], &b[b_offset], ldb, &work[*m 
	    + 1], &work[*m + np + 1], &i__1, info);
    i__1 = *m + np + 1;
    lopt = (integer) work[i__1].r;

/*     Update left-hand-side vector d = Q'*d = ( d1 ) M   
                                               ( d2 ) N-M */

    i__1 = max(1,*n);
    i__2 = *lwork - *m - np;
    zunmqr_("Left", "Conjugate transpose", n, &c__1, m, &a[a_offset], lda, &
	    work[1], &d__[1], &i__1, &work[*m + np + 1], &i__2, info);
/* Computing MAX */
    i__3 = *m + np + 1;
    i__1 = lopt, i__2 = (integer) work[i__3].r;
    lopt = max(i__1,i__2);

/*     Solve T22*y2 = d2 for y2 */

    i__1 = *n - *m;
    ztrsv_("Upper", "No transpose", "Non unit", &i__1, &b_ref(*m + 1, *m + *p 
	    - *n + 1), ldb, &d__[*m + 1], &c__1);
    i__1 = *n - *m;
    zcopy_(&i__1, &d__[*m + 1], &c__1, &y[*m + *p - *n + 1], &c__1);

/*     Set y1 = 0 */

    i__1 = *m + *p - *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	i__2 = i__;
	y[i__2].r = 0., y[i__2].i = 0.;
/* L10: */
    }

/*     Update d1 = d1 - T12*y2 */

    i__1 = *n - *m;
    z__1.r = -1., z__1.i = 0.;
    zgemv_("No transpose", m, &i__1, &z__1, &b_ref(1, *m + *p - *n + 1), ldb, 
	    &y[*m + *p - *n + 1], &c__1, &c_b2, &d__[1], &c__1);

/*     Solve triangular system: R11*x = d1 */

    ztrsv_("Upper", "No Transpose", "Non unit", m, &a[a_offset], lda, &d__[1],
	     &c__1);

/*     Copy D to X */

    zcopy_(m, &d__[1], &c__1, &x[1], &c__1);

/*     Backward transformation y = Z'*y   

   Computing MAX */
    i__1 = 1, i__2 = *n - *p + 1;
    i__3 = max(1,*p);
    i__4 = *lwork - *m - np;
    zunmrq_("Left", "Conjugate transpose", p, &c__1, &np, &b_ref(max(i__1,
	    i__2), 1), ldb, &work[*m + 1], &y[1], &i__3, &work[*m + np + 1], &
	    i__4, info);
/* Computing MAX */
    i__4 = *m + np + 1;
    i__2 = lopt, i__3 = (integer) work[i__4].r;
    i__1 = *m + np + max(i__2,i__3);
    work[1].r = (doublereal) i__1, work[1].i = 0.;

    return 0;

/*     End of ZGGGLM */

} /* zggglm_ */

#undef b_ref
#undef b_subscr


