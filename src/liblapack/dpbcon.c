/* dpbcon.f -- translated by f2c (version 20061008).
   You must link the resulting object file with libf2c:
	on Microsoft Windows system, link with libf2c.lib;
	on Linux or Unix systems, link with .../path/to/libf2c.a -lm
	or, if you install libf2c.a in a standard place, with -lf2c -lm
	-- in that order, at the end of the command line, as in
		cc *.o -lf2c -lm
	Source for libf2c is in /netlib/f2c/libf2c.zip, e.g.,

		http://www.netlib.org/f2c/libf2c.zip
*/

#include "pnl/pnl_f2c.h"

/* Table of constant values */

static int c__1 = 1;

 int dpbcon_(char *uplo, int *n, int *kd, double *
	ab, int *ldab, double *anorm, double *rcond, double *
	work, int *iwork, int *info)
{
    /* System generated locals */
    int ab_dim1, ab_offset, i__1;
    double d__1;

    /* Local variables */
    int ix, kase;
    double scale;
    extern int lsame_(char *, char *);
    int isave[3];
    extern  int drscl_(int *, double *, double *, 
	    int *);
    int upper;
    extern  int dlacn2_(int *, double *, double *, 
	     int *, double *, int *, int *);
    extern double dlamch_(char *);
    double scalel;
    extern int idamax_(int *, double *, int *);
    extern  int dlatbs_(char *, char *, char *, char *, 
	    int *, int *, double *, int *, double *, 
	    double *, double *, int *);
    double scaleu;
    extern  int xerbla_(char *, int *);
    double ainvnm;
    char normin[1];
    double smlnum;


/*  -- LAPACK routine (version 3.2) -- */
/*     Univ. of Tennessee, Univ. of California Berkeley and NAG Ltd.. */
/*     November 2006 */

/*     Modified to call DLACN2 in place of DLACON, 5 Feb 03, SJH. */

/*     .. Scalar Arguments .. */
/*     .. */
/*     .. Array Arguments .. */
/*     .. */

/*  Purpose */
/*  ======= */

/*  DPBCON estimates the reciprocal of the condition number (in the */
/*  1-norm) of a float symmetric positive definite band matrix using the */
/*  Cholesky factorization A = U**T*U or A = L*L**T computed by DPBTRF. */

/*  An estimate is obtained for norm(inv(A)), and the reciprocal of the */
/*  condition number is computed as RCOND = 1 / (ANORM * norm(inv(A))). */

/*  Arguments */
/*  ========= */

/*  UPLO    (input) CHARACTER*1 */
/*          = 'U':  Upper triangular factor stored in AB; */
/*          = 'L':  Lower triangular factor stored in AB. */

/*  N       (input) INTEGER */
/*          The order of the matrix A.  N >= 0. */

/*  KD      (input) INTEGER */
/*          The number of superdiagonals of the matrix A if UPLO = 'U', */
/*          or the number of subdiagonals if UPLO = 'L'.  KD >= 0. */

/*  AB      (input) DOUBLE PRECISION array, dimension (LDAB,N) */
/*          The triangular factor U or L from the Cholesky factorization */
/*          A = U**T*U or A = L*L**T of the band matrix A, stored in the */
/*          first KD+1 rows of the array.  The j-th column of U or L is */
/*          stored in the j-th column of the array AB as follows: */
/*          if UPLO ='U', AB(kd+1+i-j,j) = U(i,j) for MAX(1,j-kd)<=i<=j; */
/*          if UPLO ='L', AB(1+i-j,j)    = L(i,j) for j<=i<=MIN(n,j+kd). */

/*  LDAB    (input) INTEGER */
/*          The leading dimension of the array AB.  LDAB >= KD+1. */

/*  ANORM   (input) DOUBLE PRECISION */
/*          The 1-norm (or infinity-norm) of the symmetric band matrix A. */

/*  RCOND   (output) DOUBLE PRECISION */
/*          The reciprocal of the condition number of the matrix A, */
/*          computed as RCOND = 1/(ANORM * AINVNM), where AINVNM is an */
/*          estimate of the 1-norm of inv(A) computed in this routine. */

/*  WORK    (workspace) DOUBLE PRECISION array, dimension (3*N) */

/*  IWORK   (workspace) INTEGER array, dimension (N) */

/*  INFO    (output) INTEGER */
/*          = 0:  successful exit */
/*          < 0:  if INFO = -i, the i-th argument had an illegal value */

/*  ===================================================================== */

/*     .. Parameters .. */
/*     .. */
/*     .. Local Scalars .. */
/*     .. */
/*     .. Local Arrays .. */
/*     .. */
/*     .. External Functions .. */
/*     .. */
/*     .. External Subroutines .. */
/*     .. */
/*     .. Intrinsic Functions .. */
/*     .. */
/*     .. Executable Statements .. */

/*     Test the input parameters. */

    /* Parameter adjustments */
    ab_dim1 = *ldab;
    ab_offset = 1 + ab_dim1;
    ab -= ab_offset;
    --work;
    --iwork;

    /* Function Body */
    *info = 0;
    upper = lsame_(uplo, "U");
    if (! upper && ! lsame_(uplo, "L")) {
	*info = -1;
    } else if (*n < 0) {
	*info = -2;
    } else if (*kd < 0) {
	*info = -3;
    } else if (*ldab < *kd + 1) {
	*info = -5;
    } else if (*anorm < 0.) {
	*info = -6;
    }
    if (*info != 0) {
	i__1 = -(*info);
	xerbla_("DPBCON", &i__1);
	return 0;
    }

/*     Quick return if possible */

    *rcond = 0.;
    if (*n == 0) {
	*rcond = 1.;
	return 0;
    } else if (*anorm == 0.) {
	return 0;
    }

    smlnum = dlamch_("Safe minimum");

/*     Estimate the 1-norm of the inverse. */

    kase = 0;
    *(unsigned char *)normin = 'N';
L10:
    dlacn2_(n, &work[*n + 1], &work[1], &iwork[1], &ainvnm, &kase, isave);
    if (kase != 0) {
	if (upper) {

/*           Multiply by inv(U'). */

	    dlatbs_("Upper", "Transpose", "Non-unit", normin, n, kd, &ab[
		    ab_offset], ldab, &work[1], &scalel, &work[(*n << 1) + 1], 
		     info);
	    *(unsigned char *)normin = 'Y';

/*           Multiply by inv(U). */

	    dlatbs_("Upper", "No transpose", "Non-unit", normin, n, kd, &ab[
		    ab_offset], ldab, &work[1], &scaleu, &work[(*n << 1) + 1], 
		     info);
	} else {

/*           Multiply by inv(L). */

	    dlatbs_("Lower", "No transpose", "Non-unit", normin, n, kd, &ab[
		    ab_offset], ldab, &work[1], &scalel, &work[(*n << 1) + 1], 
		     info);
	    *(unsigned char *)normin = 'Y';

/*           Multiply by inv(L'). */

	    dlatbs_("Lower", "Transpose", "Non-unit", normin, n, kd, &ab[
		    ab_offset], ldab, &work[1], &scaleu, &work[(*n << 1) + 1], 
		     info);
	}

/*        Multiply by 1/SCALE if doing so will not cause overflow. */

	scale = scalel * scaleu;
	if (scale != 1.) {
	    ix = idamax_(n, &work[1], &c__1);
	    if (scale < (d__1 = work[ix], ABS(d__1)) * smlnum || scale == 0.) 
		    {
		goto L20;
	    }
	    drscl_(n, &scale, &work[1], &c__1);
	}
	goto L10;
    }

/*     Compute the estimate of the reciprocal condition number. */

    if (ainvnm != 0.) {
	*rcond = 1. / ainvnm / *anorm;
    }

L20:

    return 0;

/*     End of DPBCON */

} /* dpbcon_ */
