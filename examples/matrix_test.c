
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

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "pnl/pnl_matrix.h"
#include "pnl/pnl_random.h"
#include "tests_utils.h"

/* static double function_prod(double x, double y) {return x*y;} */


/** 
 * Sets the upper (uplo='U') or lower (uplo='L') triangle to 0
 * 
 * @param uplo 'U' or 'L'
 * @param A a real matrix
 */
static void set_triangular_to_zero (PnlMat *A, char uplo)
{
  int i, j;

  if ( uplo == 'L')
    {
      for ( i=0 ; i <A->m ; i++ )
        {
          for ( j=0 ; j<i ; j++ )
            {
              PNL_MLET (A, i, j) = 0;
            }
        }
    }
  else
    {
      for ( i=0 ; i <A->m ; i++ )
        {
          for ( j=i+1 ; j<A->n ; j++ )
            {
              PNL_MLET (A, i, j) = 0;
            }
        }
    }
}

/** 
 * Creates an invertible matrix of size nxn using the exponential function
 * 
 * @param A Output parameter, contains a n x n intervible matrix.
 * @param n size of the matrix
 * @param gen index of the generator
 */
static void create_invertible_matrix (PnlMat *A, int n, int gen)
{
  PnlMat *B;

  B = pnl_mat_create (n, n);
  pnl_mat_rand_uni2 (B, n, n, 0., 1., gen);

  pnl_mat_exp (A, B);
  pnl_mat_free (&B);
}

/** 
 * Creates a symmetric positive definite matrix of size nxn
 * 
 * @param S Output parameter, contains a n x n symmetric positive definite matrix.
 * @param n size of the matrix
 * @param gen index of the generator
 */
static void create_sym_pos_matrix (PnlMat *S, int n, int gen)
{
  PnlVect *b;
  double g;
  int i;

  b = pnl_vect_create (n);
  pnl_vect_rand_uni (b, n, 0., 1., gen);

  /* S is set to a diagonal matrix with positive eigenvalues */
  pnl_mat_set_double (S, 0.);
  for ( i=0 ; i<S->n ; i++ )
    {
      do
        {
          g = fabs (pnl_rand_normal (gen));
        }
      while (g < 1E-4);
      pnl_mat_set (S, i, i, g);
    }

  pnl_mat_dger (1., b, b, S);
  pnl_vect_free (&b);
}

static void pnl_mat_create_from_double_test()
{
  PnlMat *M;
  int rows, cols, i;
  double x;
  rows=4;
  cols=2;
  x=2.5;
  M=pnl_mat_create_from_double(rows,cols,x);
  for ( i=0 ; i<M->mn ; i++ )
    {
      if ( M->array[i] != 2.5 )
        {
          pnl_test_set_fail ( "mat_create_from_double", M->array[i], 2.5);
          goto J1;
        }
    }
  pnl_test_set_ok ("mat_create_from_double");
J1:
  pnl_mat_free(&M);
}

static void pnl_mat_create_from_ptr_test()
{
  PnlMat *M;
  int rows, cols, i;
  double x[8]={1.0, 5.0, 3.0, 8.0, 3.0, 7.0, 6.0, 9.0};
  rows=4;
  cols=2;
  M=pnl_mat_create_from_ptr(rows,cols,x);
  for ( i=0 ; i<M->mn ; i++ )
    {
      if ( M->array[i] != x[i] )
        {
          pnl_test_set_fail ( "mat_create_from_ptr", M->array[i], x[i]);
          goto J1;
        }
    }
  pnl_test_set_ok ("mat_create_from_ptr");
J1:
  pnl_mat_free(&M);
  M=pnl_mat_create_from_list(rows,cols,1.0, 5.0, 3.0, 8.0, 3.0, 7.0, 6.0, 9.0);
  for ( i=0 ; i<M->mn ; i++ )
    {
      if ( M->array[i] != x[i] )
        {
          pnl_test_set_fail ( "mat_create_from_ptr", M->array[i], x[i]);
          goto J2;
        }
    }
  pnl_test_set_ok ("mat_create_from_ptr");
J2:
  pnl_mat_free(&M);
}

static int ispos (double x[]) { return x[0] >= 0; }
static int islarger (double x[]) { return x[0] >= x[1]; }
static int cmp (double x[]) { return (x[0] >= x[1]) && (x[1] < x[2]); }

