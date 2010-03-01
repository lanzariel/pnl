
#include "f2c.h"

/* Subroutine */ int cgetri_(integer *n, complex *a, integer *lda, integer *
	ipiv, complex *work, integer *lwork, integer *info)
{
/*  -- LAPACK routine (version 3.0) --   
       Univ. of Tennessee, Univ. of California Berkeley, NAG Ltd.,   
       Courant Institute, Argonne National Lab, and Rice University   
       June 30, 1999   


    Purpose   
    =======   

    CGETRI computes the inverse of a matrix using the LU factorization   
    computed by CGETRF.   

    This method inverts U and then computes inv(A) by solving the system   
    inv(A)*L = inv(U) for inv(A).   

    Arguments   
    =========   

    N       (input) INTEGER   
            The order of the matrix A.  N >= 0.   

    A       (input/output) COMPLEX array, dimension (LDA,N)   
            On entry, the factors L and U from the factorization   
            A = P*L*U as computed by CGETRF.   
            On exit, if INFO = 0, the inverse of the original matrix A.   

    LDA     (input) INTEGER   
            The leading dimension of the array A.  LDA >= max(1,N).   

    IPIV    (input) INTEGER array, dimension (N)   
            The pivot indices from CGETRF; for 1<=i<=N, row i of the   
            matrix was interchanged with row IPIV(i).   

    WORK    (workspace/output) COMPLEX array, dimension (LWORK)   
            On exit, if INFO=0, then WORK(1) returns the optimal LWORK.   

    LWORK   (input) INTEGER   
            The dimension of the array WORK.  LWORK >= max(1,N).   
            For optimal performance LWORK >= N*NB, where NB is   
            the optimal blocksize returned by ILAENV.   

            If LWORK = -1, then a workspace query is assumed; the routine   
            only calculates the optimal size of the WORK array, returns   
            this value as the first entry of the WORK array, and no error   
            message related to LWORK is issued by XERBLA.   

    INFO    (output) INTEGER   
            = 0:  successful exit   
            < 0:  if INFO = -i, the i-th argument had an illegal value   
            > 0:  if INFO = i, U(i,i) is exactly zero; the matrix is   
                  singular and its inverse could not be computed.   

    =====================================================================   


       Test the input parameters.   

       Parameter adjustments */
    /* Table of constant values */
    static complex c_b2 = {1.f,0.f};
    static integer c__1 = 1;
    static integer c_n1 = -1;
    static integer c__2 = 2;
    
    /* System generated locals */
    integer a_dim1, a_offset, i__1, i__2, i__3, i__4, i__5;
    complex q__1;
    /* Local variables */
    static integer i__, j;
    extern /* Subroutine */ int cgemm_(char *, char *, integer *, integer *, 
	    integer *, complex *, complex *, integer *, complex *, integer *, 
	    complex *, complex *, integer *), cgemv_(char *, 
	    integer *, integer *, complex *, complex *, integer *, complex *, 
	    integer *, complex *, complex *, integer *);
    static integer nbmin;
    extern /* Subroutine */ int cswap_(integer *, complex *, integer *, 
	    complex *, integer *), ctrsm_(char *, char *, char *, char *, 
	    integer *, integer *, complex *, complex *, integer *, complex *, 
	    integer *);
    static integer jb, nb, jj, jp, nn;
    extern integer ilaenv_(integer *, char *, char *, integer *, integer *, 
	    integer *, integer *, ftnlen, ftnlen);
    extern /* Subroutine */ int xerbla_(char *, integer *);
    static integer ldwork;
    extern /* Subroutine */ int ctrtri_(char *, char *, integer *, complex *, 
	    integer *, integer *);
    static integer lwkopt;
    static logical lquery;
    static integer iws;
#define a_subscr(a_1,a_2) (a_2)*a_dim1 + a_1
#define a_ref(a_1,a_2) a[a_subscr(a_1,a_2)]


    a_dim1 = *lda;
    a_offset = 1 + a_dim1 * 1;
    a -= a_offset;
    --ipiv;
    --work;

    /* Function Body */
    *info = 0;
    nb = ilaenv_(&c__1, "CGETRI", " ", n, &c_n1, &c_n1, &c_n1, (ftnlen)6, (
	    ftnlen)1);
    lwkopt = *n * nb;
    work[1].r = (real) lwkopt, work[1].i = 0.f;
    lquery = *lwork == -1;
    if (*n < 0) {
	*info = -1;
    } else if (*lda < max(1,*n)) {
	*info = -3;
    } else if (*lwork < max(1,*n) && ! lquery) {
	*info = -6;
    }
    if (*info != 0) {
	i__1 = -(*info);
	xerbla_("CGETRI", &i__1);
	return 0;
    } else if (lquery) {
	return 0;
    }

/*     Quick return if possible */

    if (*n == 0) {
	return 0;
    }

/*     Form inv(U).  If INFO > 0 from CTRTRI, then U is singular,   
       and the inverse is not computed. */

    ctrtri_("Upper", "Non-unit", n, &a[a_offset], lda, info);
    if (*info > 0) {
	return 0;
    }

    nbmin = 2;
    ldwork = *n;
    if (nb > 1 && nb < *n) {
/* Computing MAX */
	i__1 = ldwork * nb;
	iws = max(i__1,1);
	if (*lwork < iws) {
	    nb = *lwork / ldwork;
/* Computing MAX */
	    i__1 = 2, i__2 = ilaenv_(&c__2, "CGETRI", " ", n, &c_n1, &c_n1, &
		    c_n1, (ftnlen)6, (ftnlen)1);
	    nbmin = max(i__1,i__2);
	}
    } else {
	iws = *n;
    }

/*     Solve the equation inv(A)*L = inv(U) for inv(A). */

    if (nb < nbmin || nb >= *n) {

/*        Use unblocked code. */

	for (j = *n; j >= 1; --j) {

/*           Copy current column of L to WORK and replace with zeros. */

	    i__1 = *n;
	    for (i__ = j + 1; i__ <= i__1; ++i__) {
		i__2 = i__;
		i__3 = a_subscr(i__, j);
		work[i__2].r = a[i__3].r, work[i__2].i = a[i__3].i;
		i__2 = a_subscr(i__, j);
		a[i__2].r = 0.f, a[i__2].i = 0.f;
/* L10: */
	    }

/*           Compute current column of inv(A). */

	    if (j < *n) {
		i__1 = *n - j;
		q__1.r = -1.f, q__1.i = 0.f;
		cgemv_("No transpose", n, &i__1, &q__1, &a_ref(1, j + 1), lda,
			 &work[j + 1], &c__1, &c_b2, &a_ref(1, j), &c__1);
	    }
/* L20: */
	}
    } else {

/*        Use blocked code. */

	nn = (*n - 1) / nb * nb + 1;
	i__1 = -nb;
	for (j = nn; i__1 < 0 ? j >= 1 : j <= 1; j += i__1) {
/* Computing MIN */
	    i__2 = nb, i__3 = *n - j + 1;
	    jb = min(i__2,i__3);

/*           Copy current block column of L to WORK and replace with   
             zeros. */

	    i__2 = j + jb - 1;
	    for (jj = j; jj <= i__2; ++jj) {
		i__3 = *n;
		for (i__ = jj + 1; i__ <= i__3; ++i__) {
		    i__4 = i__ + (jj - j) * ldwork;
		    i__5 = a_subscr(i__, jj);
		    work[i__4].r = a[i__5].r, work[i__4].i = a[i__5].i;
		    i__4 = a_subscr(i__, jj);
		    a[i__4].r = 0.f, a[i__4].i = 0.f;
/* L30: */
		}
/* L40: */
	    }

/*           Compute current block column of inv(A). */

	    if (j + jb <= *n) {
		i__2 = *n - j - jb + 1;
		q__1.r = -1.f, q__1.i = 0.f;
		cgemm_("No transpose", "No transpose", n, &jb, &i__2, &q__1, &
			a_ref(1, j + jb), lda, &work[j + jb], &ldwork, &c_b2, 
			&a_ref(1, j), lda);
	    }
	    ctrsm_("Right", "Lower", "No transpose", "Unit", n, &jb, &c_b2, &
		    work[j], &ldwork, &a_ref(1, j), lda);
/* L50: */
	}
    }

/*     Apply column interchanges. */

    for (j = *n - 1; j >= 1; --j) {
	jp = ipiv[j];
	if (jp != j) {
	    cswap_(n, &a_ref(1, j), &c__1, &a_ref(1, jp), &c__1);
	}
/* L60: */
    }

    work[1].r = (real) iws, work[1].i = 0.f;
    return 0;

/*     End of CGETRI */

} /* cgetri_ */

#undef a_ref
#undef a_subscr


