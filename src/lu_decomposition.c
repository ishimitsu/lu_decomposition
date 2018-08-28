#include "common.h"

#if DBG_PRINT_LU_MATRIX == 1
int check_lu_matrix_correct (int *matrix_a, int *matrix_l, int *matrix_u, int matrix_length) {

  for(int i = 0; i < matrix_length; i++) {
    for(int j = 0; j < matrix_length; j++) {
      int lu_total = 0;
      int end;

      if(i > j)
	end = i;
      else
	end = j;

      for(int k = 0; k <= end; k++)
	lu_total += matrix_l[i * matrix_length + k] * matrix_u[k * matrix_length + j];

      if(lu_total != matrix_a[i * matrix_length + j]) {
	printf("Invalid LU Matrix, A(%d %d)=%d, LU=%d\n", i ,j, matrix_a[i * matrix_length + j], lu_total);
	return 0;
      }

    }
  }
  
  return 1;
}
#endif

// To LU decomposition, use Doolittle method
int get_lu_decomposition_matrix(int *matrix_a, int *matrix_l, int *matrix_u, int matrix_length) {

  for(int i = 0; i < matrix_length; i++) {
    for(int j = 0; j < matrix_length; j++) {

      if(i > j) {
	int sum = 0;

	// Low matrix Multi = L(i,1) * U(1,j) + ... + L(i, j-1) * U(j-1,j)
	for(int k = 0; k < j; k++)
	  sum += matrix_l[i * matrix_length + k] * matrix_u[k * matrix_length + j];

	// L (i,j) = ( A(i,j) - low_matrix_multi ) / U(j,j)
	matrix_l[i * matrix_length + j] = (matrix_a[i * matrix_length + j] - sum) / matrix_u[j * matrix_length + j];

      } else  {
	int sum = 0;

	if(i == j) {
	  // L diagonal element (1,1) ... (i,i)  = 1
	  matrix_l[i * matrix_length + j] = 1;
	}

	// Upper matrix Multi = L(i,1) * U(1,j) + ... L(i, i-1) * U(i-1,j)
	for(int k = 0; k < i; k++)
	  sum += matrix_l[i * matrix_length + k] * matrix_u[k * matrix_length + j];

	// U (i,j) = A(i,j) - upper_matrix_multi
	matrix_u[i * matrix_length + j] = matrix_a[i * matrix_length + j] - sum;

      }

    }
  }

#if DBG_PRINT_LU_MATRIX == 1
  if ( !check_lu_matrix_correct(matrix_a, matrix_l, matrix_u, matrix_length) )
    return 0;
#endif

  return 1;
}