static void pnl_mat_submat_test ()
{
  int gen=PNL_RNG_MERSENNE_RANDOM_SEED;
  int i, m = 5, n = 7;
  PnlMat *M1, *M2;
  PnlVect *v1;
  PnlVectInt *indi, *indj;
  pnl_rand_init (gen, m, n);
  M1 = pnl_mat_create (m ,n);
  M2 = pnl_mat_create (m, n);
  pnl_mat_rand_normal(M1, m, n, gen);
  pnl_mat_rand_normal(M2, m, n, gen);
  indi = pnl_vect_int_create (0);
  indj = pnl_vect_int_create (0);

  /*
   * Finds all the components s.t. M1(i, j) >= 0
   * extracts the indices for which the test is true
   */
  if ( pnl_mat_find (indi, indj, "m", ispos, M1) == FAIL ) printf ("Error in pnl_mat_find\n");
  v1 = pnl_vect_create_submat (M1, indi, indj);
  for ( i=0 ; i<v1->size ; i++ )
    {
      if ( ! ispos (&(v1->array[i])) ) 
        {
          pnl_test_set_fail ("mat_find >=0", v1->array[i], 0.);
          break;
        }
    }
  if ( i == v1->size ) pnl_test_set_ok ("mat_find >=0");
  pnl_vect_free (&v1);

  /*
   * Finds all the components s.t. M1(i, j) >= 0
   * extracts the linear index for which the test is true
   */
  if ( pnl_mat_find (indi, NULL, "m", ispos, M1) == FAIL ) printf ("Error in pnl_mat_find\n");
  for ( i=0 ; i<indi->size ; i++ )
    {
      if ( ! ispos ( &(PNL_GET(M1,indi->array[i])) ) ) 
        {
          pnl_test_set_fail ("mat_find >=0 indj", v1->array[i], 0.);
          break;
        }
    }
  if ( i == indi->size ) pnl_test_set_ok ("mat_find >=0 indj");

  /*
   * Finds the components s.t. M1(i,j) >= M2(i,j)
   */
  if ( pnl_mat_find (indi, indj, "mm", islarger, M1, M2) == FAIL ) printf ("Error in pnl_mat_find\n");
  for ( i=0 ; i<indi->size ; i++ )
    {
      double M1_ij = PNL_MGET(M1, PNL_GET(indi,i), PNL_GET(indj,i));
      double M2_ij = PNL_MGET(M2, PNL_GET(indi,i), PNL_GET(indj,i));
      if ( ! (M1_ij >= M2_ij) )
        {
          pnl_test_set_fail ("mat_find islarger", M1_ij - M2_ij, 0.);
          break;
        }
    }
  if ( i == indi->size ) pnl_test_set_ok ("mat_find islarger");

  /*
   * Finds the components s.t. M1 >= M2 && M2 < 0
   */
  pnl_mat_find (indi, indj, "mmr", cmp, M1, M2, 0.);
  for ( i=0 ; i<indi->size ; i++ )
    {
      double M1_ij = PNL_MGET(M1, PNL_GET(indi,i), PNL_GET(indj,i));
      double M2_ij = PNL_MGET(M2, PNL_GET(indi,i), PNL_GET(indj,i));
      if ( ! (M1_ij >= M2_ij && M2_ij < 0) )
        {
          pnl_test_set_fail ("mat_find cmp", M1_ij - M2_ij, 0.);
          break;
        }
    }
  if ( i == indi->size ) pnl_test_set_ok ("mat_find islarger");

  pnl_vect_int_free (&indi);
  pnl_vect_int_free (&indj);
  pnl_mat_free (&M1);
  pnl_mat_free (&M2);
}

static void pnl_mat_copy_test()
{
  PnlMat *M1;
  PnlMat *M2;
  M2=pnl_mat_create_from_double(4,2,3.0);
  M1=pnl_mat_copy(M2);
  pnl_test_mat_eq_abs (M1, M2, 1E-18, "mat_copy", "");
  pnl_mat_free(&M1);
  pnl_mat_free(&M2);
}

static void pnl_mat_clone_test()
{
  PnlMat *M1;
  PnlMat *M2;
  M2=pnl_mat_create_from_double(4,2,3.0);
  M1=pnl_mat_create(0,0);
  pnl_mat_clone(M1,M2);
  pnl_test_mat_eq_abs (M1, M2, 1E-18, "mat_clone", "");
  pnl_mat_free(&M1);
  pnl_mat_free(&M2); 
}

static void pnl_mat_map_inplace_test()
{
  int i;
  PnlMat *M;
  M=pnl_mat_create_from_double(4,2,3.0);
  pnl_mat_map_inplace(M,exp);
  for ( i=0 ; i<M->mn ; i++ )
    {
      if ( M->array[i] != exp(3.) )
        {
          pnl_test_set_fail ( "mat_map_inplace", M->array[i], exp(3.));
          goto J1;
        }
    }
  pnl_test_set_ok ("mat_map_inplace");
J1:
  pnl_mat_free(&M);
}

static void pnl_mat_plus_mat_test()
{
  PnlMat *M1, *M2, *M1copy;
  int i;
  double rows;
  double cols;
  double x[8]={1.0, 5.0, 3.0, 8.0, 3.0, 7.0, 6.0, 9.0};
  rows=4;
  cols=2;
  M1=pnl_mat_create_from_ptr(rows,cols,x);
  M1copy = pnl_mat_copy (M1);
  M2=pnl_mat_create_from_double(4,2,3.0);
  pnl_mat_plus_mat(M1,M2);
  for ( i=0 ; i<M1->mn ; i++ )
    {
      double expected = M1copy->array[i] + M2->array[i];
      if ( M1->array[i] != expected )
        {
          pnl_test_set_fail ( "mat_plus_mat", M1->array[i], expected);
          goto J1;
        }
    }
  pnl_test_set_ok ("mat_plus_mat");
J1:
  pnl_mat_free(&M1);
  pnl_mat_free(&M1copy);
  pnl_mat_free(&M2); 
}

static void pnl_mat_plus_double_test()
{
  int i;
  PnlMat *M, *Mcopy;
  M=pnl_mat_create_from_double(4,2,3.0);
  Mcopy = pnl_mat_copy (M);
  pnl_mat_plus_double(M,0.5);
  for ( i=0 ; i<M->mn ; i++ )
    {
      double expected = Mcopy->array[i] + 0.5;
      if ( M->array[i] != expected )
        {
          pnl_test_set_fail ( "mat_plus_double", M->array[i], expected);
          goto J1;
        }
    }
  pnl_test_set_ok ("mat_plus_double");
J1:
  pnl_mat_free(&M);
  pnl_mat_free(&Mcopy);
}

static void pnl_mat_mult_mat_test()
{
  PnlMat *A, *B, *AB, *res;
  A = pnl_mat_create_from_file ("Data/A.txt");
  B = pnl_mat_create_from_file ("Data/B.txt");
  AB = pnl_mat_create_from_file ("Data/AB.txt");
  res = pnl_mat_mult_mat (A, B);

  pnl_test_mat_eq_abs(AB, res, 1E-12, "mat_mult_mat", "");

  pnl_mat_free (&A);
  pnl_mat_free (&B);
  pnl_mat_free (&AB);
  pnl_mat_free (&res);
}

