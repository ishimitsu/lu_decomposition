#include "common.h"

void print_matrix(int *matrix, int matrix_length) {

  for(int i = 0; i < matrix_length; i++) {
    for(int j = 0; j < matrix_length; j++) {
      printf("%d ", matrix[i * matrix_length + j]);
    }
    printf("\n");
  }
  printf("\n");

}
