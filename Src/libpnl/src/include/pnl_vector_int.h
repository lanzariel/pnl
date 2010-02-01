#ifndef VECTOR_INT_H
#define VECTOR_INT_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <stdio.h>
#include "pnl_vector.h"

struct PnlVect;
/**
 * \defgroup PnlVectInt Int Vector structure for Premia
 */
/*@{*/
typedef struct PnlVectInt{
  int size;/*!< size of the vector */ 
  int *array;/*!< pointer to store the data */
  int mem_size; /*!< size of the memory block allocated for array */
  int owner; /*!< 1 if the structure owns its array pointer */
} PnlVectInt;

#ifdef HAVE_INLINE 
extern inline
int pnl_vect_int_get (const PnlVectInt *self, int i)
{
  CheckIndexVect(self,i);
  return self->array[i];
}

extern inline
int* pnl_vect_int_lget (PnlVectInt *self, int i)
{
  CheckIndexVect(self,i);
  return &(self->array[i]);
}

extern inline
void pnl_vect_int_set (PnlVectInt *self, int i, int x)
{
  CheckIndexVect(self,i);
  self->array[i]=x;
}
#endif

extern void pnl_vect_int_set(PnlVectInt *v, int i, int x);
extern int pnl_vect_int_get(const PnlVectInt *v, int i);
extern int* pnl_vect_int_lget(PnlVectInt *v, int i);
extern void pnl_vect_int_free(PnlVectInt **v);
extern PnlVectInt* pnl_vect_int_create(int size);
extern PnlVectInt pnl_vect_int_create_wrap_array(const int *x, int size);
extern PnlVectInt* pnl_vect_int_create_from_int(int size, int x);
extern PnlVectInt* pnl_vect_int_create_from_ptr(int size, const int* x);
extern PnlVectInt* pnl_vect_int_create_from_list(int size,...);
extern PnlVectInt* pnl_vect_int_create_from_file (const char * file);
extern int pnl_vect_int_resize(PnlVectInt *v, int size);
extern int pnl_vect_int_resize_from_int(PnlVectInt *v, int size, int x);
extern int pnl_vect_int_resize_from_ptr(PnlVectInt *v, int size, const int *t);
extern PnlVectInt* pnl_vect_int_copy(const PnlVectInt *v);
extern void pnl_vect_int_clone(PnlVectInt *clone, const PnlVectInt *v);
extern PnlVectInt pnl_vect_int_wrap_subvect(const PnlVectInt *V, int i,int s);
extern PnlVectInt pnl_vect_int_wrap_subvect_with_last(const PnlVectInt *V, int i,int j);
extern void pnl_vect_int_print(const PnlVectInt *V);
extern void pnl_vect_int_print_nsp(const PnlVectInt *V);
extern void pnl_vect_int_fprint(FILE *fic, const PnlVectInt *V);
extern void pnl_vect_int_fprint_nsp(FILE *fic, const PnlVectInt *V);
extern void pnl_vect_int_plus_vect(PnlVectInt *lhs, const PnlVectInt *rhs); 
extern void pnl_vect_int_minus_vect(PnlVectInt *lhs, const PnlVectInt *rhs); 
extern void pnl_vect_int_minus(PnlVectInt *lhs);
extern void pnl_vect_int_map_inplace(PnlVectInt *lhs, int(*f)(int)); /*lhs=f(lhs)*/
extern void pnl_vect_int_plus_int(PnlVectInt *lhs, int x); /*lhs+=x*/
extern void pnl_vect_int_minus_int(PnlVectInt *lhs, int x); /*lhs-=x*/
extern void pnl_vect_int_axpby(int a, const PnlVectInt *x, int b, PnlVectInt *y); /* y:=a x + b y */
extern void pnl_vect_int_mult_int(PnlVectInt *lhs, int x); /*lhs*=x*/
extern void pnl_vect_int_div_int(PnlVectInt *lhs, int x); /*lhs*=x*/
extern void pnl_vect_int_inv_term(PnlVectInt *lhs); /* lhs = 1 ./ lhs*/
extern void
pnl_vect_int_div_vect_term(PnlVectInt *lhs, const PnlVectInt *rhs);/* lhs = lhs ./ rhs*/
extern void
pnl_vect_int_mult_vect_term(PnlVectInt *lhs, const PnlVectInt *rhs); /* lhs= lhs.*rhs */ 
extern void pnl_vect_int_set_int(PnlVectInt *v, int x);/* v[j]= x */
extern void pnl_vect_int_set_zero(PnlVectInt *v); /* v[j]= 0 */
extern int pnl_vect_int_sum(const PnlVectInt *lhs);/* sum(x) */
extern void pnl_vect_int_cumsum(PnlVectInt *lhs);
extern void
pnl_vect_int_map(PnlVectInt *lhs, const PnlVectInt *rhs,
                int(*f)(int));/* lhs(i)=f(rhs(i)) */
extern int
pnl_vect_int_scalar_prod(const PnlVectInt *rhs1, const PnlVectInt *rhs2); /*rhs1.rhs2*/
extern int pnl_vect_int_prod(const PnlVectInt *V); /*res=prod(V(i))*/
extern void pnl_vect_int_cumprod(PnlVectInt *V); /*res=prod(V(i))*/
extern int pnl_vect_int_max(const PnlVectInt *V); /*res=max(V)*/
extern int pnl_vect_int_min(const PnlVectInt *V); /*res=min(V)*/
extern void pnl_vect_int_minmax (const PnlVectInt *, int *, int *);
extern void pnl_vect_int_min_index (const PnlVectInt *, int *, int *);
extern void pnl_vect_int_max_index (const PnlVectInt *, int *, int *);
extern void pnl_vect_int_minmax_index (const PnlVectInt *, int *, int *, int *, int *);
extern void pnl_vect_int_qsort (PnlVectInt *, char);
extern void pnl_vect_int_qsort_index (PnlVectInt *, PnlVectInt *, char);

extern double pnl_vect_int_norm_two(const PnlVectInt *V); /*res=\Vert V \Vert_{l^2} */
extern double pnl_vect_int_norm_one(const PnlVectInt *V); /*res=\Vert V \Vert_{l^1} */
extern double pnl_vect_int_norm_infty(const PnlVectInt *V); /*res=\Vert V \Vert_{l^\infty} */
extern double pnl_vect_int_norm_x(const PnlVectInt *V,double(*f)(int)); /*res=\Vert V \Vert_{l^X} */

extern int pnl_vect_int_level(PnlVectInt *v, int i);
extern int pnl_vect_int_level_norm_one(PnlVectInt *v);
extern int pnl_vect_int_level_norm_inf(PnlVectInt *v);
extern void pnl_vect_int_dyadic_cast(const PnlVectInt * v_int,PnlVect * v_out);
extern void pnl_vect_int_swap_elements(PnlVectInt * v, int i, int j); 
extern void pnl_vect_int_reverse(PnlVectInt * v);


/*extern void pnl_vect_int_compute_father(const PnlVectInt *v, PnlVectInt * father, int i); */
/*extern PnlVectInt * pnl_vect_int_create_son(const PnlVectInt *v, int i,boolean LorR); */
/*extern void pnl_vect_int_compute_son(const PnlVectInt *v,PnlVectInt *son, int i,boolean LorR); */
/*@}*/


#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* VECTOR_INT_H */