static void pnl_mat_mult_mat_inplace_test()
{
  PnlMat *A, *B, *AB, *res;
  A = pnl_mat_create_from_file ("Data/A.txt");
  B = pnl_mat_create_from_file ("Data/B.txt");
  AB = pnl_mat_create_from_file ("Data/AB.txt");
  res = pnl_mat_new ();
  pnl_mat_mult_mat_inplace (res, A, B);

  pnl_test_mat_eq_abs(AB, res, 1E-12, "mat_mult_mat_inplace", "");

  pnl_mat_free (&A);
  pnl_mat_free (&B);
  pnl_mat_free (&AB);
  pnl_mat_free (&res);
}

static void pnl_mat_mult_vect_test()
{
  PnlMat *A;
  PnlVect *Ax, *x, *y;

  A = pnl_mat_create_from_file ("Data/A.txt");
  Ax = pnl_vect_create_from_file ("Data/Ax.txt");
  x = pnl_vect_create_from_file ("Data/x.txt");

  y = pnl_vect_new ();
  pnl_mat_mult_vect_inplace (y, A, x);
  pnl_test_vect_eq_abs (y, Ax, 1E-12, "mat_mult_vect_inplace", "");
  pnl_vect_free (&y);

  y = pnl_mat_mult_vect (A, x);
  pnl_test_vect_eq_abs (y, Ax, 1E-12, "mat_mult_vect", "");

  pnl_vect_free (&x);
  pnl_vect_free (&Ax);
  pnl_vect_free (&y);
  pnl_mat_free (&A);
}

static void pnl_mat_mult_double_test()
{
  int i;
  PnlMat *M, *Mcopy;
  M=pnl_mat_create_from_double(4,2,3.0);
  Mcopy = pnl_mat_copy (M);
  pnl_mat_mult_double(M,0.5);
  for ( i=0 ; i<M->mn ; i++ )
    {
      double expected = Mcopy->array[i] * 0.5;
      if ( M->array[i] != expected )
        {
          pnl_test_set_fail ( "mat_mult_double", M->array[i], expected);
          goto J1;
        }
    }
  pnl_test_set_ok ("mat_mult_double");
J1:
  pnl_mat_free(&M);
  pnl_mat_free(&Mcopy);
}

static void pnl_mat_mult_mat_term_test()
{
  int i;
  PnlMat *M1, *M2, *M;
  int gen = PNL_RNG_MERSENNE_RANDOM_SEED;
  pnl_rand_init (gen, 5, 5);
  M1 = pnl_mat_create (5, 4);
  M2 = pnl_mat_create (5, 4);
  pnl_mat_rand_normal (M1, 5, 4, gen);
  pnl_mat_rand_normal (M2, 5, 4, gen);
  M = pnl_mat_copy (M1);
  pnl_mat_mult_mat_term(M,M2);
  for ( i=0 ; i<M->mn ; i++ )
    {
      double expected = M1->array[i] * M2->array[i];
      if ( M->array[i] != expected )
        {
          pnl_test_set_fail ( "mat_mutl_mat_term", M->array[i], expected);
          goto J1;
        }
    }
  pnl_test_set_ok ("mat_mutl_mat_term");
J1:
  pnl_mat_free(&M);
  pnl_mat_free(&M1);
  pnl_mat_free(&M2);
}

static void pnl_mat_chol_test()
{
  PnlMat *S, *Scopy, *Sres;
  PnlVect *x;
  int gen = PNL_RNG_MERSENNE_RANDOM_SEED;
  pnl_rand_init (gen, 5, 5);
  x = pnl_vect_create (5);
  S = pnl_mat_create (5, 5);
  pnl_mat_set_id (S);
  pnl_vect_rand_normal (x, 5, gen);
  pnl_mat_dger (1., x, x, S);
  Scopy = pnl_mat_copy (S);
  pnl_mat_chol(S);

  Sres = pnl_mat_new ();
  pnl_mat_dgemm ('N', 'T', 1, S, S, 0, Sres);
  pnl_test_mat_eq_abs (Sres, Scopy, 1E-12, "mat_chol", "");
  
  pnl_vect_free(&x);
  pnl_mat_free(&Sres);
  pnl_mat_free(&Scopy);
  pnl_mat_free(&S);
}

static void pnl_mat_sq_transpose_test()
{
  PnlMat *M, *Mcopy;
  int i, j;
  double x[9]={3.0, 1.0, 4.0, 5.0, 3.0, 6.0, 2.0, 9.0, 3.0};
  M=pnl_mat_create_from_ptr(3,3,x);
  Mcopy = pnl_mat_copy (M);
  pnl_mat_sq_transpose(M);
  for ( i=0 ; i<M->m ; i++ )
    {
      for ( j=0 ; j<M->n ; j++ )
        {
          double Mij = MGET(Mcopy, i, j);
          double tMji = MGET(M, j, i);
          if ( Mij != tMji ) 
            {
              pnl_test_set_fail ("mat_sq_transpose", 0, 0);
              goto J1;
            }
        }
    }
  pnl_test_set_ok ("mat_sq_transpose");
J1:
  pnl_mat_free(&M);
  pnl_mat_free(&Mcopy);
}

static void pnl_mat_transpose_test()
{
  PnlMat *M,*M1;
  int i, j;
  double x[8]={1.0, 5.0, 3.0, 8.0, 3.0, 7.0, 6.0, 9.0};
  M=pnl_mat_create_from_ptr(4,2,x);
  M1=pnl_mat_transpose(M);
  for ( i=0 ; i<M->m ; i++ )
    {
      for ( j=0 ; j<M->n ; j++ )
        {
          double Mij = MGET(M, i, j);
          double tMji = MGET(M1, j, i);
          if ( Mij != tMji ) 
            {
              pnl_test_set_fail ("mat_transpose", 0, 0);
              goto J1;
            }
        }
    }
  pnl_test_set_ok ("mat_transpose");
J1:
  pnl_mat_free(&M);
  pnl_mat_free(&M1);
}

