#include "common.h"
#include "matrix.h"

#define INPUT_MATRIX "input_matrix.txt"

extern int get_lu_decomposition_matrix(int *matrix_a, int *matrix_l, int *matrix_u, int matrix_length);
extern int get_inverse_matrix_with_lu_decomp (int *matrix_l, int *matrix_u, int *matrix_a_inv, int length);
extern int check_matrix_is_inverse (int *matrix, int *matrix_inv, int matrix_length);

int main(void) {
  int matrix_length;
  int *matrix_a, *matrix_l, *matrix_u, *matrix_a_inv;

  if( (matrix_length = get_input_matrix_length(INPUT_MATRIX)) == 0) {
    printf("Invalid Matrix Length %d\n", matrix_length);
    goto end;
  }

  matrix_a = malloc_square_matrix(matrix_length);
  if( !get_input_square_matrix(INPUT_MATRIX, matrix_a, matrix_length) ) {
    printf("Failed Get Input Matrix\n");
    goto end;
  }

#if DBG_PRINT_INPUT_MATRIX == 1
  printf("Matrix A\n");
  print_matrix(matrix_a, matrix_length);
#endif

  matrix_l = malloc_square_matrix(matrix_length);
  matrix_u = malloc_square_matrix(matrix_length);
  if( !get_lu_decomposition_matrix(matrix_a, matrix_l, matrix_u, matrix_length) ) {
    printf("Failed LU Decomposition\n");
    goto end;
  }

  matrix_a_inv = malloc_square_matrix(matrix_length);
  if (!get_inverse_matrix_with_lu_decomp (matrix_l, matrix_u, matrix_a_inv, matrix_length) ) {
    printf("Failed Inverse MATRIX A\n");
    goto end;
  }

  printf("Inversed A Matrix\n");
  print_matrix(matrix_a_inv, matrix_length);

#if CHECK_A_INV_RESULT == 1
  if (!check_matrix_is_inverse (matrix_a, matrix_a_inv, matrix_length) ) {
    printf("Invalid Inverse A Matrix\n");
    goto end;
  }
#endif

 end:
  free_all_malloc_matrix();

  return 0;
}

