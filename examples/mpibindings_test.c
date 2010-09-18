
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


#include <stdlib.h>
#include <mpi.h>

#include "pnl/pnl_random.h"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include "pnl/pnl_band_matrix.h"
#include "pnl/pnl_tridiag_matrix.h"
#include "pnl/pnl_basis.h"
#include "pnl/pnl_mpi.h"

#define SENDTAG 1
#define PNL_MPI_MESSAGE(info, msg)                             \
  if ( info != MPI_SUCCESS )                                   \
    {                                                          \
      if (pnl_message_is_on ()) printf (msg);                  \
      return info;                                             \
    }

#define NB_INT 100
static int IntArray[NB_INT];

static void init_int_array ()
{
  int i, gen;
  gen = PNL_RNG_MERSENNE_RANDOM_SEED;
  pnl_rand_init (gen, NB_INT, 1);
  for ( i=0 ; i<NB_INT ; i++ ) { IntArray[i] = (int) (pnl_rand_uni (gen) * INT_MAX); }
}

/*
 * Send / Recv examples
 */

static int send_vector ()
{
  PnlVect *v;
  int info, n =5; 
  int gen = PNL_RNG_MERSENNE_RANDOM_SEED;
  v = pnl_vect_new ();
  pnl_rand_init (gen, n, 1);
  pnl_vect_rand_uni (v, n, 0, 1, gen);
  printf ("Original vector \n"); pnl_vect_print (v); printf ("\n");
  info = pnl_object_mpi_send (PNL_OBJECT(v), 1, SENDTAG, MPI_COMM_WORLD);
  pnl_vect_free (&v);
  return info;
}


static int recv_vector ()
{
  MPI_Status status;
  PnlVect *v;
  int info;
  v = pnl_vect_new ();
  info = pnl_object_mpi_recv (PNL_OBJECT(v), 0, SENDTAG, MPI_COMM_WORLD, &status);
  printf ("Received vector \n"); pnl_vect_print (v); printf ("\n");
  pnl_vect_free (&v);
  return info;
}

static int send_int_vector ()
{
  PnlVectInt *v;
  int info, n =5; 
  v = pnl_vect_int_create_from_ptr (n, IntArray);
  printf ("Original vector \n"); pnl_vect_int_print (v); printf ("\n");
  info = pnl_object_mpi_send (PNL_OBJECT(v), 1, SENDTAG, MPI_COMM_WORLD);
  pnl_vect_int_free (&v);
  return info;
}


static int recv_int_vector ()
{
  MPI_Status status;
  PnlVectInt *v;
  int info;
  v = pnl_vect_int_new ();
  info = pnl_object_mpi_recv (PNL_OBJECT(v), 0, SENDTAG, MPI_COMM_WORLD, &status);
  printf ("Received vector \n"); pnl_vect_int_print (v); printf ("\n");
  pnl_vect_int_free (&v);
  return info;
}

static int send_complex_vector ()
{
  PnlVect *v;
  PnlVectComplex *M;
  int info, m = 5;
  int gen = PNL_RNG_MERSENNE_RANDOM_SEED;
  v = pnl_vect_new ();
  pnl_rand_init (gen, m, 1);
  pnl_vect_rand_normal (v, 2*m, gen);
  M = pnl_vect_complex_create_from_ptr (m, (dcomplex *)v->array);
  printf ("Original vector \n"); pnl_vect_complex_print (M); printf ("\n");
  info = pnl_object_mpi_send (PNL_OBJECT(M), 1, SENDTAG, MPI_COMM_WORLD);
  pnl_vect_complex_free (&M);
  pnl_vect_free (&v);
  return info;
}

static int recv_complex_vector ()
{
  MPI_Status status;
  PnlVectComplex *M;
  int info;
  M = pnl_vect_complex_new ();
  info = pnl_object_mpi_recv (PNL_OBJECT(M), 0, SENDTAG, MPI_COMM_WORLD, &status);
  printf ("Received vector \n"); pnl_vect_complex_print (M); printf ("\n");
  pnl_vect_complex_free (&M);
  return info;
}