static void pnl_vect_wrap_mat_row_test()
{
  int i;
  PnlMat *M;
  PnlVect V;
  double x[8]={1.0, 5.0, 3.0, 8.0, 3.0, 7.0, 6.0, 9.0};
  M=pnl_mat_create_from_ptr(4,2,x);
  V=pnl_vect_wrap_mat_row(M,2);
  for ( i=0 ; i<V.size ; i++ )
    {
      if ( GET(&V,i) != MGET(M, 2, i) )
        {
          pnl_test_set_fail ("vect_wrap_mat_row", GET(&V,i), MGET(M, 2, i));
          goto J1;
        }
    }
  pnl_test_set_ok ("vect_wrap_mat_row");
J1:
  pnl_mat_free(&M);
}

static void pnl_mat_get_row_test()
{
  PnlMat *M;
  PnlVect *V;
  int i;
  double x[8]={1.0, 5.0, 3.0, 8.0, 3.0, 7.0, 6.0, 9.0};
  M=pnl_mat_create_from_ptr(4,2,x);
  V=pnl_vect_create(0);
  pnl_mat_get_row(V,M,2);
  for ( i=0 ; i<V->size ; i++ )
    {
      if ( GET(V,i) != MGET(M, 2, i) )
        {
          pnl_test_set_fail ("mat_row_to_vect", GET(V,i), MGET(M, 2, i));
          goto J1;
        }
    }
  pnl_test_set_ok ("mat_row_to_vect");
J1:
  pnl_vect_free(&V);
  pnl_mat_free(&M);
}

static void pnl_mat_sum_test()
{
  PnlMat *M, *Mcopy, *M_cumsum_r, *M_cumsum_c;
  double sum;
  int i;

  int gen = PNL_RNG_MERSENNE_RANDOM_SEED;
  pnl_rand_init (gen, 5, 5);

  M = pnl_mat_create (5, 4);
  pnl_mat_rand_normal (M, 5, 4, gen);
  sum = 0.;
  for ( i=0 ; i<M->mn ; i++ )
    {
      sum += M->array[i];
    }
  pnl_test_eq_abs ( sum, pnl_mat_sum(M), 1E-12, "mat_sum", "");
  pnl_mat_free(&M);

  M_cumsum_c = pnl_mat_create_from_file("Data/cumsum_A_c.txt");
  M_cumsum_r = pnl_mat_create_from_file("Data/cumsum_A_r.txt");

  Mcopy = pnl_mat_create_from_file ("Data/A.txt");
  M = pnl_mat_new ();

  pnl_mat_clone (M, Mcopy);
  pnl_mat_cumsum (M, 'r');
  pnl_test_mat_eq_abs (M, M_cumsum_r, 1E-12, "cumsum 'r'", "");
  pnl_mat_clone (M, Mcopy);
  pnl_mat_cumsum (M, 'c');
  pnl_test_mat_eq_abs (M, M_cumsum_c, 1E-12, "cumsum 'c'", "");

  pnl_mat_free(&M);
  pnl_mat_free(&M_cumsum_c);
  pnl_mat_free(&M_cumsum_r);
  pnl_mat_free(&Mcopy);
}

static void pnl_mat_prod_test()
{
  PnlMat *M, *Mcopy, *M_cumprod_r, *M_cumprod_c;
  double prod;
  int i;
  int gen = PNL_RNG_MERSENNE_RANDOM_SEED;
  pnl_rand_init (gen, 5, 5);
  M = pnl_mat_create (5, 4);
  pnl_mat_rand_normal (M, 5, 4, gen);
  prod = 1.;
  for ( i=0 ; i<M->mn ; i++ )
    {
      prod *= M->array[i];
    }
  pnl_test_eq_abs ( prod, pnl_mat_prod(M), 1E-12, "mat_prod", "");
  pnl_mat_free(&M);

  M_cumprod_c = pnl_mat_create_from_file("Data/cumprod_A_c.txt");
  M_cumprod_r = pnl_mat_create_from_file("Data/cumprod_A_r.txt");

  Mcopy = pnl_mat_create_from_file ("Data/A.txt");
  M = pnl_mat_new ();

  pnl_mat_clone (M, Mcopy);
  pnl_mat_cumprod (M, 'r');
  pnl_test_mat_eq_abs (M, M_cumprod_r, 1E-12, "cumprod 'r'", "");
  pnl_mat_clone (M, Mcopy);
  pnl_mat_cumprod (M, 'c');
  pnl_test_mat_eq_abs (M, M_cumprod_c, 1E-12, "cumprod 'c'", "");

  pnl_mat_free(&M);
  pnl_mat_free(&M_cumprod_c);
  pnl_mat_free(&M_cumprod_r);
  pnl_mat_free(&Mcopy);
}

static void pnl_mat_sum_vect_test()
{
  PnlMat *M;
  PnlVect *V, *M_sum_r, *M_sum_c;
  M_sum_c = pnl_vect_create_from_file("Data/sum_A_c.txt");
  M_sum_r = pnl_vect_create_from_file("Data/sum_A_r.txt");

  M = pnl_mat_create_from_file ("Data/A.txt");
  V = pnl_vect_new ();

  pnl_mat_sum_vect (V, M, 'r');
  pnl_test_vect_eq_abs (V, M_sum_r, 1E-12, "mat_sum 'r'", "");

  pnl_mat_sum_vect (V, M, 'c');
  pnl_test_vect_eq_abs (V, M_sum_c, 1E-12, "mat_sum 'c'", "");

  pnl_mat_free(&M);
  pnl_vect_free(&M_sum_c);
  pnl_vect_free(&M_sum_r);
  pnl_vect_free(&V);
}

