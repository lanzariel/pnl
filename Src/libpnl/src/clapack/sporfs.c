#include "blaswrap.h"
#include "f2c.h"

/* Subroutine */ int sporfs_(char *uplo, integer *n, integer *nrhs, real *a, 
	integer *lda, real *af, integer *ldaf, real *b, integer *ldb, real *x,
	 integer *ldx, real *ferr, real *berr, real *work, integer *iwork, 
	integer *info)
{
/*  -- LAPACK routine (version 3.0) --   
       Univ. of Tennessee, Univ. of California Berkeley, NAG Ltd.,   
       Courant Institute, Argonne National Lab, and Rice University   
       September 30, 1994   


    Purpose   
    =======   

    SPORFS improves the computed solution to a system of linear   
    equations when the coefficient matrix is symmetric positive definite,   
    and provides error bounds and backward error estimates for the   
    solution.   

    Arguments   
    =========   

    UPLO    (input) CHARACTER*1   
            = 'U':  Upper triangle of A is stored;   
            = 'L':  Lower triangle of A is stored.   

    N       (input) INTEGER   
            The order of the matrix A.  N >= 0.   

    NRHS    (input) INTEGER   
            The number of right hand sides, i.e., the number of columns   
            of the matrices B and X.  NRHS >= 0.   

    A       (input) REAL array, dimension (LDA,N)   
            The symmetric matrix A.  If UPLO = 'U', the leading N-by-N   
            upper triangular part of A contains the upper triangular part   
            of the matrix A, and the strictly lower triangular part of A   
            is not referenced.  If UPLO = 'L', the leading N-by-N lower   
            triangular part of A contains the lower triangular part of   
            the matrix A, and the strictly upper triangular part of A is   
            not referenced.   

    LDA     (input) INTEGER   
            The leading dimension of the array A.  LDA >= max(1,N).   

    AF      (input) REAL array, dimension (LDAF,N)   
            The triangular factor U or L from the Cholesky factorization   
            A = U**T*U or A = L*L**T, as computed by SPOTRF.   

    LDAF    (input) INTEGER   
            The leading dimension of the array AF.  LDAF >= max(1,N).   

    B       (input) REAL array, dimension (LDB,NRHS)   
            The right hand side matrix B.   

    LDB     (input) INTEGER   
            The leading dimension of the array B.  LDB >= max(1,N).   

    X       (input/output) REAL array, dimension (LDX,NRHS)   
            On entry, the solution matrix X, as computed by SPOTRS.   
            On exit, the improved solution matrix X.   

    LDX     (input) INTEGER   
            The leading dimension of the array X.  LDX >= max(1,N).   

    FERR    (output) REAL array, dimension (NRHS)   
            The estimated forward error bound for each solution vector   
            X(j) (the j-th column of the solution matrix X).   
            If XTRUE is the true solution corresponding to X(j), FERR(j)   
            is an estimated upper bound for the magnitude of the largest   
            element in (X(j) - XTRUE) divided by the magnitude of the   
            largest element in X(j).  The estimate is as reliable as   
            the estimate for RCOND, and is almost always a slight   
            overestimate of the true error.   

    BERR    (output) REAL array, dimension (NRHS)   
            The componentwise relative backward error of each solution   
            vector X(j) (i.e., the smallest relative change in   
            any element of A or B that makes X(j) an exact solution).   

    WORK    (workspace) REAL array, dimension (3*N)   

    IWORK   (workspace) INTEGER array, dimension (N)   

    INFO    (output) INTEGER   
            = 0:  successful exit   
            < 0:  if INFO = -i, the i-th argument had an illegal value   

    Internal Parameters   
    ===================   

    ITMAX is the maximum number of steps of iterative refinement.   

    =====================================================================   


       Test the input parameters.   

       Parameter adjustments */
    /* Table of constant values */
    static integer c__1 = 1;
    static real c_b12 = -1.f;
    static real c_b14 = 1.f;
    
    /* System generated locals */
    integer a_dim1, a_offset, af_dim1, af_offset, b_dim1, b_offset, x_dim1, 
	    x_offset, i__1, i__2, i__3;
    real r__1, r__2, r__3;
    /* Local variables */
    static integer kase;
    static real safe1, safe2;
    static integer i__, j, k;
    static real s;
    extern logical lsame_(char *, char *);
    static integer count;
    static logical upper;
    extern /* Subroutine */ int scopy_(integer *, real *, integer *, real *, 
	    integer *), saxpy_(integer *, real *, real *, integer *, real *, 
	    integer *), ssymv_(char *, integer *, real *, real *, integer *, 
	    real *, integer *, real *, real *, integer *);
    static real xk;
    extern doublereal slamch_(char *);
    static integer nz;
    static real safmin;
    extern /* Subroutine */ int xerbla_(char *, integer *), slacon_(
	    integer *, real *, real *, integer *, real *, integer *);
    static real lstres;
    extern /* Subroutine */ int spotrs_(char *, integer *, integer *, real *, 
	    integer *, real *, integer *, integer *);
    static real eps;
#define a_ref(a_1,a_2) a[(a_2)*a_dim1 + a_1]
#define b_ref(a_1,a_2) b[(a_2)*b_dim1 + a_1]
#define x_ref(a_1,a_2) x[(a_2)*x_dim1 + a_1]


    a_dim1 = *lda;
    a_offset = 1 + a_dim1 * 1;
    a -= a_offset;
    af_dim1 = *ldaf;
    af_offset = 1 + af_dim1 * 1;
    af -= af_offset;
    b_dim1 = *ldb;
    b_offset = 1 + b_dim1 * 1;
    b -= b_offset;
    x_dim1 = *ldx;
    x_offset = 1 + x_dim1 * 1;
    x -= x_offset;
    --ferr;
    --berr;
    --work;
    --iwork;

    /* Function Body */
    *info = 0;
    upper = lsame_(uplo, "U");
    if (! upper && ! lsame_(uplo, "L")) {
	*info = -1;
    } else if (*n < 0) {
	*info = -2;
    } else if (*nrhs < 0) {
	*info = -3;
    } else if (*lda < max(1,*n)) {
	*info = -5;
    } else if (*ldaf < max(1,*n)) {
	*info = -7;
    } else if (*ldb < max(1,*n)) {
	*info = -9;
    } else if (*ldx < max(1,*n)) {
	*info = -11;
    }
    if (*info != 0) {
	i__1 = -(*info);
	xerbla_("SPORFS", &i__1);
	return 0;
    }

/*     Quick return if possible */

    if (*n == 0 || *nrhs == 0) {
	i__1 = *nrhs;
	for (j = 1; j <= i__1; ++j) {
	    ferr[j] = 0.f;
	    berr[j] = 0.f;
/* L10: */
	}
	return 0;
    }

/*     NZ = maximum number of nonzero elements in each row of A, plus 1 */

    nz = *n + 1;
    eps = slamch_("Epsilon");
    safmin = slamch_("Safe minimum");
    safe1 = nz * safmin;
    safe2 = safe1 / eps;

/*     Do for each right hand side */

    i__1 = *nrhs;
    for (j = 1; j <= i__1; ++j) {

	count = 1;
	lstres = 3.f;
L20:

/*        Loop until stopping criterion is satisfied.   

          Compute residual R = B - A * X */

	scopy_(n, &b_ref(1, j), &c__1, &work[*n + 1], &c__1);
	ssymv_(uplo, n, &c_b12, &a[a_offset], lda, &x_ref(1, j), &c__1, &
		c_b14, &work[*n + 1], &c__1);

/*        Compute componentwise relative backward error from formula   

          max(i) ( abs(R(i)) / ( abs(A)*abs(X) + abs(B) )(i) )   

          where abs(Z) is the componentwise absolute value of the matrix   
          or vector Z.  If the i-th component of the denominator is less   
          than SAFE2, then SAFE1 is added to the i-th components of the   
          numerator and denominator before dividing. */

	i__2 = *n;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    work[i__] = (r__1 = b_ref(i__, j), dabs(r__1));
/* L30: */
	}

/*        Compute abs(A)*abs(X) + abs(B). */

	if (upper) {
	    i__2 = *n;
	    for (k = 1; k <= i__2; ++k) {
		s = 0.f;
		xk = (r__1 = x_ref(k, j), dabs(r__1));
		i__3 = k - 1;
		for (i__ = 1; i__ <= i__3; ++i__) {
		    work[i__] += (r__1 = a_ref(i__, k), dabs(r__1)) * xk;
		    s += (r__1 = a_ref(i__, k), dabs(r__1)) * (r__2 = x_ref(
			    i__, j), dabs(r__2));
/* L40: */
		}
		work[k] = work[k] + (r__1 = a_ref(k, k), dabs(r__1)) * xk + s;
/* L50: */
	    }
	} else {
	    i__2 = *n;
	    for (k = 1; k <= i__2; ++k) {
		s = 0.f;
		xk = (r__1 = x_ref(k, j), dabs(r__1));
		work[k] += (r__1 = a_ref(k, k), dabs(r__1)) * xk;
		i__3 = *n;
		for (i__ = k + 1; i__ <= i__3; ++i__) {
		    work[i__] += (r__1 = a_ref(i__, k), dabs(r__1)) * xk;
		    s += (r__1 = a_ref(i__, k), dabs(r__1)) * (r__2 = x_ref(
			    i__, j), dabs(r__2));
/* L60: */
		}
		work[k] += s;
/* L70: */
	    }
	}
	s = 0.f;
	i__2 = *n;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    if (work[i__] > safe2) {
/* Computing MAX */
		r__2 = s, r__3 = (r__1 = work[*n + i__], dabs(r__1)) / work[
			i__];
		s = dmax(r__2,r__3);
	    } else {
/* Computing MAX */
		r__2 = s, r__3 = ((r__1 = work[*n + i__], dabs(r__1)) + safe1)
			 / (work[i__] + safe1);
		s = dmax(r__2,r__3);
	    }
/* L80: */
	}
	berr[j] = s;