static int send_matrix ()
{
  PnlMat *M;
  int info, m = 4, n =5; 
  int gen = PNL_RNG_MERSENNE_RANDOM_SEED;
  M = pnl_mat_new ();
  pnl_rand_init (gen, m, n);
  pnl_mat_rand_normal (M, m, n, gen);
  printf ("Original matrix \n"); pnl_mat_print (M); printf ("\n");
  info = pnl_object_mpi_send (PNL_OBJECT(M), 1, SENDTAG, MPI_COMM_WORLD);
  pnl_mat_free (&M);
  return info;
}

static int recv_matrix ()
{
  MPI_Status status;
  PnlMat *M;
  int info;
  M = pnl_mat_new ();
  info = pnl_object_mpi_recv (PNL_OBJECT(M), 0, SENDTAG, MPI_COMM_WORLD, &status);
  printf ("Received matrix \n"); pnl_mat_print (M); printf ("\n");
  pnl_mat_free (&M);
  return info;
}

static int send_int_matrix ()
{
  PnlMatInt *M;
  int info, n =5, m=4; 
  M = pnl_mat_int_create_from_ptr (m, n, IntArray);
  printf ("Original matrix \n"); pnl_mat_int_print (M); printf ("\n");
  info = pnl_object_mpi_send (PNL_OBJECT(M), 1, SENDTAG, MPI_COMM_WORLD);
  pnl_mat_int_free (&M);
  return info;
}


static int recv_int_matrix ()
{
  MPI_Status status;
  PnlMatInt *M;
  int info;
  M = pnl_mat_int_new ();
  info = pnl_object_mpi_recv (PNL_OBJECT(M), 0, SENDTAG, MPI_COMM_WORLD, &status);
  printf ("Received matrix \n"); pnl_mat_int_print (M); printf ("\n");
  pnl_mat_int_free (&M);
  return info;
}

static int send_complex_matrix ()
{
  PnlVect *v;
  PnlMatComplex *M;
  int info, m = 4, n =5;
  int gen = PNL_RNG_MERSENNE_RANDOM_SEED;
  v = pnl_vect_new ();
  pnl_rand_init (gen, m, n);
  pnl_vect_rand_normal (v, 2*m*n, gen);
  M = pnl_mat_complex_create_from_ptr (m, n, (dcomplex *)v->array);
  printf ("Original matrix \n"); pnl_mat_complex_print (M); printf ("\n");
  info = pnl_object_mpi_send (PNL_OBJECT(M), 1, SENDTAG, MPI_COMM_WORLD);
  pnl_mat_complex_free (&M);
  pnl_vect_free (&v);
  return info;
}

static int recv_complex_matrix ()
{
  MPI_Status status;
  PnlMatComplex *M;
  int info;
  M = pnl_mat_complex_new ();
  info = pnl_object_mpi_recv (PNL_OBJECT(M), 0, SENDTAG, MPI_COMM_WORLD, &status);
  printf ("Received matrix \n"); pnl_mat_complex_print (M); printf ("\n");
  pnl_mat_complex_free (&M);
  return info;
}


static int send_bandmatrix ()
{
  PnlMat *M;
  PnlBandMat *BM;
  int info, m = 4, n =5; 
  int gen = PNL_RNG_MERSENNE_RANDOM_SEED;
  M = pnl_mat_new ();
  pnl_rand_init (gen, m, n);
  pnl_mat_rand_normal (M, m, n, gen);
  BM = pnl_band_mat_create_from_mat (M, 3,1);
  printf ("Original Band matrix \n"); pnl_band_mat_print_as_full (BM); printf ("\n");
  info = pnl_object_mpi_send (PNL_OBJECT(BM), 1, SENDTAG, MPI_COMM_WORLD);
  pnl_mat_free (&M);
  pnl_band_mat_free (&BM);
  return info;
}

static int recv_bandmatrix ()
{
  MPI_Status status;
  PnlBandMat *M;
  int info;
  M = pnl_band_mat_new ();
  info = pnl_object_mpi_recv (PNL_OBJECT(M), 0, SENDTAG, MPI_COMM_WORLD, &status);
  printf ("Received Band matrix \n"); pnl_band_mat_print_as_full (M); printf ("\n");
  pnl_band_mat_free (&M);
  return info;
}

