#include <stdio.h>

#define DBG_PRINT 1

extern int *init_square_matrix(int length);
extern void free_square_matrix(int *matrix);

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

int get_lu_decomposition_matrix(int *matrix_a, int *matrix_l, int *matrix_u, int matrix_length) {

  for(int i = 0; i < matrix_length; i++) {
    for(int j = 0; j < matrix_length; j++) {

      if(i > j) {
	int l_matrix_multi = 0;

	// Low matrix Multi = L(i,1) * U(1,j) + ... + L(i, j-1) * U(j-1,j)
	for(int k = 0; k < j; k++)
	  l_matrix_multi += matrix_l[i * matrix_length + k] * matrix_u[k * matrix_length + j];

	// L (i,j) = ( A(i,j) - low_matrix_multi ) / U(j,j)
	matrix_l[i * matrix_length + j] = (matrix_a[i * matrix_length + j] - l_matrix_multi) / matrix_u[j * matrix_length + j];

      } else  {
	int u_matrix_multi = 0;

	if(i == j) {
	  matrix_l[i * matrix_length + j] = 1;
	}

	// Upper matrix Multi = L(i,1) * U(1,j) + ... L(i, i-1) * U(i-1,j)
	for(int k = 0; k < i; k++)
	  u_matrix_multi += matrix_l[i * matrix_length + k] * matrix_u[k * matrix_length + j];

	// U (i,j) = A(i,j) - upper_matrix_multi
	matrix_u[i * matrix_length + j] = matrix_a[i * matrix_length + j] - u_matrix_multi;

      }

    }
  }
 
#ifdef DBG_PRINT
  printf("LU Decomposition Matrix: Lower\n");
  for(int i = 0; i < matrix_length; i++) {
    for(int j = 0; j < matrix_length; j++) {
	printf("%d ", matrix_l[i * matrix_length + j]);
    }
    printf("\n");
  }
  printf("\n");


  printf("LU Decomposition Matrix: Upper\n");
  for(int i = 0; i < matrix_length; i++) {
    for(int j = 0; j < matrix_length; j++) {
	printf("%d ", matrix_u[i * matrix_length + j]);
    }
    printf("\n");
  }
  printf("\n");

  check_lu_matrix_correct(matrix_a, matrix_l, matrix_u, matrix_length);
#endif

  return 1;
}


int *inverse_matrix_from_lu_decomposition (int *matrix_l, int *matrix_u, int length) {
  int *inverse_matrix;

  inverse_matrix = init_square_matrix(length);

  return inverse_matrix;
}


int *get_inverse_matrix (int *matrix_a, int matrix_length) {
  int *matrix_l;
  int *matrix_u;
  int *inverse_matrix;
  int detA;

  // detA = ...
  // if (detA == 0) ...)
  matrix_l = init_square_matrix(matrix_length);
  matrix_u = init_square_matrix(matrix_length);
  get_lu_decomposition_matrix(matrix_a, matrix_l, matrix_u, matrix_length);

  inverse_matrix = inverse_matrix_from_lu_decomposition (matrix_l, matrix_u, matrix_length);

  free_square_matrix(matrix_l);
  free_square_matrix(matrix_u);

  return inverse_matrix;

}
