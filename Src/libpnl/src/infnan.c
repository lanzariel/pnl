
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

#include "pnl_mathtools.h"

static double __pnl_div (double x, double y)
{
  return (x / y);
}

double pnl_nan (void)
{
  return __pnl_div (0.0, 0.0);
}

double pnl_posinf (void)
{
  return __pnl_div (+1.0, 0.0);
}

double pnl_neginf (void)
{
  return __pnl_div (-1.0, 0.0);
}

#ifdef _MSC_VER 
int pnl_isnan (double x)
{
  return _isnan(x);
}

int pnl_isinf (double x)
{
  int fpc = _fpclass(x);

  if (fpc == _FPCLASS_PINF) return +1;
  else if (fpc == _FPCLASS_NINF) return -1;
  else return 0;
}

int pnl_isfinite (double x)
{
  return _finite(x);
}
#else /* _MSC_VER */

int pnl_isfinite (double x)
{
#ifdef HAVE_ISFINITE
  return isfinite(x);
#elif defined (HAVE_FINITE)
  return finite(x);
#elif defined (HAVE_ISINF)
  /* isinf(3): In glibc 2.01 and earlier, isinf() returns a
     non-zero value (actually: 1) if x is an infinity (positive or
     negative).  (This is all that C99 requires.) */
  if (isinf(x)) return 0; 
  else return 1;
#else
  double z;
  z = x - x;
  return (z == z);
#endif
}

int pnl_isnan (double x)
{
#ifdef HAVE_ISNAN
  return isnan(x);
#else
  int status = (x != x);
  return status;
# endif
}

int pnl_isinf (double x)
{
#ifdef HAVE_ISINF
  /* isinf(3): In glibc 2.01 and earlier, isinf() returns a
     non-zero value (actually: 1) if x is an infinity (positive or
     negative).  (This is all that C99 requires.) */
  if (isinf(x)) return (x > 0) ? 1 : -1;
  else return 0;
#else
  if (!pnl_isfinite (x) && !pnl_isnan (x))
    return (x>0 ? 1 : -1);
  else
    return 0;
#endif
}

#endif /* _MSC_VER */