static void pnl_mat_prod_vect_test()
{
  PnlMat *M;
  PnlVect *V, *M_prod_r, *M_prod_c;
  M_prod_c = pnl_vect_create_from_file("Data/prod_A_c.txt");
  M_prod_r = pnl_vect_create_from_file("Data/prod_A_r.txt");

  M = pnl_mat_create_from_file ("Data/A.txt");
  V = pnl_vect_new ();

  pnl_mat_prod_vect (V, M, 'r');
  pnl_test_vect_eq_abs (V, M_prod_r, 1E-12, "mat_prod 'r'", "");

  pnl_mat_prod_vect (V, M, 'c');
  pnl_test_vect_eq_abs (V, M_prod_c, 1E-12, "mat_prod 'c'", "");

  pnl_mat_free(&M);
  pnl_vect_free(&M_prod_c);
  pnl_vect_free(&M_prod_r);
  pnl_vect_free(&V);
}

static void pnl_mat_minmax_test()
{
  PnlMat *M;
  PnlVect *V, *M_max_r, *M_max_c, *M_min_r, *M_min_c, *M_min_star, *M_max_star;
  M_max_c = pnl_vect_create_from_file("Data/max_A_c.txt");
  M_max_r = pnl_vect_create_from_file("Data/max_A_r.txt");
  M_max_star = pnl_vect_create_from_file("Data/max_A_star.txt");
  M_min_c = pnl_vect_create_from_file("Data/min_A_c.txt");
  M_min_r = pnl_vect_create_from_file("Data/min_A_r.txt");
  M_min_star = pnl_vect_create_from_file("Data/min_A_star.txt");

  M = pnl_mat_create_from_file ("Data/A.txt");
  V = pnl_vect_new ();

  pnl_mat_max (V, M, 'r');
  pnl_test_vect_eq_abs (V, M_max_r, 1E-12, "mat_max 'r'", "");
  pnl_mat_max (V, M, 'c');
  pnl_test_vect_eq_abs (V, M_max_c, 1E-12, "mat_max 'c'", "");
  pnl_mat_max (V, M, '*');
  pnl_test_vect_eq_abs (V, M_max_star, 1E-12, "mat_max '*'", "");

  pnl_mat_min (V, M, 'r');
  pnl_test_vect_eq_abs (V, M_min_r, 1E-12, "mat_min 'r'", "");
  pnl_mat_min (V, M, 'c');
  pnl_test_vect_eq_abs (V, M_min_c, 1E-12, "mat_min 'c'", "");
  pnl_mat_min (V, M, '*');
  pnl_test_vect_eq_abs (V, M_min_star, 1E-12, "mat_min '*'", "");

  pnl_mat_free(&M);
  pnl_vect_free(&M_max_c);
  pnl_vect_free(&M_min_c);
  pnl_vect_free(&M_max_r);
  pnl_vect_free(&M_min_r);
  pnl_vect_free(&M_max_star);
  pnl_vect_free(&M_min_star);
  pnl_vect_free(&V);

  /* printf ("--> pnl_mat_minmax \n"); */
  /* pnl_mat_minmax (A, min, max, 'r'); */
  /* printf("min(A, 'r') = "); pnl_vect_print_nsp (min); */
  /* printf("max(A, 'r') = "); pnl_vect_print_nsp (max); */

  /* printf ("--> pnl_mat_min_index \n"); */
  /* pnl_mat_min_index(A, min, imin, 'c'); */
  /* printf("min(A, 'c') = "); pnl_vect_print_nsp (min); */
  /* printf("\tindex = "); pnl_vect_int_print_nsp (imin); */
  /* pnl_mat_min_index(A, min, imin, 'r'); */
  /* printf("min(A, 'r') = "); pnl_vect_print_nsp (min); */
  /* printf("\tindex = "); pnl_vect_int_print_nsp (imin); */

  /* printf ("--> pnl_mat_max_index \n"); */
  /* pnl_mat_max_index(A, max, imax, 'c'); */
  /* printf("max(A, 'c') = "); pnl_vect_print_nsp (max); */
  /* printf("\tindex = "); pnl_vect_int_print_nsp (imax); */
  /* pnl_mat_max_index(A, max, imax, 'r'); */
  /* printf("max(A, 'r') = "); pnl_vect_print_nsp (max); */
  /* printf("\tindex = "); pnl_vect_int_print_nsp (imax); */

  /* printf ("--> pnl_mat_minmax_index \n"); */
  /* pnl_mat_minmax_index (A, min, max, imin, imax, 'r'); */
  /* printf("max(A, 'r') = "); pnl_vect_print_nsp (max); */
  /* printf("\tindex = "); pnl_vect_int_print_nsp (imax); */
  /* printf("min(A, 'r') = "); pnl_vect_print_nsp (min); */
  /* printf("\tindex = "); pnl_vect_int_print_nsp (imin); */

}

