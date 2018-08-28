#include "common.h"

extern int *malloc_square_matrix(int length);

#if DBG_PRINT_LU_INV_MATRIX == 1
int check_matrix_is_inverse (int *matrix, int *matrix_inv, int matrix_length) {
  int sum = 0;

  for(int i = 0; i < matrix_length; i++) {
    for(int j = 0; j < matrix_length; j++) {

      sum = 0;
      printf("[%d %d] = ", i, j);
      for(int k = 0; k < matrix_length; k++) {
	sum += matrix[i * matrix_length + k] * matrix_inv[k * matrix_length + j];
	printf("%d * %d + ", matrix[i * matrix_length + k], matrix_inv[k * matrix_length + j]);
      }
      printf("= %d\n", sum);

      if( (i == j && sum != 1) || (i != j && sum != 0) ) {
	printf("Diagonal [%d %d] = %d, Matrix doesn't Inversed\n", i, j, sum);
	return 0;
      }
    }

  }
  
  return 1;
}
#endif


int get_lu_inverse_matrix (int *matrix_l, int *matrix_u, int *matrix_l_inv, int *matrix_u_inv, int matrix_length) {
  int detL, detU;
  int sum = 0;

  // detLU only need diagonal element, other elements should be 0
  detL = 1;
  detU = 1;
  for(int i = 0; i < matrix_length; i++) {
    detU *= matrix_u[matrix_length * i + i];
  }
  if (detU == 0) {
    printf("detU = 0, cannot inverse U Matrix\n");
    return 0;
  }


  for(int i = 0; i < matrix_length; i++) {
    for(int j = 0; j < i; j++) {

      sum = 0;
      // SUM = L(i, j) * L_INV(j, j)) + ... + L(i, i-1) * L_INV(i-1, j)
      for(int k = j; k < i; k++)
	sum += matrix_l[i * matrix_length + k] * matrix_l_inv[k * matrix_length + j];
	
      // L_INV = - (SUM / L (i, i))
      matrix_l_inv[i * matrix_length + j] = - (sum / matrix_l[i * matrix_length + i]);
      
    }

    // i == j (L_INV Diagonal elements = 1)
    matrix_l_inv[i * matrix_length + i] = 1 / matrix_l[i * matrix_length + i];
  }


  for(int i = matrix_length-1; i >= 0; i--) {

    // i == j
    matrix_u_inv[i * matrix_length + i] = 1 / matrix_u[i * matrix_length + i];
    
    for(int j = i+1; j < matrix_length; j++) {

      sum = 0;
      // SUM = U(i, i+1) * U_INV(i+1, j)) + ... + U(i, j) * U_INV(j, j)
      for(int k = i+1; k < j; k++)
	sum += matrix_u[i * matrix_length + k] * matrix_u_inv[k * matrix_length + j];
	
      // L_INV = - (SUM / L (i, i))
      matrix_u_inv[i * matrix_length + j] = - (sum / matrix_u[i * matrix_length + i]);

    }
  }

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

#if DBG_PRINT_LU_INV_MATRIX == 1
  printf("Inverse Matrix L\n");
  print_matrix(matrix_l_inv, matrix_length);
  if (!check_matrix_is_inverse (matrix_l, matrix_l_inv, matrix_length) ) {
    printf("Failed Inverse L Matrix\n");
    return 0;
  }

  printf("Inverse Matrix U\n");
  print_matrix(matrix_u_inv, matrix_length);
  if (!check_matrix_is_inverse (matrix_u, matrix_u_inv, matrix_length) ) {
    printf("Failed Inverse U Matrix\n");
    return 0;
  }

#endif


  // Inverse A = Inverse U * Inverse L 

  // matrix_1/u_inv free at  free_all_malloc_matrix ()

  return 1;
}