static int send_tridiagmatrix ()
{
  PnlMat *M;
  PnlTridiagMat *TM;
  int info, m = 5; 
  int gen = PNL_RNG_MERSENNE_RANDOM_SEED;
  M = pnl_mat_new ();
  pnl_rand_init (gen, m, m);
  pnl_mat_rand_normal (M, m, m, gen);
  TM = pnl_tridiag_mat_create_from_mat (M);
  printf ("Original Tridiag matrix \n"); pnl_tridiag_mat_print (TM); printf ("\n");
  info = pnl_object_mpi_send (PNL_OBJECT(TM), 1, SENDTAG, MPI_COMM_WORLD);
  pnl_mat_free (&M);
  pnl_tridiag_mat_free (&TM);
  return info;
}

static int send_basis ()
{
  PnlBasis *B;
  int info, index, degree, spaced;
  index = CANONICAL;
  degree = 4;
  spaced =3;
  B = pnl_basis_create_from_degree (index, degree, spaced);
  printf ("Original Basis \n"); pnl_basis_print (B); printf ("\n");
  info = pnl_object_mpi_send (PNL_OBJECT(B), 1, SENDTAG, MPI_COMM_WORLD);
  PNL_MPI_MESSAGE (info, "Basis not sent\n");
  pnl_basis_free (&B);
  return info;
}


static int recv_tridiagmatrix ()
{
  MPI_Status status;
  PnlTridiagMat *M;
  int info;
  M = pnl_tridiag_mat_new ();
  info = pnl_object_mpi_recv (PNL_OBJECT(M), 0, SENDTAG, MPI_COMM_WORLD, &status);
  printf ("Received Tridiag matrix \n"); pnl_tridiag_mat_print (M); printf ("\n");
  pnl_tridiag_mat_free (&M);
  return info;
}

static int send_int_hmatrix ()
{
  PnlHmatInt *M;
  int info, m = 4, n =2, p=3;
  int dims[3]; dims[0] = m; dims[1] = n; dims[2] = p;
  M = pnl_hmat_int_create_from_ptr (3, dims, IntArray);
  printf ("Original Hmatrix \n"); pnl_hmat_int_print (M); printf ("\n");
  info = pnl_object_mpi_send (PNL_OBJECT(M), 1, SENDTAG, MPI_COMM_WORLD);
  
  printf ("Hmat sent\n");
  pnl_hmat_int_free (&M);
  return info;
}

static int recv_int_hmatrix ()
{
  MPI_Status status;
  PnlHmatInt *M;
  int info;
  M = pnl_hmat_int_new ();
  info = pnl_object_mpi_recv (PNL_OBJECT(M), 0, SENDTAG, MPI_COMM_WORLD, &status);
  printf ("Received Hmatrix \n"); pnl_hmat_int_print (M); printf ("\n");
  pnl_hmat_int_free (&M);
  return info;
}

static int recv_basis ()
{
  MPI_Status status;
  PnlBasis *B;
  int info;
  B = pnl_basis_new ();
  info = pnl_object_mpi_recv (PNL_OBJECT(B), 0, SENDTAG, MPI_COMM_WORLD, &status);
  printf ("Received basis\n"); pnl_basis_print (B); printf ("\n");
  pnl_basis_free (&B);
  return info;
}

/*
 * Isend / Irecv examples
 */

static int isend_matrix ()
{
  MPI_Request request;
  MPI_Status status;
  PnlMat *M;
  int info, m = 4, n =5; 
  int gen = PNL_RNG_MERSENNE_RANDOM_SEED;
  M = pnl_mat_new ();
  pnl_rand_init (gen, m, n);
  pnl_mat_rand_normal (M, m, n, gen);
  printf ("Original matrix (Isend)\n"); pnl_mat_print (M); printf ("\n");
  info = pnl_object_mpi_isend (PNL_OBJECT(M), 1, SENDTAG, MPI_COMM_WORLD, &request);
  info = MPI_Wait (&request, &status);  
  pnl_mat_free (&M);
  return info;
}

static int status_is_empty (MPI_Status *status)
{
  if ( status->MPI_TAG == MPI_ANY_TAG && status->MPI_SOURCE == MPI_ANY_SOURCE
       && status->MPI_ERROR == MPI_SUCCESS )
    return TRUE;
  else return FALSE;
}