static void pnl_mat_qsort_test ()
{
  PnlMat *M, *Mclone, *res;
  PnlMatInt *t, *res_t;

  Mclone = pnl_mat_create_from_file("Data/A.txt");
  t = pnl_mat_int_new();
  M = pnl_mat_new ();

  /*
   * For each sort, we compare the result with those from Nsp stored onto
   * files. Note that because indexing starts at 0 in PNL, the index matrix
   * from Nsp should be unshifted
   */
  pnl_mat_clone (M, Mclone);
  pnl_mat_qsort_index (M, t, 'r', 'i');
  res = pnl_mat_create_from_file ("Data/sort_A_r_i.txt");
  res_t = pnl_mat_int_create_from_file ("Data/sort_A_r_i_index.txt");
  pnl_mat_int_plus_int( res_t, -1);
  pnl_test_mat_eq_abs ( M, res, 1E-12, "mat_qsort r i", ""); 
  pnl_test_mat_int_eq ( t, res_t,  "mat_qsort r i (index)", ""); 
  pnl_mat_free (&res);
  pnl_mat_int_free (&res_t);

  pnl_mat_clone (M, Mclone);
  pnl_mat_qsort_index (M, t, 'r', 'd');
  res = pnl_mat_create_from_file ("Data/sort_A_r_d.txt");
  res_t = pnl_mat_int_create_from_file ("Data/sort_A_r_d_index.txt");
  pnl_mat_int_plus_int( res_t, -1);
  pnl_test_mat_eq_abs ( M, res, 1E-12, "mat_qsort r d", ""); 
  pnl_test_mat_int_eq ( t, res_t,  "mat_qsort r d (index)", ""); 
  pnl_mat_free (&res);
  pnl_mat_int_free (&res_t);

  pnl_mat_clone (M, Mclone);
  pnl_mat_qsort_index (M, t, 'c', 'i');
  res = pnl_mat_create_from_file ("Data/sort_A_c_i.txt");
  res_t = pnl_mat_int_create_from_file ("Data/sort_A_c_i_index.txt");
  pnl_mat_int_plus_int( res_t, -1);
  pnl_test_mat_eq_abs ( M, res, 1E-12, "mat_qsort c i", ""); 
  pnl_test_mat_int_eq ( t, res_t,  "mat_qsort c i (index)", ""); 
  pnl_mat_free (&res);
  pnl_mat_int_free (&res_t);

  pnl_mat_clone (M, Mclone);
  pnl_mat_qsort_index (M, t, 'c', 'd');
  res = pnl_mat_create_from_file ("Data/sort_A_c_d.txt");
  res_t = pnl_mat_int_create_from_file ("Data/sort_A_c_d_index.txt");
  pnl_mat_int_plus_int( res_t, -1);
  pnl_test_mat_eq_abs ( M, res, 1E-12, "mat_qsort c d", ""); 
  pnl_test_mat_int_eq ( t, res_t,  "mat_qsort c d (index)", ""); 
  pnl_mat_free (&res);
  pnl_mat_int_free (&res_t);

  pnl_mat_free (&M);
  pnl_mat_free (&Mclone);
  pnl_mat_int_free (&t);
}

static void pnl_mat_map_test()
{
  int i;
  PnlMat *M1,*M2;
  double x[8]={1.0, 5.0, 3.0, 8.0, 3.0, 7.0, 6.0, 9.0};
  M1=pnl_mat_create(0,0);
  M2=pnl_mat_create_from_ptr(4,2,x);
  pnl_mat_map(M1,M2,exp);
  for ( i=0; i<M1->mn; i++ )
    {
      if ( M1->array[i] != exp(M2->array[i]) )
        {
          pnl_test_set_fail ("mat_map", M1->array[i],  exp(M2->array[i]) );
          goto J1;
        }
    }
  pnl_test_set_ok ("mat_map");
J1:
  pnl_mat_free(&M1);
  pnl_mat_free(&M2);
}

static void pnl_mat_triangular_inverse_test ()
{
  PnlMat *A, *B, *AB, *Id;
  PnlVect *d;
  int gen;
  int n = 5;

  gen = PNL_RNG_MERSENNE_RANDOM_SEED;
  pnl_rand_init (gen, n, n);
  d = pnl_vect_create_from_double (n, 1.);
  Id = pnl_mat_create_diag (d);
  A = pnl_mat_create (n, n);
  B = pnl_mat_create (n, n);
  AB = pnl_mat_create (n, n);
  create_invertible_matrix (B, n, gen);

  set_triangular_to_zero (B, 'L');
  pnl_mat_upper_inverse(A,B);
  pnl_mat_mult_mat_inplace (AB, A, B);
  pnl_test_mat_eq_abs (AB, Id, 1E-8, "mat_upper_inverse", "");

  create_invertible_matrix (B, n, gen);
  set_triangular_to_zero (B, 'U');
  pnl_mat_lower_inverse(A,B);
  pnl_mat_mult_mat_inplace (AB, A, B);
  pnl_test_mat_eq_abs (AB, Id, 1E-8, "mat_upper_inverse", "");


  pnl_mat_free(&A);
  pnl_mat_free(&B);
  pnl_mat_free(&AB);
  pnl_mat_free(&Id);
  pnl_vect_free (&d);
}

static void pnl_mat_inverse_test ()
{
  PnlMat *A, *invA, *invAA, *Id;
  PnlVect *d;
  int gen;
  int n = 5;

  gen = PNL_RNG_MERSENNE_RANDOM_SEED;
  pnl_rand_init (gen, n, n);
  d = pnl_vect_create_from_double (n, 1.);
  Id = pnl_mat_create_diag (d);
  A = pnl_mat_create (n, n);
  invA = pnl_mat_create (n, n);
  invAA = pnl_mat_create (n, n);

  create_sym_pos_matrix (A, n, gen);
  pnl_mat_inverse_with_chol (invA, A);
  pnl_mat_mult_mat_inplace (invAA, A, invA);
  pnl_test_mat_eq_abs (invAA, Id, 1E-8, "mat_inverse_with_chol", "");

  create_invertible_matrix (A, n, gen);
  pnl_mat_inverse (invA, A);
  pnl_mat_mult_mat_inplace (invAA, A, invA);
  pnl_test_mat_eq_abs (invAA, Id, 1E-8, "mat_inverse", "");

  pnl_mat_free (&A);
  pnl_mat_free (&invA);
  pnl_mat_free (&invAA);
  pnl_mat_free (&Id);
  pnl_vect_free (&d);
}

