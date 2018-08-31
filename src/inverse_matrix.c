#include "common.h"
#include "matrix.h"

int check_matrix_is_inverse (double *matrix, double *matrix_inv, int matrix_length) {
  double *matrix_e;
  int ret = 1;

  matrix_e = calloc ( matrix_length * matrix_length, sizeof(double));

  for(int i = 0; i < matrix_length; i++) {
    matrix_e[i * matrix_length + i] = 1;
  }

  // Check A * A_INV = E 
  if ( !compare_matrix_multi (matrix, matrix_inv, matrix_e, matrix_length) ) {
    ret = 0;
    goto end;
  }

  // Check A_INV * A = E 
  if ( !compare_matrix_multi (matrix_inv, matrix, matrix_e, matrix_length) ) {
    ret = 0;
    goto end;
  }

 end:

  free(matrix_e);
  
  return ret;
}


int get_lu_inverse_matrix (double *matrix_l, double *matrix_u, double *matrix_l_inv, double *matrix_u_inv, int matrix_length) {
  double detL, detU;
  double sum, pivot;

  // detLU only need diagonal numer, other numers should be 0
  detL = 1;
  detU = 1;
  for(int i = 0; i < matrix_length; i++) {
    detU *= matrix_u[matrix_length * i + i];
  }
  if (detU == 0) {
    printf("detU = 0, so Matrix A doesn't have inverse Matrix\n");
    return 0;
  }

  for(int i = 0; i < matrix_length; i++) {

    // L (i,i) always 1, so no need pivot select
    pivot = matrix_l[i * matrix_length + i]; 
    if(pivot != 1) {
      printf("L(%d %d) = %f, Invalid Lower Triangle Matrix\n", i, i, pivot);
      return 0;
    }

    for(int j = 0; j < i; j++) {

      sum = 0;
      // SUM = L(i, j) * L_INV(j, j)) + ... + L(i, i-1) * L_INV(i-1, j)
      for(int k = j; k < i; k++)
	sum += matrix_l[i * matrix_length + k] * matrix_l_inv[k * matrix_length + j];

      // L_INV = - (SUM / L (i, i)) = -1 * SUM
      matrix_l_inv[i * matrix_length + j] = - (sum / pivot);
      
    }

    // i == j (L_INV(i, i) diagonal numer always 1)
    matrix_l_inv[i * matrix_length + i] = 1 / pivot;
  }


  for(int i = matrix_length-1; i >= 0; i--) {

    pivot = matrix_u[i * matrix_length + i];
    if(!pivot) {
      // if U diagonal element = 0, need pivot selection
      printf("U[%d %d] is 0, need pivot selection\n", i, i);
    }

    // i == j (U_INV diagonal numer) = 1 / U (i, i)
    matrix_u_inv[i * matrix_length + i] = 1 / pivot;

    // for(int j = i+1; j < matrix_length; j++) {
    for(int j = matrix_length-1; j >= i+1; j--) {
      
      sum = 0;
      // SUM = U(i, i+1) * U_INV(i+1, j)) + ... + U(i, j) * U_INV(j, j)
      for(int k = i+1; k <= j; k++) {
	sum += matrix_u[i * matrix_length + k] * matrix_u_inv[k * matrix_length + j];
      }

      // U_INV = - (SUM / U (i, i))
      matrix_u_inv[i * matrix_length + j] = - (sum / pivot);

    }
  }

  return 1;

}

int get_inverse_matrix_with_lu_decomp (double *matrix_l, double *matrix_u, double *matrix_a_inv, int matrix_length) {
  double *matrix_l_inv, *matrix_u_inv;

  matrix_l_inv = malloc_square_matrix(matrix_length);
  matrix_u_inv = malloc_square_matrix(matrix_length);

  if ( !get_lu_inverse_matrix (matrix_l, matrix_u, matrix_l_inv, matrix_u_inv, matrix_length) ) {
    printf("Can't inverse LU Matrix\n");
    return 0;
  }

#if DBG_PRINT_LU_INV_MATRIX == 1
  printf("Inversed L Matrix\n");
  print_matrix(matrix_l_inv, matrix_length);

  printf("Inversed U Matrix\n");
  print_matrix(matrix_u_inv, matrix_length);
#endif 

#if CHECK_LU_INV_RESULT == 1
  printf("Check L * L_INV = L_INV * L = E ... ");
  if (!check_matrix_is_inverse (matrix_l, matrix_l_inv, matrix_length) ) {
    printf("NG\n");
    return 0;
  }
  printf("OK \n");

  printf("Check U * U_INV = U_INV * U = E ... ");
  if (!check_matrix_is_inverse (matrix_u, matrix_u_inv, matrix_length) ) {
    printf("NG\n");
    return 0;
  }
  printf("OK\n");
  printf("\n");
#endif

  // Inverse A = Inverse U * Inverse L 
  for(int i = 0; i < matrix_length; i++) {
    for(int j = 0; j < matrix_length; j++) {

      for(int k = 0; k < matrix_length; k++)
	matrix_a_inv[i * matrix_length + j] += matrix_u_inv[i * matrix_length + k] 
	  * matrix_l_inv[k * matrix_length + j] ;

    }
  }  

  // matrix_1/u_inv free at  free_all_malloc_matrix ()

  return 1;
}

