
#include "pnl/pnl_f2c.h"

/* Subroutine */ int dtrtrs_(char *uplo, char *trans, char *diag, integer *n, 
	integer *nrhs, doublereal *a, integer *lda, doublereal *b, integer *
	ldb, integer *info)
{
/*  -- LAPACK routine (version 3.0) --   
       Univ. of Tennessee, Univ. of California Berkeley, NAG Ltd.,   
       Courant Institute, Argonne National Lab, and Rice University   
       March 31, 1993   


    Purpose   
    =======   

    DTRTRS solves a triangular system of the form   

       A * X = B  or  A**T * X = B,   

    where A is a triangular matrix of order N, and B is an N-by-NRHS   
    matrix.  A check is made to verify that A is nonsingular.   

    Arguments   
    =========   

    UPLO    (input) CHARACTER*1   
            = 'U':  A is upper triangular;   
            = 'L':  A is lower triangular.   

    TRANS   (input) CHARACTER*1   
            Specifies the form of the system of equations:   
            = 'N':  A * X = B  (No transpose)   
            = 'T':  A**T * X = B  (Transpose)   
            = 'C':  A**H * X = B  (Conjugate transpose = Transpose)   

    DIAG    (input) CHARACTER*1   
            = 'N':  A is non-unit triangular;   
            = 'U':  A is unit triangular.   

    N       (input) INTEGER   
            The order of the matrix A.  N >= 0.   

    NRHS    (input) INTEGER   
            The number of right hand sides, i.e., the number of columns   
            of the matrix B.  NRHS >= 0.   

    A       (input) DOUBLE PRECISION array, dimension (LDA,N)   
            The triangular matrix A.  If UPLO = 'U', the leading N-by-N   
            upper triangular part of the array A contains the upper   
            triangular matrix, and the strictly lower triangular part of   
            A is not referenced.  If UPLO = 'L', the leading N-by-N lower   
            triangular part of the array A contains the lower triangular   
            matrix, and the strictly upper triangular part of A is not   
            referenced.  If DIAG = 'U', the diagonal elements of A are   
            also not referenced and are assumed to be 1.   

    LDA     (input) INTEGER   
            The leading dimension of the array A.  LDA >= max(1,N).   

    B       (input/output) DOUBLE PRECISION array, dimension (LDB,NRHS)   
            On entry, the right hand side matrix B.   
            On exit, if INFO = 0, the solution matrix X.   

    LDB     (input) INTEGER   
            The leading dimension of the array B.  LDB >= max(1,N).   

    INFO    (output) INTEGER   
            = 0:  successful exit   
            < 0: if INFO = -i, the i-th argument had an illegal value   
            > 0: if INFO = i, the i-th diagonal element of A is zero,   
                 indicating that the matrix is singular and the solutions   
                 X have not been computed.   

    =====================================================================   


       Test the input parameters.   

       Parameter adjustments */
    /* Table of constant values */
    static doublereal c_b12 = 1.;
    
    /* System generated locals */
    integer a_dim1, a_offset, b_dim1, b_offset, i__1;
    /* Local variables */
    extern logical lsame_(char *, char *);
    extern /* Subroutine */ int dtrsm_(char *, char *, char *, char *, 
	    integer *, integer *, doublereal *, doublereal *, integer *, 
	    doublereal *, integer *), xerbla_(
	    char *, integer *);
    static logical nounit;
#define a_ref(a_1,a_2) a[(a_2)*a_dim1 + a_1]


    a_dim1 = *lda;
    a_offset = 1 + a_dim1 * 1;
    a -= a_offset;
    b_dim1 = *ldb;
    b_offset = 1 + b_dim1 * 1;
    b -= b_offset;

    /* Function Body */
    *info = 0;
    nounit = lsame_(diag, "N");
    if (! lsame_(uplo, "U") && ! lsame_(uplo, "L")) {
	*info = -1;
    } else if (! lsame_(trans, "N") && ! lsame_(trans, 
	    "T") && ! lsame_(trans, "C")) {
	*info = -2;
    } else if (! nounit && ! lsame_(diag, "U")) {
	*info = -3;
    } else if (*n < 0) {
	*info = -4;
    } else if (*nrhs < 0) {
	*info = -5;
    } else if (*lda < max(1,*n)) {
	*info = -7;
    } else if (*ldb < max(1,*n)) {
	*info = -9;
    }
    if (*info != 0) {
	i__1 = -(*info);
	xerbla_("DTRTRS", &i__1);
	return 0;
    }

/*     Quick return if possible */

    if (*n == 0) {
	return 0;
    }

/*     Check for singularity. */

    if (nounit) {
	i__1 = *n;
	for (*info = 1; *info <= i__1; ++(*info)) {
	    if (a_ref(*info, *info) == 0.) {
		return 0;
	    }
/* L10: */
	}
    }
    *info = 0;

/*     Solve A * x = b  or  A' * x = b. */

    dtrsm_("Left", uplo, trans, diag, n, nrhs, &c_b12, &a[a_offset], lda, &b[
	    b_offset], ldb);

    return 0;

/*     End of DTRTRS */

} /* dtrtrs_ */

#undef a_ref