static void pnl_mat_syslin_test ()
{
  PnlMat *S, *Scopy, *B, *Bcopy, *SB;
  PnlVect *b, *x, *Sx;
  PnlVectInt *p;
  int gen = PNL_RNG_MERSENNE_RANDOM_SEED;
  pnl_rand_init (gen, 5, 5);
  b = pnl_vect_create (5);
  Sx = pnl_vect_new ();
  S = pnl_mat_create (5, 5);
  SB = pnl_mat_new ();
  pnl_vect_rand_normal (b, 5, gen);
  create_sym_pos_matrix (S, 5, gen);

  Scopy = pnl_mat_copy (S);
  x = pnl_vect_new ();
  /* pnl_mat_upper_syslin(x,S,b); */
  /* pnl_mat_mult_vect_inplace (Sx, Scopy, x); */
  /* pnl_test_vect_eq_abs (Sx, b, 1E-8, "mat_upper_syslin", ""); */

  /* printf("test de la fonction 'pnl_mat_lower_syslin' : \n"); */
  /* pnl_mat_lower_syslin(x,S,b); */
  /* pnl_vect_print(x); */

  pnl_mat_chol (S);
  pnl_mat_chol_syslin(x, S, b);
  pnl_mat_mult_vect_inplace (Sx, Scopy, x);
  pnl_test_vect_eq_abs (Sx, b, 1E-8, "mat_chol_syslin", "");

  pnl_mat_clone (S, Scopy);
  p = pnl_vect_int_create (5);
  pnl_mat_lu (S, p);
  pnl_mat_lu_syslin (x, S, p, b);
  pnl_mat_mult_vect_inplace (Sx, Scopy, x);
  pnl_test_vect_eq_abs (Sx, b, 1E-8, "mat_lu_syslin (symmetric)", "");

  pnl_mat_clone (S, Scopy);
  pnl_mat_syslin (x, S, b);
  pnl_mat_mult_vect_inplace (Sx, Scopy, x);
  pnl_test_vect_eq_abs (Sx, b, 1E-8, "mat_syslin (symmetric)", "");

  B = pnl_mat_create (5,5);
  pnl_mat_rand_normal (B, 5, 5, gen);
  Bcopy = pnl_mat_copy (B);
  pnl_mat_clone (S, Scopy);
  pnl_mat_chol (S);
  pnl_mat_chol_syslin_mat (S, B);
  pnl_mat_mult_mat_inplace (SB, Scopy, B);
  pnl_test_mat_eq_abs (SB, Bcopy, 1E-8, "mat_chol_syslin_mat (symmetric)", "");

  pnl_mat_clone (S, Scopy);
  pnl_mat_clone (B, Bcopy);
  pnl_mat_lu (S, p);
  pnl_mat_lu_syslin_mat (S, p, B);
  pnl_mat_mult_mat_inplace (SB, Scopy, B);
  pnl_test_mat_eq_abs (SB, Bcopy, 1E-8, "mat_lu_syslin_mat (symmetric)", "");

  pnl_mat_clone (S, Scopy);
  pnl_mat_clone (B, Bcopy);
  pnl_mat_syslin_mat (S, B);
  pnl_mat_mult_mat_inplace (SB, Scopy, B);
  pnl_test_mat_eq_abs (SB, Bcopy, 1E-8, "mat_syslin_mat (symmetric)", "");


  create_invertible_matrix (S, 5, gen);
  pnl_mat_clone (Scopy, S);
  pnl_mat_lu (S, p);
  pnl_mat_lu_syslin (x, S, p, b);
  pnl_mat_mult_vect_inplace (Sx, Scopy, x);
  pnl_test_vect_eq_abs (Sx, b, 1E-8, "mat_lu_syslin", "");

  pnl_mat_clone (S, Scopy);
  pnl_mat_syslin (x, S, b);
  pnl_mat_mult_vect_inplace (Sx, Scopy, x);
  pnl_test_vect_eq_abs (Sx, b, 1E-8, "mat_syslin", "");

  pnl_mat_clone (S, Scopy);
  pnl_mat_clone (B, Bcopy);
  pnl_mat_lu (S, p);
  pnl_mat_lu_syslin_mat (S, p, B);
  pnl_mat_mult_mat_inplace (SB, Scopy, B);
  pnl_test_mat_eq_abs (SB, Bcopy, 1E-8, "mat_lu_syslin_mat", "");

  pnl_mat_clone (S, Scopy);
  pnl_mat_clone (B, Bcopy);
  pnl_mat_syslin_mat (S, B);
  pnl_test_mat_eq_abs (SB, Bcopy, 1E-8, "mat_syslin_mat", "");

  pnl_mat_free (&B);
  pnl_mat_free (&SB);
  pnl_mat_free (&Bcopy);
  pnl_mat_free (&S);
  pnl_mat_free (&Scopy);
  pnl_vect_free (&b);
  pnl_vect_free (&Sx);
  pnl_vect_free (&x);
  pnl_vect_int_free (&p);
}

static void pnl_mat_create_from_file_test ()
{
  PnlMat *M, *res;
  M = pnl_mat_create_from_file ("Data/Test_read_mat.dat");
  res = pnl_mat_create_from_list (4, 6, 1.,2.,3.,4.,5.,6., 7.,8.,9.,10.,11.,12., 13.,14.,15.,16.,17.,18., 19.,20.,21.,22.,23.,24.);
  pnl_test_mat_eq_abs (M, res, 1E-18, "mat_create_from_file", "");
  pnl_mat_free (&M);
  pnl_mat_free (&res);

}

