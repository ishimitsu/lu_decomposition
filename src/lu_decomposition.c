#include "common.h"
#include "matrix.h"

int select_pivot (double *matrix_a, int matrix_length) {
  double max, change_line = 0;
  double *temp;
  
  return 1;
}


int get_lu_decomposition_matrix(double *matrix_a, double *matrix_l, double *matrix_u, int matrix_length) {
  double sum, pivot;

  for(int i = 0; i < matrix_length; i++) {

    // L diagonal (1,1) ... (i,i)  = 1
    matrix_l[i * matrix_length + i] = 1;

    for(int j = 0; j < matrix_length; j++) {
      sum = 0;

      if(i > j) {

	pivot = matrix_u[j * matrix_length + j];
	if(fabs(pivot) < EPSILON) {
	  // If U diagonal(i, i) = 0, need pivot selection to avoid 0 division
	  // select_pivot (matrix_a, matrix_u, matrix_length);
	  printf("U[%d %d] is 0, pivot selection invalid\n", j, j);
	  return 0;
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

  return 1;
}
