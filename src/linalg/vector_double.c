
/************************************************************************/
/* Copyright J�r�me Lelong <jerome.lelong@gmail.com>                    */
/* Copyright C�line Labart                                              */
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


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "config.h"
#include "pnl/pnl_matrix.h"
#include "pnl/pnl_mathtools.h"

static char pnl_vector_compact_label[] = "PnlVectCompact";

/**
 * Create a new PnlVectCompact with size 0
 * @return a pointeur to PnlVectCompact
 */
PnlVectCompact* pnl_vect_compact_new ()
{
  PnlVectCompact *o;
  if ( (o=malloc (sizeof (PnlVectCompact)))==NULL) return NULL;
  o->object.type = PNL_TYPE_VECTOR_COMPACT;
  o->object.parent_type = PNL_TYPE_OBJECT;
  o->object.label = pnl_vector_compact_label;
  o->size = 0;
  o->convert = 'd';
  o->val = 0;
  return o;
}

/**
 * Create a PnlVectCompact. By default type='d'.
 * @param n size
 * @param x value to fill the vector
 * @return a pointeur to PnlVectCompact
 */
PnlVectCompact* pnl_vect_compact_create (int n, double x)
{
  PnlVectCompact *v;
  if ((v=malloc (sizeof(PnlVectCompact)))==NULL) return NULL;
  v->size = n;
  v->convert = 'd';
  v->val = x;
  return v;
}

/**
* Create a PnlVectCompact from a vector of doubles. By default type='a'.
* @param n size
* @param x pointer to a vector of doubles to fill the PnlVectCompact
* @return a pointeur to PnlVectCompact
*/
PnlVectCompact* pnl_vect_compact_create_from_ptr(int n, double const *x) 
{
	PnlVectCompact *v; int i;
	if ((v=malloc (sizeof(PnlVectCompact)))==NULL) return NULL;
	v->size = n;
	v->convert = 'a';
	if ((v->array = (double*)malloc(sizeof (double) * n)) == NULL) { free(v); return NULL;};
	for (i = 0; i != n; ++i)
		v->array[i] = x[i];
	return v;
}

/**
 * Resize a PnlVectCompact.
 * @param v the PvlVectCompact to be resized
 * @param size new size
 * @param x new value to set
 * @return OK or WRONG
 */
int pnl_vect_compact_resize (PnlVectCompact *v, int size, double x)
{
  if (v->convert == 'a')
    {
      free (v->array); v->array = NULL;
    }
  v->size = size;
  v->convert = 'd';
  v->val = x;
  return OK;
}

/**
 * Copy a PnlVectCompact
 *
 * @param v a constant PnlVectCompact pointer
 * @return  a PnlVectCompact  pointer initialised with v
 */
PnlVectCompact* pnl_vect_compact_copy(const PnlVectCompact *v)
{
  PnlVectCompact *ret = NULL;

  if (v->convert == 'd')
    ret = pnl_vect_compact_create (v->size, v->val);
  else
    {
      if ((ret=malloc(sizeof(PnlVectCompact)))==NULL) return NULL;
      ret->size = v->size;
      ret->convert = 'a';
      if ((ret->array=malloc(v->size*sizeof(double)))==NULL) return NULL;
      memcpy(ret->array, v->array, sizeof(double)*ret->size);
    }
  return ret;
}


/**
 * Free a PnlVectCompact
 * @param v address of a PnlVectCompact
 */
void pnl_vect_compact_free (PnlVectCompact **v)
{
  if ((*v) == NULL) return;
  if ((*v)->convert == 'a' && (*v)->array!= NULL)
    {
      free ((*v)->array); (*v)->array = NULL;
    }
  free (*v); *v=NULL;
}

/**
 * Convert a PnlVectCompact pointeur to a PnlVect pointeur
 * @param C the PnlVectCompact to be expanded
 * @return a PnlVect pointeur
 */
PnlVect* pnl_vect_compact_to_pnl_vect (const PnlVectCompact *C)
{
  PnlVect *v;
  if (C->convert == 'd')
    {
      v = pnl_vect_create_from_double (C->size, C->val);
    }
  else
    {
      v = pnl_vect_create (C->size);
      memcpy (v->array, C->array, C->size*sizeof(double));
    }
  return v;
}

/**
 * Access an element
 * @param C a PnlVectCompact
 * @param i index
 * @return C[i]
 */
double pnl_vect_compact_get (const PnlVectCompact *C, int i)
{
  CheckIndexVect (C, i);
  if (C->convert == 'd') return C->val;
  return C->array[i];
}

/**
 * Fill a PnlVectCompact with a unique value.
 * The storage is converted to the compact way
 * 
 * @param C a PnlVectCompact
 * @param x a real value
 */
void pnl_vect_compact_set_double (PnlVectCompact *C, double x)
{
  if ( C->convert =='a' && C->array != NULL )
    {
      free (C->array); C->array = NULL;
    }
  C->convert = 'd'; C->val = x;
}
/**
 * Copy an array into a PnlVectCompact
 * 
 * @param C a PnlVectCompact
 * @param ptr an array of double. We assume it has the same size as C
 */
void pnl_vect_compact_set_ptr (PnlVectCompact *C, double *ptr)
{
  if ( C->convert == 'd' )
    {
      C->convert = 'a';
      C->array = malloc (sizeof(double) * C->size);
    }
  memcpy (C->array, ptr, C->size * sizeof(double));
}