static void pnl_mat_dgemm_test()
{
  PnlMat *A, *B, *tA, *tB, *C, *Cclone, *expec;
  double alpha, beta;

  alpha = 1.3;
  beta = 0.5;
  expec = pnl_mat_create_from_file ("Data/alpha_AB_beta_C.txt");
  A = pnl_mat_create_from_file ("Data/A.txt");
  tA = pnl_mat_create_from_file ("Data/tA.txt");
  B = pnl_mat_create_from_file ("Data/B.txt");
  tB = pnl_mat_create_from_file ("Data/tB.txt");
  Cclone = pnl_mat_create_from_file ("Data/C.txt");
  C = pnl_mat_new ();


  pnl_mat_clone (C, Cclone);
  pnl_mat_dgemm ('N', 'N', alpha, A, B, beta, C);
  pnl_test_mat_eq_abs (C, expec, 1E-12, "dgemm N N", "");


  pnl_mat_clone (C, Cclone);
  pnl_mat_dgemm ('T', 'N', alpha, tA, B, beta, C);
  pnl_test_mat_eq_abs (C, expec, 1E-12, "dgemm T N", "");

  pnl_mat_clone (C, Cclone);
  pnl_mat_dgemm ('N', 'T', alpha, A, tB, beta, C);
  pnl_test_mat_eq_abs (C, expec, 1E-12, "dgemm N T", "");

  pnl_mat_clone (C, Cclone);
  pnl_mat_dgemm ('T', 'T', alpha, tA, tB, beta, C);
  pnl_test_mat_eq_abs (C, expec, 1E-12, "dgemm T T", "");


  pnl_mat_free(&A);
  pnl_mat_free(&B);
  pnl_mat_free(&tA);
  pnl_mat_free(&tB);
  pnl_mat_free(&C);
  pnl_mat_free(&Cclone);
}

static void pnl_mat_dgemv_test ()
{
  double   alpha, beta;
  PnlMat  *A, *tA;
  PnlVect *x, *y, *yclone, *expec;
  alpha = 1.3;
  beta = 0.5;


  expec = pnl_vect_create_from_file ("Data/alpha_Ax_beta_y.txt");
  A = pnl_mat_create_from_file ("Data/A.txt");
  tA = pnl_mat_create_from_file ("Data/tA.txt");
  x = pnl_vect_create_from_file ("Data/x.txt");
  yclone = pnl_vect_create_from_file ("Data/y.txt");
  y = pnl_vect_new ();

  pnl_vect_clone (y, yclone);
  pnl_mat_dgemv ('N', alpha, A, x, beta, y);
  pnl_test_vect_eq_abs (y, expec, 1E-12, "dgemv N", "" );

  pnl_vect_clone (y, yclone);
  pnl_mat_dgemv ('T', alpha, tA, x, beta, y); 
  pnl_test_vect_eq_abs (y, expec, 1E-12, "dgemv T", "" );

  pnl_mat_free (&A);
  pnl_mat_free (&tA);
  pnl_vect_free (&x);
  pnl_vect_free (&y);
  pnl_vect_free (&yclone);
}

static void pnl_mat_mult_vect_transpose_test ()
{
  PnlMat *tA;
  PnlVect *x, *y1, *Ax;

  tA = pnl_mat_create_from_file ("Data/tA.txt");
  Ax = pnl_vect_create_from_file ("Data/Ax.txt");
  x = pnl_vect_create_from_file ("Data/x.txt");
  y1 = pnl_vect_new ();
  pnl_mat_mult_vect_transpose_inplace (y1, tA, x);
  pnl_test_vect_eq_abs (y1, Ax, 1E-12, "mat_mult_vect_transpose_inplace", "");


  pnl_vect_free (&x);
  pnl_vect_free (&y1);
  pnl_vect_free (&Ax);
  pnl_mat_free (&tA);
}

static void pnl_mat_scalar_prod_test ()
{
  PnlMat *A;
  PnlVect *x, *y, *Ay;
  int gen = PNL_RNG_MERSENNE_RANDOM_SEED;
  pnl_rand_init (gen, 5, 4);
  A = pnl_mat_create (5, 4);
  pnl_mat_rand_normal (A, 5, 4, gen);
  y = pnl_vect_create (4);
  pnl_vect_rand_normal(y, 4, gen);
  Ay = pnl_mat_mult_vect (A, y);

  x = pnl_vect_create (5);
  pnl_vect_rand_normal(x, 5, gen);
  pnl_test_eq (pnl_mat_scalar_prod (A, x, y), pnl_vect_scalar_prod (x, Ay),
               1E-12, "mat_scalar_prod", "");

  pnl_vect_free (&x);
  pnl_vect_free (&y);
  pnl_vect_free (&Ay);
  pnl_mat_free (&A);
}

int main (int argc, char **argv)
{
  pnl_test_init (argc, argv);
  pnl_mat_create_from_double_test();
  pnl_mat_create_from_ptr_test();
  pnl_mat_submat_test();
  pnl_mat_copy_test();
  pnl_mat_clone_test();
  pnl_mat_map_inplace_test();
  pnl_mat_plus_mat_test();
  pnl_mat_plus_double_test();
  pnl_mat_mult_mat_test();
  pnl_mat_mult_mat_inplace_test();
  pnl_mat_mult_vect_test();
  pnl_mat_mult_double_test();
  pnl_mat_mult_mat_term_test();
  pnl_mat_chol_test();
  pnl_mat_sq_transpose_test();
  pnl_mat_transpose_test();
  pnl_vect_wrap_mat_row_test();
  pnl_mat_get_row_test();
  pnl_mat_sum_test();
  pnl_mat_sum_vect_test();
  pnl_mat_prod_test();
  pnl_mat_prod_vect_test();
  pnl_mat_minmax_test();
  pnl_mat_qsort_test();
  pnl_mat_map_test();
  pnl_mat_triangular_inverse_test();
  pnl_mat_inverse_test();
  pnl_mat_syslin_test();
  pnl_mat_create_from_file_test();
  pnl_mat_dgemm_test();
  pnl_mat_mult_vect_transpose_test();
  pnl_mat_scalar_prod_test();
  pnl_mat_dgemv_test();
  exit (pnl_test_finalize ("Matrix"));
}
