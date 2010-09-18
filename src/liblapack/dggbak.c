
#include "pnl/pnl_f2c.h"

/* Subroutine */ int dggbak_(char *job, char *side, integer *n, integer *ilo, 
	integer *ihi, doublereal *lscale, doublereal *rscale, integer *m, 
	doublereal *v, integer *ldv, integer *info)
{
/*  -- LAPACK routine (version 3.0) --   
       Univ. of Tennessee, Univ. of California Berkeley, NAG Ltd.,   
       Courant Institute, Argonne National Lab, and Rice University   
       September 30, 1994   


    Purpose   
    =======   

    DGGBAK forms the right or left eigenvectors of a real generalized   
    eigenvalue problem A*x = lambda*B*x, by backward transformation on   
    the computed eigenvectors of the balanced pair of matrices output by   
    DGGBAL.   

    Arguments   
    =========   

    JOB     (input) CHARACTER*1   
            Specifies the type of backward transformation required:   
            = 'N':  do nothing, return immediately;   
            = 'P':  do backward transformation for permutation only;   
            = 'S':  do backward transformation for scaling only;   
            = 'B':  do backward transformations for both permutation and   
                    scaling.   
            JOB must be the same as the argument JOB supplied to DGGBAL.   

    SIDE    (input) CHARACTER*1   
            = 'R':  V contains right eigenvectors;   
            = 'L':  V contains left eigenvectors.   

    N       (input) INTEGER   
            The number of rows of the matrix V.  N >= 0.   

    ILO     (input) INTEGER   
    IHI     (input) INTEGER   
            The integers ILO and IHI determined by DGGBAL.   
            1 <= ILO <= IHI <= N, if N > 0; ILO=1 and IHI=0, if N=0.   

    LSCALE  (input) DOUBLE PRECISION array, dimension (N)   
            Details of the permutations and/or scaling factors applied   
            to the left side of A and B, as returned by DGGBAL.   

    RSCALE  (input) DOUBLE PRECISION array, dimension (N)   
            Details of the permutations and/or scaling factors applied   
            to the right side of A and B, as returned by DGGBAL.   

    M       (input) INTEGER   
            The number of columns of the matrix V.  M >= 0.   

    V       (input/output) DOUBLE PRECISION array, dimension (LDV,M)   
            On entry, the matrix of right or left eigenvectors to be   
            transformed, as returned by DTGEVC.   
            On exit, V is overwritten by the transformed eigenvectors.   

    LDV     (input) INTEGER   
            The leading dimension of the matrix V. LDV >= max(1,N).   

    INFO    (output) INTEGER   
            = 0:  successful exit.   
            < 0:  if INFO = -i, the i-th argument had an illegal value.   

    Further Details   
    ===============   

    See R.C. Ward, Balancing the generalized eigenvalue problem,   
                   SIAM J. Sci. Stat. Comp. 2 (1981), 141-152.   

    =====================================================================   


       Test the input parameters   

       Parameter adjustments */
    /* System generated locals */
    integer v_dim1, v_offset, i__1;
    /* Local variables */
    static integer i__, k;
    extern /* Subroutine */ int dscal_(integer *, doublereal *, doublereal *, 
	    integer *);
    extern logical lsame_(char *, char *);
    extern /* Subroutine */ int dswap_(integer *, doublereal *, integer *, 
	    doublereal *, integer *);
    static logical leftv;
    extern /* Subroutine */ int xerbla_(char *, integer *);
    static logical rightv;
#define v_ref(a_1,a_2) v[(a_2)*v_dim1 + a_1]

    --lscale;
    --rscale;
    v_dim1 = *ldv;
    v_offset = 1 + v_dim1 * 1;
    v -= v_offset;

    /* Function Body */
    rightv = lsame_(side, "R");
    leftv = lsame_(side, "L");

    *info = 0;
    if (! lsame_(job, "N") && ! lsame_(job, "P") && ! lsame_(job, "S") 
	    && ! lsame_(job, "B")) {
	*info = -1;
    } else if (! rightv && ! leftv) {
	*info = -2;
    } else if (*n < 0) {
	*info = -3;
    } else if (*ilo < 1) {
	*info = -4;
    } else if (*ihi < *ilo || *ihi > max(1,*n)) {
	*info = -5;
    } else if (*m < 0) {
	*info = -6;
    } else if (*ldv < max(1,*n)) {
	*info = -10;
    }
    if (*info != 0) {
	i__1 = -(*info);
	xerbla_("DGGBAK", &i__1);
	return 0;
    }

/*     Quick return if possible */

    if (*n == 0) {
	return 0;
    }
    if (*m == 0) {
	return 0;
    }
    if (lsame_(job, "N")) {
	return 0;
    }

    if (*ilo == *ihi) {
	goto L30;
    }

/*     Backward balance */

    if (lsame_(job, "S") || lsame_(job, "B")) {

/*        Backward transformation on right eigenvectors */

	if (rightv) {
	    i__1 = *ihi;
	    for (i__ = *ilo; i__ <= i__1; ++i__) {
		dscal_(m, &rscale[i__], &v_ref(i__, 1), ldv);
/* L10: */
	    }
	}

/*        Backward transformation on left eigenvectors */

	if (leftv) {
	    i__1 = *ihi;
	    for (i__ = *ilo; i__ <= i__1; ++i__) {
		dscal_(m, &lscale[i__], &v_ref(i__, 1), ldv);
/* L20: */
	    }
	}
    }

/*     Backward permutation */

L30:
    if (lsame_(job, "P") || lsame_(job, "B")) {

/*        Backward permutation on right eigenvectors */

	if (rightv) {
	    if (*ilo == 1) {
		goto L50;
	    }

	    for (i__ = *ilo - 1; i__ >= 1; --i__) {
		k = (integer) rscale[i__];
		if (k == i__) {
		    goto L40;
		}
		dswap_(m, &v_ref(i__, 1), ldv, &v_ref(k, 1), ldv);
L40:
		;
	    }

L50:
	    if (*ihi == *n) {
		goto L70;
	    }
	    i__1 = *n;
	    for (i__ = *ihi + 1; i__ <= i__1; ++i__) {
		k = (integer) rscale[i__];
		if (k == i__) {
		    goto L60;
		}
		dswap_(m, &v_ref(i__, 1), ldv, &v_ref(k, 1), ldv);
L60:
		;
	    }
	}

/*        Backward permutation on left eigenvectors */

L70:
	if (leftv) {
	    if (*ilo == 1) {
		goto L90;
	    }
	    for (i__ = *ilo - 1; i__ >= 1; --i__) {
		k = (integer) lscale[i__];
		if (k == i__) {
		    goto L80;
		}
		dswap_(m, &v_ref(i__, 1), ldv, &v_ref(k, 1), ldv);
L80:
		;
	    }

L90:
	    if (*ihi == *n) {
		goto L110;
	    }
	    i__1 = *n;
	    for (i__ = *ihi + 1; i__ <= i__1; ++i__) {
		k = (integer) lscale[i__];
		if (k == i__) {
		    goto L100;
		}
		dswap_(m, &v_ref(i__, 1), ldv, &v_ref(k, 1), ldv);
L100:
		;
	    }
	}
    }

L110:

    return 0;

/*     End of DGGBAK */

} /* dggbak_ */

#undef v_ref