static int irecv_matrix ()
{
  MPI_Request request;
  MPI_Status status;
  void *buf;
  PnlMat *M;
  int flag,info, size, pos=0;
  M = pnl_mat_new ();
  /* Wait until a message is available for reception */
  do
    {
      info = pnl_object_mpi_irecv (&buf, &size, 0, SENDTAG, MPI_COMM_WORLD, &flag, &request);
    }
  while (flag == FALSE);
  PNL_MPI_MESSAGE (info, "error in pnl_object_mpi_irecv\n");
  MPI_Test (&request, &flag, &status);
  if ( status_is_empty (&status) == TRUE )
    {
      printf ("Emtpy status\n");
      return MPI_ERR_REQUEST;
    }
  if ( flag == FALSE )
    {
      info = MPI_Wait (&request, &status);
    }
  pnl_object_mpi_unpack (PNL_OBJECT(M), buf, size, &pos, MPI_COMM_WORLD);
  printf ("Received matrix (Irecv)\n"); pnl_mat_print (M); printf ("\n");
  pnl_mat_free (&M);
  free (buf);
  return info;
}


/*
 * Bcast examples
 */

static int bcast_matrix (PnlMat *M)
{
  int rank, info, m = 4, n =5; 
  int gen = PNL_RNG_MERSENNE_RANDOM_SEED;
  MPI_Comm_rank (MPI_COMM_WORLD, &rank);

  if ( rank == 0 )
    {
      pnl_rand_init (gen, m, n);
      pnl_mat_rand_normal (M, m, n, gen);
      printf ("Original matrix (Bcast)\n"); pnl_mat_print (M); printf ("\n");
    }
  info = pnl_object_mpi_bcast (PNL_OBJECT(M), 0, MPI_COMM_WORLD);
  printf ("Received matrix (Bcast, rank = %d)\n", rank); pnl_mat_print (M); printf ("\n");
  return info;
}



int main(int argc, char *argv[])
{
  PnlMat *M;
  int rank, nproc;
  MPI_Init (&argc, &argv);
  MPI_Comm_size (MPI_COMM_WORLD, &nproc);
  MPI_Comm_rank (MPI_COMM_WORLD, &rank);

  pnl_message_off();
  if ( nproc != 2 )
    {
      if ( rank == 0 )
        {
          printf("Run the test with -np 2.\n");
        }
      MPI_Finalize ();
      exit (0);
    }

  if ( rank == 0 )
    {
      /* initializes some integers for the tests */
      init_int_array ();
      send_vector (); MPI_Barrier (MPI_COMM_WORLD);
      send_int_vector (); MPI_Barrier (MPI_COMM_WORLD);
      send_complex_vector (); MPI_Barrier (MPI_COMM_WORLD);
      send_matrix (); MPI_Barrier (MPI_COMM_WORLD);
      send_int_matrix (); MPI_Barrier (MPI_COMM_WORLD);
      send_complex_matrix (); MPI_Barrier (MPI_COMM_WORLD);
      send_bandmatrix (); MPI_Barrier (MPI_COMM_WORLD);
      send_tridiagmatrix (); MPI_Barrier (MPI_COMM_WORLD);
      send_int_hmatrix (); MPI_Barrier (MPI_COMM_WORLD);
      isend_matrix (); MPI_Barrier (MPI_COMM_WORLD);
      send_matrix (); MPI_Barrier (MPI_COMM_WORLD);
      send_basis (); MPI_Barrier (MPI_COMM_WORLD);
    }
  else
    {
      recv_vector (); MPI_Barrier (MPI_COMM_WORLD);
      recv_int_vector (); MPI_Barrier (MPI_COMM_WORLD);
      recv_complex_vector (); MPI_Barrier (MPI_COMM_WORLD);
      recv_matrix (); MPI_Barrier (MPI_COMM_WORLD);
      recv_int_matrix (); MPI_Barrier (MPI_COMM_WORLD);
      recv_complex_matrix (); MPI_Barrier (MPI_COMM_WORLD);
      recv_bandmatrix (); MPI_Barrier (MPI_COMM_WORLD);
      recv_tridiagmatrix (); MPI_Barrier (MPI_COMM_WORLD);
      recv_int_hmatrix (); MPI_Barrier (MPI_COMM_WORLD);
      recv_matrix (); MPI_Barrier (MPI_COMM_WORLD);
      irecv_matrix (); MPI_Barrier (MPI_COMM_WORLD);
      recv_basis (); MPI_Barrier (MPI_COMM_WORLD);
    }

  M = pnl_mat_new ();
  bcast_matrix (M);
  pnl_mat_free (&M);
  MPI_Barrier (MPI_COMM_WORLD);
  
  MPI_Finalize ();
  exit (0);
}
