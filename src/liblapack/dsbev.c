
#include "pnl/pnl_f2c.h"

/* Subroutine */ int dsbev_(char *jobz, char *uplo, integer *n, integer *kd, 
	doublereal *ab, integer *ldab, doublereal *w, doublereal *z__, 
	integer *ldz, doublereal *work, integer *info)
{
/*  -- LAPACK driver routine (version 3.0) --   
       Univ. of Tennessee, Univ. of California Berkeley, NAG Ltd.,   
       Courant Institute, Argonne National Lab, and Rice University   
       June 30, 1999   


    Purpose   
    =======   

    DSBEV computes all the eigenvalues and, optionally, eigenvectors of   
    a real symmetric band matrix A.   

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

    KD      (input) INTEGER   
            The number of superdiagonals of the matrix A if UPLO = 'U',   
            or the number of subdiagonals if UPLO = 'L'.  KD >= 0.   

    AB      (input/output) DOUBLE PRECISION array, dimension (LDAB, N)   
            On entry, the upper or lower triangle of the symmetric band   
            matrix A, stored in the first KD+1 rows of the array.  The   
            j-th column of A is stored in the j-th column of the array AB   
            as follows:   
            if UPLO = 'U', AB(kd+1+i-j,j) = A(i,j) for max(1,j-kd)<=i<=j;   
            if UPLO = 'L', AB(1+i-j,j)    = A(i,j) for j<=i<=min(n,j+kd).   

            On exit, AB is overwritten by values generated during the   
            reduction to tridiagonal form.  If UPLO = 'U', the first   
            superdiagonal and the diagonal of the tridiagonal matrix T   
            are returned in rows KD and KD+1 of AB, and if UPLO = 'L',   
            the diagonal and first subdiagonal of T are returned in the   
            first two rows of AB.   

    LDAB    (input) INTEGER   
            The leading dimension of the array AB.  LDAB >= KD + 1.   

    W       (output) DOUBLE PRECISION array, dimension (N)   
            If INFO = 0, the eigenvalues in ascending order.   

    Z       (output) DOUBLE PRECISION array, dimension (LDZ, N)   
            If JOBZ = 'V', then if INFO = 0, Z contains the orthonormal   
            eigenvectors of the matrix A, with the i-th column of Z   
            holding the eigenvector associated with W(i).   
            If JOBZ = 'N', then Z is not referenced.   

    LDZ     (input) INTEGER   
            The leading dimension of the array Z.  LDZ >= 1, and if   
            JOBZ = 'V', LDZ >= max(1,N).   

    WORK    (workspace) DOUBLE PRECISION array, dimension (max(1,3*N-2))   

    INFO    (output) INTEGER   
            = 0:  successful exit   
            < 0:  if INFO = -i, the i-th argument had an illegal value   
            > 0:  if INFO = i, the algorithm failed to converge; i   
                  off-diagonal elements of an intermediate tridiagonal   
                  form did not converge to zero.   

    =====================================================================   


       Test the input parameters.   

       Parameter adjustments */
    /* Table of constant values */
    static doublereal c_b11 = 1.;
    static integer c__1 = 1;
    
    /* System generated locals */
    integer ab_dim1, ab_offset, z_dim1, z_offset, i__1;
    doublereal d__1;
    /* Builtin functions */
    double sqrt(doublereal);
    /* Local variables */
    static integer inde;
    static doublereal anrm;
    static integer imax;
    static doublereal rmin, rmax;
    extern /* Subroutine */ int dscal_(integer *, doublereal *, doublereal *, 
	    integer *);
    static doublereal sigma;
    extern logical lsame_(char *, char *);
    static integer iinfo;
    static logical lower, wantz;
    extern doublereal dlamch_(char *);
    static integer iscale;
    extern /* Subroutine */ int dlascl_(char *, integer *, integer *, 
	    doublereal *, doublereal *, integer *, integer *, doublereal *, 
	    integer *, integer *);
    extern doublereal dlansb_(char *, char *, integer *, integer *, 
	    doublereal *, integer *, doublereal *);
    static doublereal safmin;
    extern /* Subroutine */ int xerbla_(char *, integer *);
    static doublereal bignum;
    extern /* Subroutine */ int dsbtrd_(char *, char *, integer *, integer *, 
	    doublereal *, integer *, doublereal *, doublereal *, doublereal *,
	     integer *, doublereal *, integer *), dsterf_(
	    integer *, doublereal *, doublereal *, integer *);
    static integer indwrk;
    extern /* Subroutine */ int dsteqr_(char *, integer *, doublereal *, 
	    doublereal *, doublereal *, integer *, doublereal *, integer *);
    static doublereal smlnum, eps;
#define z___ref(a_1,a_2) z__[(a_2)*z_dim1 + a_1]
#define ab_ref(a_1,a_2) ab[(a_2)*ab_dim1 + a_1]


    ab_dim1 = *ldab;
    ab_offset = 1 + ab_dim1 * 1;
    ab -= ab_offset;
    --w;
    z_dim1 = *ldz;
    z_offset = 1 + z_dim1 * 1;
    z__ -= z_offset;
    --work;

    /* Function Body */
    wantz = lsame_(jobz, "V");
    lower = lsame_(uplo, "L");

    *info = 0;
    if (! (wantz || lsame_(jobz, "N"))) {
	*info = -1;
    } else if (! (lower || lsame_(uplo, "U"))) {
	*info = -2;
    } else if (*n < 0) {
	*info = -3;
    } else if (*kd < 0) {
	*info = -4;
    } else if (*ldab < *kd + 1) {
	*info = -6;
    } else if (*ldz < 1 || wantz && *ldz < *n) {
	*info = -9;
    }

    if (*info != 0) {
	i__1 = -(*info);
	xerbla_("DSBEV ", &i__1);
	return 0;
    }

/*     Quick return if possible */

    if (*n == 0) {
	return 0;
    }

    if (*n == 1) {
	if (lower) {
	    w[1] = ab_ref(1, 1);
	} else {
	    w[1] = ab_ref(*kd + 1, 1);
	}
	if (wantz) {
	    z___ref(1, 1) = 1.;
	}
	return 0;
    }

/*     Get machine constants. */

    safmin = dlamch_("Safe minimum");
    eps = dlamch_("Precision");
    smlnum = safmin / eps;
    bignum = 1. / smlnum;
    rmin = sqrt(smlnum);
    rmax = sqrt(bignum);

/*     Scale matrix to allowable range, if necessary. */

    anrm = dlansb_("M", uplo, n, kd, &ab[ab_offset], ldab, &work[1]);
    iscale = 0;
    if (anrm > 0. && anrm < rmin) {
	iscale = 1;
	sigma = rmin / anrm;
    } else if (anrm > rmax) {
	iscale = 1;
	sigma = rmax / anrm;
    }
    if (iscale == 1) {
	if (lower) {
	    dlascl_("B", kd, kd, &c_b11, &sigma, n, n, &ab[ab_offset], ldab, 
		    info);
	} else {
	    dlascl_("Q", kd, kd, &c_b11, &sigma, n, n, &ab[ab_offset], ldab, 
		    info);
	}
    }

/*     Call DSBTRD to reduce symmetric band matrix to tridiagonal form. */

    inde = 1;
    indwrk = inde + *n;
    dsbtrd_(jobz, uplo, n, kd, &ab[ab_offset], ldab, &w[1], &work[inde], &z__[
	    z_offset], ldz, &work[indwrk], &iinfo);

/*     For eigenvalues only, call DSTERF.  For eigenvectors, call SSTEQR. */

    if (! wantz) {
	dsterf_(n, &w[1], &work[inde], info);
    } else {
	dsteqr_(jobz, n, &w[1], &work[inde], &z__[z_offset], ldz, &work[
		indwrk], info);
    }

/*     If matrix was scaled, then rescale eigenvalues appropriately. */

    if (iscale == 1) {
	if (*info == 0) {
	    imax = *n;
	} else {
	    imax = *info - 1;
	}
	d__1 = 1. / sigma;
	dscal_(&imax, &d__1, &w[1], &c__1);
    }

    return 0;

/*     End of DSBEV */

} /* dsbev_ */

#undef ab_ref
#undef z___ref


