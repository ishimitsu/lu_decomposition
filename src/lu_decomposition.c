#include "common.h"
#include "matrix.h"

int get_lu_decomposition_matrix(double *matrix_a, double *matrix_l, double *matrix_u, int matrix_length) {
  double sum, pivot;

  for(int i = 0; i < matrix_length; i++) {

    // L diagonal (1,1) ... (i,i)  = 1
    matrix_l[i * matrix_length + i] = 1;

    for(int j = 0; j < matrix_length; j++) {
      sum = 0;

      if(i > j) {

	pivot = matrix_u[j * matrix_length + j];
	if(!pivot) {
	  // need pivot selection
	  printf("U[%d %d] is 0, need pivot selection\n", j, j);
	}

	// Low matrix Sum = L(i,1) * U(1,j) + ... + L(i, j-1) * U(j-1,j)
	for(int k = 0; k < j; k++)
	  sum += matrix_l[i * matrix_length + k] * matrix_u[k * matrix_length + j];
	
	// L (i,j) = ( A(i,j) - low_matrix_sum ) / U(j,j)
	matrix_l[i * matrix_length + j] = (matrix_a[i * matrix_length + j] - sum) / pivot;
      
      } else {
	// Upper matrix Sum = L(i,1) * U(1,j) + ... L(i, i-1) * U(i-1,j)
	for(int k = 0; k < i; k++)
	  sum += matrix_l[i * matrix_length + k] * matrix_u[k * matrix_length + j];
	
	// U (i,j) = A(i,j) - upper_matrix_sum
	matrix_u[i * matrix_length + j] = matrix_a[i * matrix_length + j] - sum;
      }
      
    }
  }
      
#if DBG_PRINT_LU_MATRIX == 1
  printf("Matrix L\n");
  print_matrix(matrix_l, matrix_length);

  printf("Matrix U\n");
  print_matrix(matrix_u, matrix_length);
#endif

#if CHECK_LU_DECOMP_RESULT == 1
  printf("Check LU = A or not ... ");
  if ( !compare_matrix_multi (matrix_l, matrix_u, matrix_a, matrix_length) ) {
    // Check LU = A or Not
    printf("NG\n");
    return 0;
  }
  printf("OK\n");
  printf("\n");
#endif

  return 1;
}
