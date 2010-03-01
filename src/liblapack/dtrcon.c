
#include "f2c.h"

/* Subroutine */ int dtrcon_(char *norm, char *uplo, char *diag, integer *n, 
	doublereal *a, integer *lda, doublereal *rcond, doublereal *work, 
	integer *iwork, integer *info)
{
/*  -- LAPACK routine (version 3.0) --   
       Univ. of Tennessee, Univ. of California Berkeley, NAG Ltd.,   
       Courant Institute, Argonne National Lab, and Rice University   
       March 31, 1993   


    Purpose   
    =======   

    DTRCON estimates the reciprocal of the condition number of a   
    triangular matrix A, in either the 1-norm or the infinity-norm.   

    The norm of A is computed and an estimate is obtained for   
    norm(inv(A)), then the reciprocal of the condition number is   
    computed as   
       RCOND = 1 / ( norm(A) * norm(inv(A)) ).   

    Arguments   
    =========   

    NORM    (input) CHARACTER*1   
            Specifies whether the 1-norm condition number or the   
            infinity-norm condition number is required:   
            = '1' or 'O':  1-norm;   
            = 'I':         Infinity-norm.   

    UPLO    (input) CHARACTER*1   
            = 'U':  A is upper triangular;   
            = 'L':  A is lower triangular.   

    DIAG    (input) CHARACTER*1   
            = 'N':  A is non-unit triangular;   
            = 'U':  A is unit triangular.   

    N       (input) INTEGER   
            The order of the matrix A.  N >= 0.   

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

    RCOND   (output) DOUBLE PRECISION   
            The reciprocal of the condition number of the matrix A,   
            computed as RCOND = 1/(norm(A) * norm(inv(A))).   

    WORK    (workspace) DOUBLE PRECISION array, dimension (3*N)   

    IWORK   (workspace) INTEGER array, dimension (N)   

    INFO    (output) INTEGER   
            = 0:  successful exit   
            < 0:  if INFO = -i, the i-th argument had an illegal value   

    =====================================================================   


       Test the input parameters.   

       Parameter adjustments */
    /* Table of constant values */
    static integer c__1 = 1;
    
    /* System generated locals */
    integer a_dim1, a_offset, i__1;
    doublereal d__1;
    /* Local variables */
    static integer kase, kase1;
    static doublereal scale;
    extern logical lsame_(char *, char *);
    extern /* Subroutine */ int drscl_(integer *, doublereal *, doublereal *, 
	    integer *);
    static doublereal anorm;
    static logical upper;
    static doublereal xnorm;
    extern doublereal dlamch_(char *);
    extern /* Subroutine */ int dlacon_(integer *, doublereal *, doublereal *,
	     integer *, doublereal *, integer *);
    static integer ix;
    extern integer idamax_(integer *, doublereal *, integer *);
    extern /* Subroutine */ int xerbla_(char *, integer *);
    extern doublereal dlantr_(char *, char *, char *, integer *, integer *, 
	    doublereal *, integer *, doublereal *);
    static doublereal ainvnm;
    extern /* Subroutine */ int dlatrs_(char *, char *, char *, char *, 
	    integer *, doublereal *, integer *, doublereal *, doublereal *, 
	    doublereal *, integer *);
    static logical onenrm;
    static char normin[1];
    static doublereal smlnum;
    static logical nounit;


    a_dim1 = *lda;
    a_offset = 1 + a_dim1 * 1;
    a -= a_offset;
    --work;
    --iwork;

    /* Function Body */
    *info = 0;
    upper = lsame_(uplo, "U");
    onenrm = *(unsigned char *)norm == '1' || lsame_(norm, "O");
    nounit = lsame_(diag, "N");

    if (! onenrm && ! lsame_(norm, "I")) {
	*info = -1;
    } else if (! upper && ! lsame_(uplo, "L")) {
	*info = -2;
    } else if (! nounit && ! lsame_(diag, "U")) {
	*info = -3;
    } else if (*n < 0) {
	*info = -4;
    } else if (*lda < max(1,*n)) {
	*info = -6;
    }
    if (*info != 0) {
	i__1 = -(*info);
	xerbla_("DTRCON", &i__1);
	return 0;
    }

/*     Quick return if possible */

    if (*n == 0) {
	*rcond = 1.;
	return 0;
    }

    *rcond = 0.;
    smlnum = dlamch_("Safe minimum") * (doublereal) max(1,*n);

/*     Compute the norm of the triangular matrix A. */

    anorm = dlantr_(norm, uplo, diag, n, n, &a[a_offset], lda, &work[1]);

/*     Continue only if ANORM > 0. */

    if (anorm > 0.) {

/*        Estimate the norm of the inverse of A. */

	ainvnm = 0.;
	*(unsigned char *)normin = 'N';
	if (onenrm) {
	    kase1 = 1;
	} else {
	    kase1 = 2;
	}
	kase = 0;
L10:
	dlacon_(n, &work[*n + 1], &work[1], &iwork[1], &ainvnm, &kase);
	if (kase != 0) {
	    if (kase == kase1) {

/*              Multiply by inv(A). */

		dlatrs_(uplo, "No transpose", diag, normin, n, &a[a_offset], 
			lda, &work[1], &scale, &work[(*n << 1) + 1], info);
	    } else {

/*              Multiply by inv(A'). */

		dlatrs_(uplo, "Transpose", diag, normin, n, &a[a_offset], lda,
			 &work[1], &scale, &work[(*n << 1) + 1], info);
	    }
	    *(unsigned char *)normin = 'Y';

/*           Multiply by 1/SCALE if doing so will not cause overflow. */

	    if (scale != 1.) {
		ix = idamax_(n, &work[1], &c__1);
		xnorm = (d__1 = work[ix], abs(d__1));
		if (scale < xnorm * smlnum || scale == 0.) {
		    goto L20;
		}
		drscl_(n, &scale, &work[1], &c__1);
	    }
	    goto L10;
	}

/*        Compute the estimate of the reciprocal condition number. */

	if (ainvnm != 0.) {
	    *rcond = 1. / anorm / ainvnm;
	}
    }

L20:
    return 0;

/*     End of DTRCON */

} /* dtrcon_ */

