
#include "f2c.h"

/* Subroutine */ int zlarcm_(integer *m, integer *n, doublereal *a, integer *
	lda, doublecomplex *b, integer *ldb, doublecomplex *c__, integer *ldc,
	 doublereal *rwork)
{
/*  -- LAPACK auxiliary routine (version 3.0) --   
       Univ. of Tennessee, Univ. of California Berkeley, NAG Ltd.,   
       Courant Institute, Argonne National Lab, and Rice University   
       June 30, 1999   


    Purpose   
    =======   

    ZLARCM performs a very simple matrix-matrix multiplication:   
             C := A * B,   
    where A is M by M and real; B is M by N and complex;   
    C is M by N and complex.   

    Arguments   
    =========   

    M       (input) INTEGER   
            The number of rows of the matrix A and of the matrix C.   
            M >= 0.   

    N       (input) INTEGER   
            The number of columns and rows of the matrix B and   
            the number of columns of the matrix C.   
            N >= 0.   

    A       (input) DOUBLE PRECISION array, dimension (LDA, M)   
            A contains the M by M matrix A.   

    LDA     (input) INTEGER   
            The leading dimension of the array A. LDA >=max(1,M).   

    B       (input) DOUBLE PRECISION array, dimension (LDB, N)   
            B contains the M by N matrix B.   

    LDB     (input) INTEGER   
            The leading dimension of the array B. LDB >=max(1,M).   

    C       (input) COMPLEX*16 array, dimension (LDC, N)   
            C contains the M by N matrix C.   

    LDC     (input) INTEGER   
            The leading dimension of the array C. LDC >=max(1,M).   

    RWORK   (workspace) DOUBLE PRECISION array, dimension (2*M*N)   

    =====================================================================   


       Quick return if possible.   

       Parameter adjustments */
    /* Table of constant values */
    static doublereal c_b6 = 1.;
    static doublereal c_b7 = 0.;
    
    /* System generated locals */
    integer a_dim1, a_offset, b_dim1, b_offset, c_dim1, c_offset, i__1, i__2, 
	    i__3, i__4, i__5;
    doublereal d__1;
    doublecomplex z__1;
    /* Builtin functions */
    double d_imag(doublecomplex *);
    /* Local variables */
    static integer i__, j, l;
    extern /* Subroutine */ int dgemm_(char *, char *, integer *, integer *, 
	    integer *, doublereal *, doublereal *, integer *, doublereal *, 
	    integer *, doublereal *, doublereal *, integer *);
#define b_subscr(a_1,a_2) (a_2)*b_dim1 + a_1
#define b_ref(a_1,a_2) b[b_subscr(a_1,a_2)]
#define c___subscr(a_1,a_2) (a_2)*c_dim1 + a_1
#define c___ref(a_1,a_2) c__[c___subscr(a_1,a_2)]


    a_dim1 = *lda;
    a_offset = 1 + a_dim1 * 1;
    a -= a_offset;
    b_dim1 = *ldb;
    b_offset = 1 + b_dim1 * 1;
    b -= b_offset;
    c_dim1 = *ldc;
    c_offset = 1 + c_dim1 * 1;
    c__ -= c_offset;
    --rwork;

    /* Function Body */
    if (*m == 0 || *n == 0) {
	return 0;
    }

    i__1 = *n;
    for (j = 1; j <= i__1; ++j) {
	i__2 = *m;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    i__3 = b_subscr(i__, j);
	    rwork[(j - 1) * *m + i__] = b[i__3].r;
/* L10: */
	}
/* L20: */
    }

    l = *m * *n + 1;
    dgemm_("N", "N", m, n, m, &c_b6, &a[a_offset], lda, &rwork[1], m, &c_b7, &
	    rwork[l], m);
    i__1 = *n;
    for (j = 1; j <= i__1; ++j) {
	i__2 = *m;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    i__3 = c___subscr(i__, j);
	    i__4 = l + (j - 1) * *m + i__ - 1;
	    c__[i__3].r = rwork[i__4], c__[i__3].i = 0.;
/* L30: */
	}
/* L40: */
    }

    i__1 = *n;
    for (j = 1; j <= i__1; ++j) {
	i__2 = *m;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    rwork[(j - 1) * *m + i__] = d_imag(&b_ref(i__, j));
/* L50: */
	}
/* L60: */
    }
    dgemm_("N", "N", m, n, m, &c_b6, &a[a_offset], lda, &rwork[1], m, &c_b7, &
	    rwork[l], m);
    i__1 = *n;
    for (j = 1; j <= i__1; ++j) {
	i__2 = *m;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    i__3 = c___subscr(i__, j);
	    i__4 = c___subscr(i__, j);
	    d__1 = c__[i__4].r;
	    i__5 = l + (j - 1) * *m + i__ - 1;
	    z__1.r = d__1, z__1.i = rwork[i__5];
	    c__[i__3].r = z__1.r, c__[i__3].i = z__1.i;
/* L70: */
	}
/* L80: */
    }

    return 0;

/*     End of ZLARCM */

} /* zlarcm_ */

#undef c___ref
#undef c___subscr
#undef b_ref
#undef b_subscr


