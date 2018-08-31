#include "common.h"
#include "matrix.h"

int select_pivot (double *matrix_a, int matrix_length) {
  double max, change_line = 0;
  double *temp;

  temp = calloc ( matrix_length, sizeof(double) );  

  for(int i = 0; i < matrix_length; i++) {
    if(matrix_a[i * matrix_length + i] == 0) {
      max = matrix_a[i * matrix_length];
      change_line = 0;

      for(int j = 0; i < matrix_length; i++) {
	if(max < matrix_a[i * matrix_length + j]) {
	  max = matrix_a[i * matrix_length + j];
	  change_line = j;
	}

      }

      

    }
  }
  
  return 1;
}


int get_lu_decomposition_matrix(double *matrix_a_base, double *matrix_l, double *matrix_u, int matrix_length) {
  double sum, pivot;
  double *matrix_a;

  matrix_a = calloc ( matrix_length * matrix_length, sizeof(double) );  
  memcpy (matrix_a, matrix_a_base, matrix_length * matrix_length * sizeof(double));

  for(int i = 0; i < matrix_length; i++) {

    // L diagonal (1,1) ... (i,i)  = 1
    matrix_l[i * matrix_length + i] = 1;

    for(int j = 0; j < matrix_length; j++) {
      sum = 0;

      if(i > j) {

	pivot = matrix_u[j * matrix_length + j];
	if(!pivot) {
	  // If U diagonal(i, i) = 0, need pivot selection to avoid 0 division
	  // select_pivot (matrix_a, matrix_u, matrix_length);
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

  free(matrix_a);

  return 1;
}
