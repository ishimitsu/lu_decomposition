#include "common.h"
#include "matrix.h"

#define INPUT_MATRIX "input_matrix.txt"

extern int get_lu_decomposition_matrix(double *matrix_a, double *matrix_l, double *matrix_u, int matrix_length);
extern int get_inverse_matrix_with_lu_decomp (double *matrix_l, double *matrix_u, double *matrix_p, double *matrix_a_inv, int matrix_length);

#if CHECK_A_INV_RESULT == 1
extern int check_matrix_is_inverse (double *matrix, double *matrix_inv, int matrix_length);
#endif

int main(void) {
  int matrix_length;
  double *matrix_a_base, *matrix_a, *matrix_p, *matrix_l, *matrix_u, *matrix_a_inv;
  char *input_matrix_file = INPUT_MATRIX;

  // Get Input Matrix
  if( !(matrix_length = get_input_matrix_length(input_matrix_file)) ) {
    printf("Failed Get Matrix Length from %s\n", input_matrix_file);
    goto end;
  }

  matrix_a_base = malloc_square_matrix(matrix_length);
  if( !get_input_square_matrix_element(INPUT_MATRIX, matrix_a_base, matrix_length) ) {
    printf("Failed Get Input Matrix\n");
    goto end;
  }

#if DBG_PRINT_INPUT_MATRIX == 1
  printf("Matrix A\n");
  print_matrix(matrix_a_base, matrix_length);
#endif

  // A = P * A(pivot selected)
  matrix_a = malloc_square_matrix(matrix_length);
  memcpy(matrix_a, matrix_a_base, matrix_length * matrix_length * sizeof(double));
  matrix_p = malloc_square_matrix(matrix_length);
  if( !pivot_selection (matrix_a, matrix_p, matrix_length) ) {
    printf("Input Matrix can't be lu decompose\n");
    goto end;
  }

  // Calculate PA = LU
  matrix_l = malloc_square_matrix(matrix_length);
  matrix_u = malloc_square_matrix(matrix_length);
  if( !get_lu_decomposition_matrix(matrix_a, matrix_l, matrix_u, matrix_length) ) {
    printf("Failed LU Decomposition\n");
    goto end;
  }
#if DBG_PRINT_LU_MATRIX == 1
  printf("Matrix L\n");
  print_matrix(matrix_l, matrix_length);

  printf("Matrix U\n");
  print_matrix(matrix_u, matrix_length);
#endif

#if CHECK_LU_DECOMP_RESULT == 1
  printf("Check PA = LU or not ... ");
  if ( !compare_matrix_multi (matrix_l, matrix_u, matrix_a, matrix_length) ) {
    // Check LU = A or Not
    printf("NG\n");
    goto end;
  }
  printf("OK\n");
  printf("\n");
#endif

  // Calculate U_UNV * L_INV * P = A_INV
  matrix_a_inv = malloc_square_matrix(matrix_length);
  if (!get_inverse_matrix_with_lu_decomp (matrix_l, matrix_u, matrix_p, matrix_a_inv, matrix_length) ) {
    printf("Failed Inverse MATRIX A\n");
    goto end;
  }

  printf("Inversed A Matrix\n");
  print_matrix(matrix_a_inv, matrix_length);

#if CHECK_A_INV_RESULT == 1
  printf("Check A * A_INV = A_INV * A = E ... ");
  if (!check_matrix_is_inverse (matrix_a_base, matrix_a_inv, matrix_length) ) {
    printf("NG\n");
    goto end;
  }
  printf("OK \n");
#endif

 end:
  free_all_malloc_matrix();

  return 0;
}

