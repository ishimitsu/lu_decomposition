#include "common.h"

extern int *malloc_square_matrix(int length);

int get_lu_inverse_matrix (int *matrix_l, int *matrix_u, int *matrix_l_inv, int *matrix_u_inv, int matrix_length) {
  int detL, detU;

  // detLU only need diagonal element, other elements should be 0
  detL = 1;
  for(int i = 0, detU = 1; i < matrix_length; i++) {
    detU *= matrix_u[matrix_length * i + i];
  }
  if (detU == 0) {
    printf("detU = 0, cannot inverse U Matrix\n");
    return 0;
  }

  for(int i = 0; i < matrix_length; i++) {
    for(int j = 0; j < matrix_length; j++) {

      if(i > j) {
	int sum = 0;

	for(int k = j; k < i; k++)
	  sum += matrix_l[i * matrix_length + k] * matrix_l_inv[k * matrix_length + j];
	
	matrix_l_inv[i * matrix_length + j] = - (sum / matrix_l[i * matrix_length + i]);
	
      } else {
	int sum = 0;
	
	if(i == j) {
	  matrix_l_inv[i * matrix_length + j] = 1 / matrix_l[i * matrix_length + j];
	  matrix_u_inv[i * matrix_length + j] = 1 / matrix_u[i * matrix_length + j];
	}
	
	for(int k = i+1; k < j; k++)
	  sum += matrix_u[i * matrix_length + k] * matrix_u_inv[k * matrix_length + j];
	
	matrix_u_inv[i * matrix_length + j] = - (sum / matrix_u[i * matrix_length + i]);

      }

    }
  }

#if DBG_PRINT_LU_INV_MATRIX == 1
  printf("Inverse Matrix L\n");
  print_matrix(matrix_l_inv, matrix_length);
  printf("Inverse Matrix U\n");
  print_matrix(matrix_u_inv, matrix_length);
#endif

  return 1;

}

int get_inverse_matrix_with_lu_decomp (int *matrix_l, int *matrix_u, int *matrix_a_inv, int matrix_length) {
  int *matrix_l_inv, *matrix_u_inv;

  matrix_l_inv = malloc_square_matrix(matrix_length);
  matrix_u_inv = malloc_square_matrix(matrix_length);
  if ( !get_lu_inverse_matrix (matrix_l, matrix_u, matrix_l_inv, matrix_u_inv, matrix_length) ) {
    printf("Can't inverse LU Matrix\n");
    return 0;
  }

  // Inverse A = Inverse U * Inverse L 

  // matrix_1/u_inv free at  free_all_malloc_matrix ()

  return 1;
}

