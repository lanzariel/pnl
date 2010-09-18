
#include "pnl/pnl_f2c.h"

/* Subroutine */ int dsytf2_(char *uplo, integer *n, doublereal *a, integer *
	lda, integer *ipiv, integer *info)
{
/*  -- LAPACK routine (version 3.0) --   
       Univ. of Tennessee, Univ. of California Berkeley, NAG Ltd.,   
       Courant Institute, Argonne National Lab, and Rice University   
       June 30, 1999   


    Purpose   
    =======   

    DSYTF2 computes the factorization of a real symmetric matrix A using   
    the Bunch-Kaufman diagonal pivoting method:   

       A = U*D*U'  or  A = L*D*L'   

    where U (or L) is a product of permutation and unit upper (lower)   
    triangular matrices, U' is the transpose of U, and D is symmetric and   
    block diagonal with 1-by-1 and 2-by-2 diagonal blocks.   

    This is the unblocked version of the algorithm, calling Level 2 BLAS.   

    Arguments   
    =========   

    UPLO    (input) CHARACTER*1   
            Specifies whether the upper or lower triangular part of the   
            symmetric matrix A is stored:   
            = 'U':  Upper triangular   
            = 'L':  Lower triangular   

    N       (input) INTEGER   
            The order of the matrix A.  N >= 0.   

    A       (input/output) DOUBLE PRECISION array, dimension (LDA,N)   
            On entry, the symmetric matrix A.  If UPLO = 'U', the leading   
            n-by-n upper triangular part of A contains the upper   
            triangular part of the matrix A, and the strictly lower   
            triangular part of A is not referenced.  If UPLO = 'L', the   
            leading n-by-n lower triangular part of A contains the lower   
            triangular part of the matrix A, and the strictly upper   
            triangular part of A is not referenced.   

            On exit, the block diagonal matrix D and the multipliers used   
            to obtain the factor U or L (see below for further details).   

    LDA     (input) INTEGER   
            The leading dimension of the array A.  LDA >= max(1,N).   

    IPIV    (output) INTEGER array, dimension (N)   
            Details of the interchanges and the block structure of D.   
            If IPIV(k) > 0, then rows and columns k and IPIV(k) were   
            interchanged and D(k,k) is a 1-by-1 diagonal block.   
            If UPLO = 'U' and IPIV(k) = IPIV(k-1) < 0, then rows and   
            columns k-1 and -IPIV(k) were interchanged and D(k-1:k,k-1:k)   
            is a 2-by-2 diagonal block.  If UPLO = 'L' and IPIV(k) =   
            IPIV(k+1) < 0, then rows and columns k+1 and -IPIV(k) were   
            interchanged and D(k:k+1,k:k+1) is a 2-by-2 diagonal block.   

    INFO    (output) INTEGER   
            = 0: successful exit   
            < 0: if INFO = -k, the k-th argument had an illegal value   
            > 0: if INFO = k, D(k,k) is exactly zero.  The factorization   
                 has been completed, but the block diagonal matrix D is   
                 exactly singular, and division by zero will occur if it   
                 is used to solve a system of equations.   

    Further Details   
    ===============   

    1-96 - Based on modifications by J. Lewis, Boeing Computer Services   
           Company   

    If UPLO = 'U', then A = U*D*U', where   
       U = P(n)*U(n)* ... *P(k)U(k)* ...,   
    i.e., U is a product of terms P(k)*U(k), where k decreases from n to   
    1 in steps of 1 or 2, and D is a block diagonal matrix with 1-by-1   
    and 2-by-2 diagonal blocks D(k).  P(k) is a permutation matrix as   
    defined by IPIV(k), and U(k) is a unit upper triangular matrix, such   
    that if the diagonal block D(k) is of order s (s = 1 or 2), then   

               (   I    v    0   )   k-s   
       U(k) =  (   0    I    0   )   s   
               (   0    0    I   )   n-k   
                  k-s   s   n-k   

    If s = 1, D(k) overwrites A(k,k), and v overwrites A(1:k-1,k).   
    If s = 2, the upper triangle of D(k) overwrites A(k-1,k-1), A(k-1,k),   
    and A(k,k), and v overwrites A(1:k-2,k-1:k).   

    If UPLO = 'L', then A = L*D*L', where   
       L = P(1)*L(1)* ... *P(k)*L(k)* ...,   
    i.e., L is a product of terms P(k)*L(k), where k increases from 1 to   
    n in steps of 1 or 2, and D is a block diagonal matrix with 1-by-1   
    and 2-by-2 diagonal blocks D(k).  P(k) is a permutation matrix as   
    defined by IPIV(k), and L(k) is a unit lower triangular matrix, such   
    that if the diagonal block D(k) is of order s (s = 1 or 2), then   

               (   I    0     0   )  k-1   
       L(k) =  (   0    I     0   )  s   
               (   0    v     I   )  n-k-s+1   
                  k-1   s  n-k-s+1   

    If s = 1, D(k) overwrites A(k,k), and v overwrites A(k+1:n,k).   
    If s = 2, the lower triangle of D(k) overwrites A(k,k), A(k+1,k),   
    and A(k+1,k+1), and v overwrites A(k+2:n,k:k+1).   

    =====================================================================   


       Test the input parameters.   

       Parameter adjustments */
    /* Table of constant values */
    static integer c__1 = 1;
    
    /* System generated locals */
    integer a_dim1, a_offset, i__1, i__2;
    doublereal d__1, d__2, d__3;
    /* Builtin functions */
    double sqrt(doublereal);
    /* Local variables */
    static integer imax, jmax;
    extern /* Subroutine */ int dsyr_(char *, integer *, doublereal *, 
	    doublereal *, integer *, doublereal *, integer *);
    static integer i__, j, k;
    static doublereal t, alpha;
    extern /* Subroutine */ int dscal_(integer *, doublereal *, doublereal *, 
	    integer *);
    extern logical lsame_(char *, char *);
    extern /* Subroutine */ int dswap_(integer *, doublereal *, integer *, 
	    doublereal *, integer *);
    static integer kstep;
    static logical upper;
    static doublereal r1, d11, d12, d21, d22;
    static integer kk, kp;
    static doublereal absakk, wk;
    extern integer idamax_(integer *, doublereal *, integer *);
    extern /* Subroutine */ int xerbla_(char *, integer *);
    static doublereal colmax, rowmax, wkm1, wkp1;
#define a_ref(a_1,a_2) a[(a_2)*a_dim1 + a_1]


    a_dim1 = *lda;
    a_offset = 1 + a_dim1 * 1;
    a -= a_offset;
    --ipiv;

    /* Function Body */
    *info = 0;
    upper = lsame_(uplo, "U");
    if (! upper && ! lsame_(uplo, "L")) {
	*info = -1;
    } else if (*n < 0) {
	*info = -2;
    } else if (*lda < max(1,*n)) {
	*info = -4;
    }
    if (*info != 0) {
	i__1 = -(*info);
	xerbla_("DSYTF2", &i__1);
	return 0;
    }

/*     Initialize ALPHA for use in choosing pivot block size. */

    alpha = (sqrt(17.) + 1.) / 8.;

    if (upper) {

/*        Factorize A as U*D*U' using the upper triangle of A   

          K is the main loop index, decreasing from N to 1 in steps of   
          1 or 2 */

	k = *n;
L10:

/*        If K < 1, exit from loop */

	if (k < 1) {
	    goto L70;
	}
	kstep = 1;

/*        Determine rows and columns to be interchanged and whether   
          a 1-by-1 or 2-by-2 pivot block will be used */

	absakk = (d__1 = a_ref(k, k), abs(d__1));

/*        IMAX is the row-index of the largest off-diagonal element in   
          column K, and COLMAX is its absolute value */

	if (k > 1) {
	    i__1 = k - 1;
	    imax = idamax_(&i__1, &a_ref(1, k), &c__1);
	    colmax = (d__1 = a_ref(imax, k), abs(d__1));
	} else {
	    colmax = 0.;
	}

	if (max(absakk,colmax) == 0.) {

/*           Column K is zero: set INFO and continue */

	    if (*info == 0) {
		*info = k;
	    }
	    kp = k;
	} else {
	    if (absakk >= alpha * colmax) {

/*              no interchange, use 1-by-1 pivot block */

		kp = k;
	    } else {

/*              JMAX is the column-index of the largest off-diagonal   
                element in row IMAX, and ROWMAX is its absolute value */

		i__1 = k - imax;
		jmax = imax + idamax_(&i__1, &a_ref(imax, imax + 1), lda);
		rowmax = (d__1 = a_ref(imax, jmax), abs(d__1));
		if (imax > 1) {
		    i__1 = imax - 1;
		    jmax = idamax_(&i__1, &a_ref(1, imax), &c__1);
/* Computing MAX */
		    d__2 = rowmax, d__3 = (d__1 = a_ref(jmax, imax), abs(d__1)
			    );
		    rowmax = max(d__2,d__3);
		}

		if (absakk >= alpha * colmax * (colmax / rowmax)) {

/*                 no interchange, use 1-by-1 pivot block */

		    kp = k;
		} else if ((d__1 = a_ref(imax, imax), abs(d__1)) >= alpha * 
			rowmax) {

/*                 interchange rows and columns K and IMAX, use 1-by-1   
                   pivot block */

		    kp = imax;
		} else {

/*                 interchange rows and columns K-1 and IMAX, use 2-by-2   
                   pivot block */

		    kp = imax;
		    kstep = 2;
		}
	    }

	    kk = k - kstep + 1;
	    if (kp != kk) {

/*              Interchange rows and columns KK and KP in the leading   
                submatrix A(1:k,1:k) */

		i__1 = kp - 1;
		dswap_(&i__1, &a_ref(1, kk), &c__1, &a_ref(1, kp), &c__1);
		i__1 = kk - kp - 1;
		dswap_(&i__1, &a_ref(kp + 1, kk), &c__1, &a_ref(kp, kp + 1), 
			lda);
		t = a_ref(kk, kk);
		a_ref(kk, kk) = a_ref(kp, kp);
		a_ref(kp, kp) = t;
		if (kstep == 2) {
		    t = a_ref(k - 1, k);
		    a_ref(k - 1, k) = a_ref(kp, k);
		    a_ref(kp, k) = t;
		}
	    }

/*           Update the leading submatrix */

	    if (kstep == 1) {

/*              1-by-1 pivot block D(k): column k now holds   

                W(k) = U(k)*D(k)   

                where U(k) is the k-th column of U   

                Perform a rank-1 update of A(1:k-1,1:k-1) as   

                A := A - U(k)*D(k)*U(k)' = A - W(k)*1/D(k)*W(k)' */

		r1 = 1. / a_ref(k, k);
		i__1 = k - 1;
		d__1 = -r1;
		dsyr_(uplo, &i__1, &d__1, &a_ref(1, k), &c__1, &a[a_offset], 
			lda);

/*              Store U(k) in column k */

		i__1 = k - 1;
		dscal_(&i__1, &r1, &a_ref(1, k), &c__1);
	    } else {

/*              2-by-2 pivot block D(k): columns k and k-1 now hold   

                ( W(k-1) W(k) ) = ( U(k-1) U(k) )*D(k)   

                where U(k) and U(k-1) are the k-th and (k-1)-th columns   
                of U   

                Perform a rank-2 update of A(1:k-2,1:k-2) as   

                A := A - ( U(k-1) U(k) )*D(k)*( U(k-1) U(k) )'   
                   = A - ( W(k-1) W(k) )*inv(D(k))*( W(k-1) W(k) )' */

		if (k > 2) {

		    d12 = a_ref(k - 1, k);
		    d22 = a_ref(k - 1, k - 1) / d12;
		    d11 = a_ref(k, k) / d12;
		    t = 1. / (d11 * d22 - 1.);
		    d12 = t / d12;

		    for (j = k - 2; j >= 1; --j) {
			wkm1 = d12 * (d11 * a_ref(j, k - 1) - a_ref(j, k));
			wk = d12 * (d22 * a_ref(j, k) - a_ref(j, k - 1));
			for (i__ = j; i__ >= 1; --i__) {
			    a_ref(i__, j) = a_ref(i__, j) - a_ref(i__, k) * 
				    wk - a_ref(i__, k - 1) * wkm1;
/* L20: */
			}
			a_ref(j, k) = wk;
			a_ref(j, k - 1) = wkm1;
/* L30: */
		    }

		}

	    }
	}

/*        Store details of the interchanges in IPIV */

	if (kstep == 1) {
	    ipiv[k] = kp;
	} else {
	    ipiv[k] = -kp;
	    ipiv[k - 1] = -kp;
	}

/*        Decrease K and return to the start of the main loop */

	k -= kstep;
	goto L10;

    } else {

/*        Factorize A as L*D*L' using the lower triangle of A   

          K is the main loop index, increasing from 1 to N in steps of   
          1 or 2 */

	k = 1;
L40:

/*        If K > N, exit from loop */

	if (k > *n) {
	    goto L70;
	}
	kstep = 1;

/*        Determine rows and columns to be interchanged and whether   
          a 1-by-1 or 2-by-2 pivot block will be used */

	absakk = (d__1 = a_ref(k, k), abs(d__1));

/*        IMAX is the row-index of the largest off-diagonal element in   
          column K, and COLMAX is its absolute value */

	if (k < *n) {
	    i__1 = *n - k;
	    imax = k + idamax_(&i__1, &a_ref(k + 1, k), &c__1);
	    colmax = (d__1 = a_ref(imax, k), abs(d__1));
	} else {
	    colmax = 0.;
	}

	if (max(absakk,colmax) == 0.) {

/*           Column K is zero: set INFO and continue */

	    if (*info == 0) {
		*info = k;
	    }
	    kp = k;
	} else {
	    if (absakk >= alpha * colmax) {

/*              no interchange, use 1-by-1 pivot block */

		kp = k;
	    } else {

/*              JMAX is the column-index of the largest off-diagonal   
                element in row IMAX, and ROWMAX is its absolute value */

		i__1 = imax - k;
		jmax = k - 1 + idamax_(&i__1, &a_ref(imax, k), lda);
		rowmax = (d__1 = a_ref(imax, jmax), abs(d__1));
		if (imax < *n) {
		    i__1 = *n - imax;
		    jmax = imax + idamax_(&i__1, &a_ref(imax + 1, imax), &
			    c__1);
/* Computing MAX */
		    d__2 = rowmax, d__3 = (d__1 = a_ref(jmax, imax), abs(d__1)
			    );
		    rowmax = max(d__2,d__3);
		}

		if (absakk >= alpha * colmax * (colmax / rowmax)) {

/*                 no interchange, use 1-by-1 pivot block */

		    kp = k;
		} else if ((d__1 = a_ref(imax, imax), abs(d__1)) >= alpha * 
			rowmax) {

/*                 interchange rows and columns K and IMAX, use 1-by-1   
                   pivot block */

		    kp = imax;
		} else {

/*                 interchange rows and columns K+1 and IMAX, use 2-by-2   
                   pivot block */

		    kp = imax;
		    kstep = 2;
		}
	    }

	    kk = k + kstep - 1;
	    if (kp != kk) {

/*              Interchange rows and columns KK and KP in the trailing   
                submatrix A(k:n,k:n) */

		if (kp < *n) {
		    i__1 = *n - kp;
		    dswap_(&i__1, &a_ref(kp + 1, kk), &c__1, &a_ref(kp + 1, 
			    kp), &c__1);
		}
		i__1 = kp - kk - 1;
		dswap_(&i__1, &a_ref(kk + 1, kk), &c__1, &a_ref(kp, kk + 1), 
			lda);
		t = a_ref(kk, kk);
		a_ref(kk, kk) = a_ref(kp, kp);
		a_ref(kp, kp) = t;
		if (kstep == 2) {
		    t = a_ref(k + 1, k);
		    a_ref(k + 1, k) = a_ref(kp, k);
		    a_ref(kp, k) = t;
		}
	    }

/*           Update the trailing submatrix */

	    if (kstep == 1) {

/*              1-by-1 pivot block D(k): column k now holds   

                W(k) = L(k)*D(k)   

                where L(k) is the k-th column of L */

		if (k < *n) {

/*                 Perform a rank-1 update of A(k+1:n,k+1:n) as   

                   A := A - L(k)*D(k)*L(k)' = A - W(k)*(1/D(k))*W(k)' */

		    d11 = 1. / a_ref(k, k);
		    i__1 = *n - k;
		    d__1 = -d11;
		    dsyr_(uplo, &i__1, &d__1, &a_ref(k + 1, k), &c__1, &a_ref(
			    k + 1, k + 1), lda);

/*                 Store L(k) in column K */

		    i__1 = *n - k;
		    dscal_(&i__1, &d11, &a_ref(k + 1, k), &c__1);
		}
	    } else {

/*              2-by-2 pivot block D(k) */

		if (k < *n - 1) {

/*                 Perform a rank-2 update of A(k+2:n,k+2:n) as   

                   A := A - ( (A(k) A(k+1))*D(k)**(-1) ) * (A(k) A(k+1))'   

                   where L(k) and L(k+1) are the k-th and (k+1)-th   
                   columns of L */

		    d21 = a_ref(k + 1, k);
		    d11 = a_ref(k + 1, k + 1) / d21;
		    d22 = a_ref(k, k) / d21;
		    t = 1. / (d11 * d22 - 1.);
		    d21 = t / d21;

		    i__1 = *n;
		    for (j = k + 2; j <= i__1; ++j) {

			wk = d21 * (d11 * a_ref(j, k) - a_ref(j, k + 1));
			wkp1 = d21 * (d22 * a_ref(j, k + 1) - a_ref(j, k));

			i__2 = *n;
			for (i__ = j; i__ <= i__2; ++i__) {
			    a_ref(i__, j) = a_ref(i__, j) - a_ref(i__, k) * 
				    wk - a_ref(i__, k + 1) * wkp1;
/* L50: */
			}

			a_ref(j, k) = wk;
			a_ref(j, k + 1) = wkp1;

/* L60: */
		    }
		}
	    }
	}

/*        Store details of the interchanges in IPIV */

	if (kstep == 1) {
	    ipiv[k] = kp;
	} else {
	    ipiv[k] = -kp;
	    ipiv[k + 1] = -kp;
	}

/*        Increase K and return to the start of the main loop */

	k += kstep;
	goto L40;

    }

L70:

    return 0;

/*     End of DSYTF2 */

} /* dsytf2_ */

#undef a_ref


