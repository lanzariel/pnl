
#include "f2c.h"

/* Subroutine */ int claqhb_(char *uplo, integer *n, integer *kd, complex *ab,
	 integer *ldab, real *s, real *scond, real *amax, char *equed)
{
/*  -- LAPACK auxiliary routine (version 3.0) --   
       Univ. of Tennessee, Univ. of California Berkeley, NAG Ltd.,   
       Courant Institute, Argonne National Lab, and Rice University   
       September 30, 1994   


    Purpose   
    =======   

    CLAQHB equilibrates a symmetric band matrix A using the scaling   
    factors in the vector S.   

    Arguments   
    =========   

    UPLO    (input) CHARACTER*1   
            Specifies whether the upper or lower triangular part of the   
            symmetric matrix A is stored.   
            = 'U':  Upper triangular   
            = 'L':  Lower triangular   

    N       (input) INTEGER   
            The order of the matrix A.  N >= 0.   

    KD      (input) INTEGER   
            The number of super-diagonals of the matrix A if UPLO = 'U',   
            or the number of sub-diagonals if UPLO = 'L'.  KD >= 0.   

    AB      (input/output) COMPLEX array, dimension (LDAB,N)   
            On entry, the upper or lower triangle of the symmetric band   
            matrix A, stored in the first KD+1 rows of the array.  The   
            j-th column of A is stored in the j-th column of the array AB   
            as follows:   
            if UPLO = 'U', AB(kd+1+i-j,j) = A(i,j) for max(1,j-kd)<=i<=j;   
            if UPLO = 'L', AB(1+i-j,j)    = A(i,j) for j<=i<=min(n,j+kd).   

            On exit, if INFO = 0, the triangular factor U or L from the   
            Cholesky factorization A = U'*U or A = L*L' of the band   
            matrix A, in the same storage format as A.   

    LDAB    (input) INTEGER   
            The leading dimension of the array AB.  LDAB >= KD+1.   

    S       (output) REAL array, dimension (N)   
            The scale factors for A.   

    SCOND   (input) REAL   
            Ratio of the smallest S(i) to the largest S(i).   

    AMAX    (input) REAL   
            Absolute value of largest matrix entry.   

    EQUED   (output) CHARACTER*1   
            Specifies whether or not equilibration was done.   
            = 'N':  No equilibration.   
            = 'Y':  Equilibration was done, i.e., A has been replaced by   
                    diag(S) * A * diag(S).   

    Internal Parameters   
    ===================   

    THRESH is a threshold value used to decide if scaling should be done   
    based on the ratio of the scaling factors.  If SCOND < THRESH,   
    scaling is done.   

    LARGE and SMALL are threshold values used to decide if scaling should   
    be done based on the absolute size of the largest matrix element.   
    If AMAX > LARGE or AMAX < SMALL, scaling is done.   

    =====================================================================   


       Quick return if possible   

       Parameter adjustments */
    /* System generated locals */
    integer ab_dim1, ab_offset, i__1, i__2, i__3, i__4;
    real r__1;
    complex q__1;
    /* Local variables */
    static integer i__, j;
    static real large;
    extern logical lsame_(char *, char *);
    static real small, cj;
    extern doublereal slamch_(char *);
#define ab_subscr(a_1,a_2) (a_2)*ab_dim1 + a_1
#define ab_ref(a_1,a_2) ab[ab_subscr(a_1,a_2)]

    ab_dim1 = *ldab;
    ab_offset = 1 + ab_dim1 * 1;
    ab -= ab_offset;
    --s;

    /* Function Body */
    if (*n <= 0) {
	*(unsigned char *)equed = 'N';
	return 0;
    }

/*     Initialize LARGE and SMALL. */

    small = slamch_("Safe minimum") / slamch_("Precision");
    large = 1.f / small;

    if (*scond >= .1f && *amax >= small && *amax <= large) {

/*        No equilibration */

	*(unsigned char *)equed = 'N';
    } else {

/*        Replace A by diag(S) * A * diag(S). */

	if (lsame_(uplo, "U")) {

/*           Upper triangle of A is stored in band format. */

	    i__1 = *n;
	    for (j = 1; j <= i__1; ++j) {
		cj = s[j];
/* Computing MAX */
		i__2 = 1, i__3 = j - *kd;
		i__4 = j - 1;
		for (i__ = max(i__2,i__3); i__ <= i__4; ++i__) {
		    i__2 = ab_subscr(*kd + 1 + i__ - j, j);
		    r__1 = cj * s[i__];
		    i__3 = ab_subscr(*kd + 1 + i__ - j, j);
		    q__1.r = r__1 * ab[i__3].r, q__1.i = r__1 * ab[i__3].i;
		    ab[i__2].r = q__1.r, ab[i__2].i = q__1.i;
/* L10: */
		}
		i__4 = ab_subscr(*kd + 1, j);
		i__2 = ab_subscr(*kd + 1, j);
		r__1 = cj * cj * ab[i__2].r;
		ab[i__4].r = r__1, ab[i__4].i = 0.f;
/* L20: */
	    }
	} else {

/*           Lower triangle of A is stored. */

	    i__1 = *n;
	    for (j = 1; j <= i__1; ++j) {
		cj = s[j];
		i__4 = ab_subscr(1, j);
		i__2 = ab_subscr(1, j);
		r__1 = cj * cj * ab[i__2].r;
		ab[i__4].r = r__1, ab[i__4].i = 0.f;
/* Computing MIN */
		i__2 = *n, i__3 = j + *kd;
		i__4 = min(i__2,i__3);
		for (i__ = j + 1; i__ <= i__4; ++i__) {
		    i__2 = ab_subscr(i__ + 1 - j, j);
		    r__1 = cj * s[i__];
		    i__3 = ab_subscr(i__ + 1 - j, j);
		    q__1.r = r__1 * ab[i__3].r, q__1.i = r__1 * ab[i__3].i;
		    ab[i__2].r = q__1.r, ab[i__2].i = q__1.i;
/* L30: */
		}
/* L40: */
	    }
	}
	*(unsigned char *)equed = 'Y';
    }

    return 0;

/*     End of CLAQHB */

} /* claqhb_ */

#undef ab_ref
#undef ab_subscr


