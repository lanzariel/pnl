/* crotg.f -- translated by f2c (version 20061008).
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

 int crotg_(complex *ca, complex *cb, float *c__, complex *s)
{
    /* System generated locals */
    float r__1, r__2;
    complex q__1, q__2, q__3;

    /* Builtin functions */
    double c_ABS(complex *), sqrt(double);
    void r_cnjg(complex *, complex *);

    /* Local variables */
    float norm;
    complex alpha;
    float scale;

/*     .. Scalar Arguments .. */
/*     .. */

/*  Purpose */
/*  ======= */

/*  CROTG determines a complex Givens rotation. */

/*     .. Local Scalars .. */
/*     .. */
/*     .. Intrinsic Functions .. */
/*     .. */
    if (c_ABS(ca) != 0.f) {
	goto L10;
    }
    *c__ = 0.f;
    s->r = 1.f, s->i = 0.f;
    ca->r = cb->r, ca->i = cb->i;
    goto L20;
L10:
    scale = c_ABS(ca) + c_ABS(cb);
    q__1.r = ca->r / scale, q__1.i = ca->i / scale;
/* Computing 2nd power */
    r__1 = c_ABS(&q__1);
    q__2.r = cb->r / scale, q__2.i = cb->i / scale;
/* Computing 2nd power */
    r__2 = c_ABS(&q__2);
    norm = scale * sqrt(r__1 * r__1 + r__2 * r__2);
    r__1 = c_ABS(ca);
    q__1.r = ca->r / r__1, q__1.i = ca->i / r__1;
    alpha.r = q__1.r, alpha.i = q__1.i;
    *c__ = c_ABS(ca) / norm;
    r_cnjg(&q__3, cb);
    q__2.r = alpha.r * q__3.r - alpha.i * q__3.i, q__2.i = alpha.r * q__3.i + 
	    alpha.i * q__3.r;
    q__1.r = q__2.r / norm, q__1.i = q__2.i / norm;
    s->r = q__1.r, s->i = q__1.i;
    q__1.r = norm * alpha.r, q__1.i = norm * alpha.i;
    ca->r = q__1.r, ca->i = q__1.i;
L20:
    return 0;
} /* crotg_ */
