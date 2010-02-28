
#include "f2c.h"

/* Subroutine */ int sspevd_(char *jobz, char *uplo, integer *n, real *ap, 
	real *w, real *z__, integer *ldz, real *work, integer *lwork, integer 
	*iwork, integer *liwork, integer *info)
{
/*  -- LAPACK driver routine (version 3.0) --   
       Univ. of Tennessee, Univ. of California Berkeley, NAG Ltd.,   
       Courant Institute, Argonne National Lab, and Rice University   
       June 30, 1999   


    Purpose   
    =======   

    SSPEVD computes all the eigenvalues and, optionally, eigenvectors   
    of a real symmetric matrix A in packed storage. If eigenvectors are   
    desired, it uses a divide and conquer algorithm.   

    The divide and conquer algorithm makes very mild assumptions about   
    floating point arithmetic. It will work on machines with a guard   
    digit in add/subtract, or on those binary machines without guard   
    digits which subtract like the Cray X-MP, Cray Y-MP, Cray C-90, or   
    Cray-2. It could conceivably fail on hexadecimal or decimal machines   
    without guard digits, but we know of none.   

    Arguments   
    =========   

    JOBZ    (input) CHARACTER*1   
            = 'N':  Compute eigenvalues only;   
            = 'V':  Compute eigenvalues and eigenvectors.   

    UPLO    (input) CHARACTER*1   
            = 'U':  Upper triangle of A is stored;   
            = 'L':  Lower triangle of A is stored.   

    N       (input) INTEGER   
            The order of the matrix A.  N >= 0.   

    AP      (input/output) REAL array, dimension (N*(N+1)/2)   
            On entry, the upper or lower triangle of the symmetric matrix   
            A, packed columnwise in a linear array.  The j-th column of A   
            is stored in the array AP as follows:   
            if UPLO = 'U', AP(i + (j-1)*j/2) = A(i,j) for 1<=i<=j;   
            if UPLO = 'L', AP(i + (j-1)*(2*n-j)/2) = A(i,j) for j<=i<=n.   

            On exit, AP is overwritten by values generated during the   
            reduction to tridiagonal form.  If UPLO = 'U', the diagonal   
            and first superdiagonal of the tridiagonal matrix T overwrite   
            the corresponding elements of A, and if UPLO = 'L', the   
            diagonal and first subdiagonal of T overwrite the   
            corresponding elements of A.   

    W       (output) REAL array, dimension (N)   
            If INFO = 0, the eigenvalues in ascending order.   

    Z       (output) REAL array, dimension (LDZ, N)   
            If JOBZ = 'V', then if INFO = 0, Z contains the orthonormal   
            eigenvectors of the matrix A, with the i-th column of Z   
            holding the eigenvector associated with W(i).   
            If JOBZ = 'N', then Z is not referenced.   

    LDZ     (input) INTEGER   
            The leading dimension of the array Z.  LDZ >= 1, and if   
            JOBZ = 'V', LDZ >= max(1,N).   

    WORK    (workspace/output) REAL array,   
                                           dimension (LWORK)   
            On exit, if INFO = 0, WORK(1) returns the optimal LWORK.   

    LWORK   (input) INTEGER   
            The dimension of the array WORK.   
            If N <= 1,               LWORK must be at least 1.   
            If JOBZ = 'N' and N > 1, LWORK must be at least 2*N.   
            If JOBZ = 'V' and N > 1, LWORK must be at least   
                                                   1 + 6*N + N**2.   

            If LWORK = -1, then a workspace query is assumed; the routine   
            only calculates the optimal size of the WORK array, returns   
            this value as the first entry of the WORK array, and no error   
            message related to LWORK is issued by XERBLA.   

    IWORK   (workspace/output) INTEGER array, dimension (LIWORK)   
            On exit, if INFO = 0, IWORK(1) returns the optimal LIWORK.   

    LIWORK  (input) INTEGER   
            The dimension of the array IWORK.   
            If JOBZ  = 'N' or N <= 1, LIWORK must be at least 1.   
            If JOBZ  = 'V' and N > 1, LIWORK must be at least 3 + 5*N.   

            If LIWORK = -1, then a workspace query is assumed; the   
            routine only calculates the optimal size of the IWORK array,   
            returns this value as the first entry of the IWORK array, and   
            no error message related to LIWORK is issued by XERBLA.   

    INFO    (output) INTEGER   
            = 0:  successful exit   
            < 0:  if INFO = -i, the i-th argument had an illegal value.   
            > 0:  if INFO = i, the algorithm failed to converge; i   
                  off-diagonal elements of an intermediate tridiagonal   
                  form did not converge to zero.   

    =====================================================================   


       Test the input parameters.   

       Parameter adjustments */
    /* Table of constant values */
    static integer c__1 = 1;
    
    /* System generated locals */
    integer z_dim1, z_offset, i__1;
    real r__1;
    /* Builtin functions */
    double sqrt(doublereal);
    /* Local variables */
    static integer inde;
    static real anrm, rmin, rmax, sigma;
    extern logical lsame_(char *, char *);
    static integer iinfo;
    extern /* Subroutine */ int sscal_(integer *, real *, real *, integer *);
    static integer lwmin;
    static logical wantz;
    static integer iscale;
    extern doublereal slamch_(char *);
    static real safmin;
    extern /* Subroutine */ int xerbla_(char *, integer *);
    static real bignum;
    static integer indtau;
    extern /* Subroutine */ int sstedc_(char *, integer *, real *, real *, 
	    real *, integer *, real *, integer *, integer *, integer *, 
	    integer *);
    static integer indwrk, liwmin;
    extern doublereal slansp_(char *, char *, integer *, real *, real *);
    extern /* Subroutine */ int ssterf_(integer *, real *, real *, integer *);
    static integer llwork;
    static real smlnum;
    extern /* Subroutine */ int ssptrd_(char *, integer *, real *, real *, 
	    real *, real *, integer *);
    static logical lquery;
    extern /* Subroutine */ int sopmtr_(char *, char *, char *, integer *, 
	    integer *, real *, real *, real *, integer *, real *, integer *);
    static real eps;
#define z___ref(a_1,a_2) z__[(a_2)*z_dim1 + a_1]


    --ap;
    --w;
    z_dim1 = *ldz;
    z_offset = 1 + z_dim1 * 1;
    z__ -= z_offset;
    --work;
    --iwork;

    /* Function Body */
    wantz = lsame_(jobz, "V");
    lquery = *lwork == -1 || *liwork == -1;

    *info = 0;
    if (*n <= 1) {
	liwmin = 1;
	lwmin = 1;
    } else {
	if (wantz) {
	    liwmin = *n * 5 + 3;
/* Computing 2nd power */
	    i__1 = *n;
	    lwmin = *n * 6 + 1 + i__1 * i__1;
	} else {
	    liwmin = 1;
	    lwmin = *n << 1;
	}
    }
    if (! (wantz || lsame_(jobz, "N"))) {
	*info = -1;
    } else if (! (lsame_(uplo, "U") || lsame_(uplo, 
	    "L"))) {
	*info = -2;
    } else if (*n < 0) {
	*info = -3;
    } else if (*ldz < 1 || wantz && *ldz < *n) {
	*info = -7;
    } else if (*lwork < lwmin && ! lquery) {
	*info = -9;
    } else if (*liwork < liwmin && ! lquery) {
	*info = -11;
    }

    if (*info == 0) {
	work[1] = (real) lwmin;
	iwork[1] = liwmin;
    }

    if (*info != 0) {
	i__1 = -(*info);
	xerbla_("SSPEVD", &i__1);
	return 0;
    } else if (lquery) {
	return 0;
    }

/*     Quick return if possible */

    if (*n == 0) {
	return 0;
    }

    if (*n == 1) {
	w[1] = ap[1];
	if (wantz) {
	    z___ref(1, 1) = 1.f;
	}
	return 0;
    }

/*     Get machine constants. */

    safmin = slamch_("Safe minimum");
    eps = slamch_("Precision");
    smlnum = safmin / eps;
    bignum = 1.f / smlnum;
    rmin = sqrt(smlnum);
    rmax = sqrt(bignum);

/*     Scale matrix to allowable range, if necessary. */

    anrm = slansp_("M", uplo, n, &ap[1], &work[1]);
    iscale = 0;
    if (anrm > 0.f && anrm < rmin) {
	iscale = 1;
	sigma = rmin / anrm;
    } else if (anrm > rmax) {
	iscale = 1;
	sigma = rmax / anrm;
    }
    if (iscale == 1) {
	i__1 = *n * (*n + 1) / 2;
	sscal_(&i__1, &sigma, &ap[1], &c__1);
    }

/*     Call SSPTRD to reduce symmetric packed matrix to tridiagonal form. */

    inde = 1;
    indtau = inde + *n;
    ssptrd_(uplo, n, &ap[1], &w[1], &work[inde], &work[indtau], &iinfo);

/*     For eigenvalues only, call SSTERF.  For eigenvectors, first call   
       SSTEDC to generate the eigenvector matrix, WORK(INDWRK), of the   
       tridiagonal matrix, then call SOPMTR to multiply it by the   
       Householder transformations represented in AP. */

    if (! wantz) {
	ssterf_(n, &w[1], &work[inde], info);
    } else {
	indwrk = indtau + *n;
	llwork = *lwork - indwrk + 1;
	sstedc_("I", n, &w[1], &work[inde], &z__[z_offset], ldz, &work[indwrk]
		, &llwork, &iwork[1], liwork, info);
	sopmtr_("L", uplo, "N", n, n, &ap[1], &work[indtau], &z__[z_offset], 
		ldz, &work[indwrk], &iinfo);
    }

/*     If matrix was scaled, then rescale eigenvalues appropriately. */

    if (iscale == 1) {
	r__1 = 1.f / sigma;
	sscal_(n, &r__1, &w[1], &c__1);
    }

    work[1] = (real) lwmin;
    iwork[1] = liwmin;
    return 0;

/*     End of SSPEVD */

} /* sspevd_ */

#undef z___ref


