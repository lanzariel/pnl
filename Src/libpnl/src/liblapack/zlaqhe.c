
#include "f2c.h"

/* Subroutine */ int zlaqhe_(char *uplo, integer *n, doublecomplex *a, 
	integer *lda, doublereal *s, doublereal *scond, doublereal *amax, 
	char *equed)
{
/*  -- LAPACK auxiliary routine (version 3.0) --   
       Univ. of Tennessee, Univ. of California Berkeley, NAG Ltd.,   
       Courant Institute, Argonne National Lab, and Rice University   
       September 30, 1994   


    Purpose   
    =======   

    ZLAQHE equilibrates a Hermitian matrix A using the scaling factors   
    in the vector S.   

    Arguments   
    =========   

    UPLO    (input) CHARACTER*1   
            Specifies whether the upper or lower triangular part of the   
            Hermitian matrix A is stored.   
            = 'U':  Upper triangular   
            = 'L':  Lower triangular   

    N       (input) INTEGER   
            The order of the matrix A.  N >= 0.   

    A       (input/output) COMPLEX*16 array, dimension (LDA,N)   
            On entry, the Hermitian matrix A.  If UPLO = 'U', the leading   
            n by n upper triangular part of A contains the upper   
            triangular part of the matrix A, and the strictly lower   
            triangular part of A is not referenced.  If UPLO = 'L', the   
            leading n by n lower triangular part of A contains the lower   
            triangular part of the matrix A, and the strictly upper   
            triangular part of A is not referenced.   

            On exit, if EQUED = 'Y', the equilibrated matrix:   
            diag(S) * A * diag(S).   

    LDA     (input) INTEGER   
            The leading dimension of the array A.  LDA >= max(N,1).   

    S       (input) DOUBLE PRECISION array, dimension (N)   
            The scale factors for A.   

    SCOND   (input) DOUBLE PRECISION   
            Ratio of the smallest S(i) to the largest S(i).   

    AMAX    (input) DOUBLE PRECISION   
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
    integer a_dim1, a_offset, i__1, i__2, i__3, i__4;
    doublereal d__1;
    doublecomplex z__1;
    /* Local variables */
    static integer i__, j;
    static doublereal large;
    extern logical lsame_(char *, char *);
    static doublereal small, cj;
    extern doublereal dlamch_(char *);
#define a_subscr(a_1,a_2) (a_2)*a_dim1 + a_1
#define a_ref(a_1,a_2) a[a_subscr(a_1,a_2)]

    a_dim1 = *lda;
    a_offset = 1 + a_dim1 * 1;
    a -= a_offset;
    --s;

    /* Function Body */
    if (*n <= 0) {
	*(unsigned char *)equed = 'N';
	return 0;
    }

/*     Initialize LARGE and SMALL. */

    small = dlamch_("Safe minimum") / dlamch_("Precision");
    large = 1. / small;

    if (*scond >= .1 && *amax >= small && *amax <= large) {

/*        No equilibration */

	*(unsigned char *)equed = 'N';
    } else {

/*        Replace A by diag(S) * A * diag(S). */

	if (lsame_(uplo, "U")) {

/*           Upper triangle of A is stored. */

	    i__1 = *n;
	    for (j = 1; j <= i__1; ++j) {
		cj = s[j];
		i__2 = j - 1;
		for (i__ = 1; i__ <= i__2; ++i__) {
		    i__3 = a_subscr(i__, j);
		    d__1 = cj * s[i__];
		    i__4 = a_subscr(i__, j);
		    z__1.r = d__1 * a[i__4].r, z__1.i = d__1 * a[i__4].i;
		    a[i__3].r = z__1.r, a[i__3].i = z__1.i;
/* L10: */
		}
		i__2 = a_subscr(j, j);
		i__3 = a_subscr(j, j);
		d__1 = cj * cj * a[i__3].r;
		a[i__2].r = d__1, a[i__2].i = 0.;
/* L20: */
	    }
	} else {

/*           Lower triangle of A is stored. */

	    i__1 = *n;
	    for (j = 1; j <= i__1; ++j) {
		cj = s[j];
		i__2 = a_subscr(j, j);
		i__3 = a_subscr(j, j);
		d__1 = cj * cj * a[i__3].r;
		a[i__2].r = d__1, a[i__2].i = 0.;
		i__2 = *n;
		for (i__ = j + 1; i__ <= i__2; ++i__) {
		    i__3 = a_subscr(i__, j);
		    d__1 = cj * s[i__];
		    i__4 = a_subscr(i__, j);
		    z__1.r = d__1 * a[i__4].r, z__1.i = d__1 * a[i__4].i;
		    a[i__3].r = z__1.r, a[i__3].i = z__1.i;
/* L30: */
		}
/* L40: */
	    }
	}
	*(unsigned char *)equed = 'Y';
    }

    return 0;

/*     End of ZLAQHE */

} /* zlaqhe_ */

#undef a_ref
#undef a_subscr