/*        Test stopping criterion. Continue iterating if   
             1) The residual BERR(J) is larger than machine epsilon, and   
             2) BERR(J) decreased by at least a factor of 2 during the   
                last iteration, and   
             3) At most ITMAX iterations tried. */

	if (berr[j] > eps && berr[j] * 2.f <= lstres && count <= 5) {

/*           Update solution and try again. */

	    spotrs_(uplo, n, &c__1, &af[af_offset], ldaf, &work[*n + 1], n, 
		    info);
	    saxpy_(n, &c_b14, &work[*n + 1], &c__1, &x_ref(1, j), &c__1);
	    lstres = berr[j];
	    ++count;
	    goto L20;
	}

/*        Bound error from formula   

          norm(X - XTRUE) / norm(X) .le. FERR =   
          norm( abs(inv(A))*   
             ( abs(R) + NZ*EPS*( abs(A)*abs(X)+abs(B) ))) / norm(X)   

          where   
            norm(Z) is the magnitude of the largest component of Z   
            inv(A) is the inverse of A   
            abs(Z) is the componentwise absolute value of the matrix or   
               vector Z   
            NZ is the maximum number of nonzeros in any row of A, plus 1   
            EPS is machine epsilon   

          The i-th component of abs(R)+NZ*EPS*(abs(A)*abs(X)+abs(B))   
          is incremented by SAFE1 if the i-th component of   
          abs(A)*abs(X) + abs(B) is less than SAFE2.   

          Use SLACON to estimate the infinity-norm of the matrix   
             inv(A) * diag(W),   
          where W = abs(R) + NZ*EPS*( abs(A)*abs(X)+abs(B) ))) */

	i__2 = *n;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    if (work[i__] > safe2) {
		work[i__] = (r__1 = work[*n + i__], dabs(r__1)) + nz * eps * 
			work[i__];
	    } else {
		work[i__] = (r__1 = work[*n + i__], dabs(r__1)) + nz * eps * 
			work[i__] + safe1;
	    }
