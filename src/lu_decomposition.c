#include "common.h"

// To LU decomposition, use Doolittle method
int get_lu_decomposition_matrix(int *matrix_a, int *matrix_l, int *matrix_u, int matrix_length) {
  int sum = 0;

  for(int i = 0; i < matrix_length; i++) {

    // L diagonal element (1,1) ... (i,i)  = 1
    matrix_l[i * matrix_length + i] = 1;

    for(int j = 0; j < matrix_length; j++) {
      sum = 0;

      if(i > j) {
	// L need (i > j) pattern

	// Low matrix Sum = L(i,1) * U(1,j) + ... + L(i, j-1) * U(j-1,j)
	for(int k = 0; k < j; k++)
	  sum += matrix_l[i * matrix_length + k] * matrix_u[k * matrix_length + j];
	
	// L (i,j) = ( A(i,j) - low_matrix_sum ) / U(j,j)
	// Need Pivot
	matrix_l[i * matrix_length + j] = (matrix_a[i * matrix_length + j] - sum) / matrix_u[j * matrix_length + j];

	if(matrix_u[j * matrix_length + j] != 1 && matrix_u[j * matrix_length + j] != -1)
	  printf("U[%d %d] = %d / %d, Need pivot\n", i, j, matrix_a[i * matrix_length + j] - sum, matrix_u[j * matrix_length + j]);
	
      } else {
	// U need (i <= j) pattern

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
  if ( !compare_matrix_multi (matrix_l, matrix_u, matrix_a, matrix_length) ) {
    // Check LU = A or Not
    printf("Calculate LU != A\n");
    return 0;
  }
#endif

  return 1;
}
