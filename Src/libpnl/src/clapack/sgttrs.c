#include "blaswrap.h"
#include "f2c.h"

/* Subroutine */ int sgttrs_(char *trans, integer *n, integer *nrhs, real *dl,
	 real *d__, real *du, real *du2, integer *ipiv, real *b, integer *ldb,
	 integer *info)
{
/*  -- LAPACK routine (version 3.0) --   
       Univ. of Tennessee, Univ. of California Berkeley, NAG Ltd.,   
       Courant Institute, Argonne National Lab, and Rice University   
       June 30, 1999   


    Purpose   
    =======   

    SGTTRS solves one of the systems of equations   
       A*X = B  or  A'*X = B,   
    with a tridiagonal matrix A using the LU factorization computed   
    by SGTTRF.   

    Arguments   
    =========   

    TRANS   (input) CHARACTER   
            Specifies the form of the system of equations.   
            = 'N':  A * X = B  (No transpose)   
            = 'T':  A'* X = B  (Transpose)   
            = 'C':  A'* X = B  (Conjugate transpose = Transpose)   

    N       (input) INTEGER   
            The order of the matrix A.   

    NRHS    (input) INTEGER   
            The number of right hand sides, i.e., the number of columns   
            of the matrix B.  NRHS >= 0.   

    DL      (input) REAL array, dimension (N-1)   
            The (n-1) multipliers that define the matrix L from the   
            LU factorization of A.   

    D       (input) REAL array, dimension (N)   
            The n diagonal elements of the upper triangular matrix U from   
            the LU factorization of A.   

    DU      (input) REAL array, dimension (N-1)   
            The (n-1) elements of the first super-diagonal of U.   

    DU2     (input) REAL array, dimension (N-2)   
            The (n-2) elements of the second super-diagonal of U.   

    IPIV    (input) INTEGER array, dimension (N)   
            The pivot indices; for 1 <= i <= n, row i of the matrix was   
            interchanged with row IPIV(i).  IPIV(i) will always be either   
            i or i+1; IPIV(i) = i indicates a row interchange was not   
            required.   

    B       (input/output) REAL array, dimension (LDB,NRHS)   
            On entry, the matrix of right hand side vectors B.   
            On exit, B is overwritten by the solution vectors X.   

    LDB     (input) INTEGER   
            The leading dimension of the array B.  LDB >= max(1,N).   

    INFO    (output) INTEGER   
            = 0:  successful exit   
            < 0:  if INFO = -i, the i-th argument had an illegal value   

    =====================================================================   


       Parameter adjustments */
    /* Table of constant values */
    static integer c__1 = 1;
    static integer c_n1 = -1;
    
    /* System generated locals */
    integer b_dim1, b_offset, i__1, i__2, i__3;
    /* Local variables */
    static integer j, jb, nb;
    extern /* Subroutine */ int sgtts2_(integer *, integer *, integer *, real 
	    *, real *, real *, real *, integer *, real *, integer *), xerbla_(
	    char *, integer *);
    extern integer ilaenv_(integer *, char *, char *, integer *, integer *, 
	    integer *, integer *, ftnlen, ftnlen);
    static integer itrans;
    static logical notran;
#define b_ref(a_1,a_2) b[(a_2)*b_dim1 + a_1]


    --dl;
    --d__;
    --du;
    --du2;
    --ipiv;
    b_dim1 = *ldb;
    b_offset = 1 + b_dim1 * 1;
    b -= b_offset;

    /* Function Body */
    *info = 0;
    notran = *(unsigned char *)trans == 'N' || *(unsigned char *)trans == 'n';
    if (! notran && ! (*(unsigned char *)trans == 'T' || *(unsigned char *)
	    trans == 't') && ! (*(unsigned char *)trans == 'C' || *(unsigned 
	    char *)trans == 'c')) {
	*info = -1;
    } else if (*n < 0) {
	*info = -2;
    } else if (*nrhs < 0) {
	*info = -3;
    } else if (*ldb < max(*n,1)) {
	*info = -10;
    }
    if (*info != 0) {
	i__1 = -(*info);
	xerbla_("SGTTRS", &i__1);
	return 0;
    }

/*     Quick return if possible */

    if (*n == 0 || *nrhs == 0) {
	return 0;
    }

/*     Decode TRANS */

    if (notran) {
	itrans = 0;
    } else {
	itrans = 1;
    }

/*     Determine the number of right-hand sides to solve at a time. */

    if (*nrhs == 1) {
	nb = 1;
    } else {
/* Computing MAX */
	i__1 = 1, i__2 = ilaenv_(&c__1, "SGTTRS", trans, n, nrhs, &c_n1, &
		c_n1, (ftnlen)6, (ftnlen)1);
	nb = max(i__1,i__2);
    }

    if (nb >= *nrhs) {
	sgtts2_(&itrans, n, nrhs, &dl[1], &d__[1], &du[1], &du2[1], &ipiv[1], 
		&b[b_offset], ldb);
    } else {
	i__1 = *nrhs;
	i__2 = nb;
	for (j = 1; i__2 < 0 ? j >= i__1 : j <= i__1; j += i__2) {
/* Computing MIN */
	    i__3 = *nrhs - j + 1;
	    jb = min(i__3,nb);
	    sgtts2_(&itrans, n, &jb, &dl[1], &d__[1], &du[1], &du2[1], &ipiv[
		    1], &b_ref(1, j), ldb);
/* L10: */
	}
    }

/*     End of SGTTRS */

    return 0;
} /* sgttrs_ */

#undef b_ref


