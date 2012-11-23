
/************************************************************************/
/* Copyright J�r�me Lelong <jerome.lelong@gmail.com>                    */
/*                                                                      */
/* This program is free software: you can redistribute it and/or modify */
/* it under the terms of the GNU Lesser General Public License as       */
/* published by the Free Software Foundation, either version 3 of the   */
/* License, or (at your option) any later version.                      */
/*                                                                      */
/* This program is distributed in the hope that it will be useful, but  */
/* WITHOUT ANY WARRANTY; without even the implied warranty of           */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU    */
/* Lesser General Public License for more details.                      */
/*                                                                      */
/* You should have received a copy of the GNU Lesser General Public     */
/* License along with this program.  If not, see                        */
/* <http://www.gnu.org/licenses/>.                                      */
/************************************************************************/

#include <math.h>

#include "pnl/pnl_config.h"
#include "pnl/pnl_mathtools.h"
#include "pnl/pnl_specfun.h"


/** nearest integer round off function
 * @param s the value to be rounded
 * @return the rounded value casted to an integer. 1.5 is rounded to 1 and
 * -1.5 is rounded to -1
 */
int intapprox(double s)
{
  int r = (int) s;
  if (s > r + 0.5) { r++; }
  if (s < r - 0.5) { r--; }
  return r;
}


#ifndef PNL_HAVE_TRUNC
double trunc(double x)
{
  return (double) ((int) x);
}
#endif

/** Computes the binomial coefficients with an error smaller than 1 for large
 * values of the parameter n
 * @param n an integer
 * @param p an integer
 * @return C(n, p)
 */
double Cnp(int n, int p) 
{                          
  
  double z, iter;
  int i;
  z=0.0;

  if ((n-p<= -1) || (n<0) || (p<0))
  {
    return z;
  }
  else
  {
    if (p==0)
      z=1.0;
    else
    {
      z=1.0;
      iter=z;
      i=0;
      while(i<=p-1)
      {
        iter=iter*(double)(n-i)/(p-i);
        i=i+1;
      }
      if ((iter-floor(iter))>=0.5)
        z=floor(iter)+1;
      else
        z=floor(iter);
    }
  }

  return z;
}

/** factorial function
 * @param n an integer
 * @return the factorial of n as a double
 */
double pnl_fact(int n)  
{
  int i;
  double z=1.0;
  if (n<=0) { z=1.0; }
  else { for ( i=1 ; i<=n ; i++) z=z*i; }
  return z;
}


/**
  * Computes the natural logarithm of the Gamma function
  * @param x  a real number > 0
  * @return  log(gamma(x))
  */ 

double pnl_lgamma(double x)
{
  return pnl_sf_log_gamma (x);
}

/**
  * Computes the true Gamma function 
  * @param x  a real number > -1
  * @return  gamma(x)
  */
double pnl_tgamma(double x)
{
  return pnl_sf_gamma (x);
} 

/**
  * Interger power function
  * @param x a real number
  * @param n an integer
  * @ return x^n
  */
double pnl_pow_i (double x, int n)
{
  double y = 1.;

  if ( x == 0 && n > 0 ) return 0.;

  if ( n < 0 ) 
    {
      x = 1. / x;
      n = -n;
    }

  while (n)
    {
       if (PNL_IS_ODD (n)) y *= x; /* y = x * x^(n-1) */
       n >>= 1;
       x *= x;
    } 
  return y;
} 


/** 
 * Computes the inverse of the cosh function based on its logarithmic
 * representation. This function is alredy provided by the C99 libc as
 * acosh
 * 
 * @param x a real number in the range [1, +infinity]
 * 
 * @return acosh(x)
 */
double pnl_acosh (double x)
{
  if ( x == 1. ) return 0.;
  else if ( x > 1. / SQRT_DBL_EPSILON )
    {
      return log(x) + M_LN2;
    }
  else if ( x > 1 )
    {
      double y = sqrt(x - 1.); 
      return pnl_log1p (y * (y + sqrt (x + 1.)));
    }
  else
    {
      return PNL_NAN;
    }
}

/** 
 * Computes the inverse of the asinh function based on its logarithmic
 * representation. This function is alredy provided by the C99 libc as
 * asinh
 * 
 * @param x a real number in the range [-infinity, +infinity]
 * 
 * @return asinh(x)
 */
double pnl_asinh (double x)
{
  double y = fabs (x);
  double sgn = PNL_SIGN (x);
  if ( y > 1. / SQRT_DBL_EPSILON )
    {
      return sgn * (log(y) + M_LN2);
    }
  else if ( y > SQRT_DBL_EPSILON )
    {
      double y_1 = 1. / y;
      return sgn * pnl_log1p (y + y / (y_1 + sqrt (1. + y_1 * y_1)));
    }
  else
    {
      return x;
    }
}

/** 
 * Computes the inverse of the tanh function based on its logarithmic
 * representation. This function is alredy provided by the C99 libc as
 * tanh
 * 
 * @param x a real number in the range [-1, +1]
 * 
 * @return atanh(x)
 */
double pnl_atanh (double x)
{
  double y = fabs (x);
  double sgn = PNL_SIGN (x);
  if ( y > 1. ) return PNL_NAN;
  else if ( y == 1. )
    {
      return sgn * PNL_POSINF;
    }
  else if ( y >= DBL_EPSILON )
    {
      return sgn * 0.5 * pnl_log1p (2 * y / (1 - y));
    }
  else
    {
      return x;
    }
}