/* L90: */
	}

	kase = 0;
L100:
	slacon_(n, &work[(*n << 1) + 1], &work[*n + 1], &iwork[1], &ferr[j], &
		kase);
	if (kase != 0) {
	    if (kase == 1) {

/*              Multiply by diag(W)*inv(A'). */

		spotrs_(uplo, n, &c__1, &af[af_offset], ldaf, &work[*n + 1], 
			n, info);
		i__2 = *n;
		for (i__ = 1; i__ <= i__2; ++i__) {
		    work[*n + i__] = work[i__] * work[*n + i__];
/* L110: */
		}
	    } else if (kase == 2) {

/*              Multiply by inv(A)*diag(W). */

		i__2 = *n;
		for (i__ = 1; i__ <= i__2; ++i__) {
		    work[*n + i__] = work[i__] * work[*n + i__];
/* L120: */
		}
		spotrs_(uplo, n, &c__1, &af[af_offset], ldaf, &work[*n + 1], 
			n, info);
	    }
	    goto L100;
	}

/*        Normalize error. */

	lstres = 0.f;
	i__2 = *n;
	for (i__ = 1; i__ <= i__2; ++i__) {
/* Computing MAX */
	    r__2 = lstres, r__3 = (r__1 = x_ref(i__, j), dabs(r__1));
	    lstres = dmax(r__2,r__3);
/* L130: */
	}
	if (lstres != 0.f) {
	    ferr[j] /= lstres;
	}

/* L140: */
    }

    return 0;

/*     End of SPORFS */

} /* sporfs_ */

#undef x_ref
#undef b_ref
#undef a_ref


