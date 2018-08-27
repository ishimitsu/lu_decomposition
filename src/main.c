#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DBG_PRINT 1

#define INPUT_MATRIX "input_matrix.txt"
#define BUF_LEN 1024

struct matrix {
  int length;
  int *matrix;
} typedef MATRIX;

extern int *init_square_matrix(int length);
extern void free_square_matrix(int *matrix);
extern int get_matrix_elements(int *matrix, char *input, int now_line, int matrix_length);
extern int *get_inverse_matrix (int *matrix_a, int matrix_length);

MATRIX *make_square_matrix (char *filename) {
  FILE *fp;
  char input[BUF_LEN];
  int square_matrix_length;
  int now_line = 0;
  int now_matrix_line = 0;
  int *input_matrix;
  MATRIX *ret;

  if ( (fp = fopen(INPUT_MATRIX, "r") ) == NULL ) {
    printf("Error: Open %s\n", INPUT_MATRIX);
    return NULL;
  }

  while( fgets(input, sizeof(input), fp) != NULL) {

    if(now_line == 0 && square_matrix_length == 0) {
      // initialize square matrix
      square_matrix_length = atoi(input);
      input_matrix = init_square_matrix(square_matrix_length);

    } else {
      // Add elements to square matrix
      if(now_matrix_line < square_matrix_length) {

	if( !(get_matrix_elements(input_matrix, input, 
				  now_matrix_line, square_matrix_length)) ) {
	  printf("Invalid Matrix\n");
	  return NULL;
	}
				
      } else {
	printf("Max Matrix Line is %d, so line %d is ignored\n",
	       square_matrix_length, now_matrix_line);
      }

      now_matrix_line++;

    }

    now_line++;
  }

  fclose(fp);

  ret = memset(malloc(sizeof(MATRIX)), 0, sizeof(MATRIX));
  ret->length = square_matrix_length;
  ret->matrix = input_matrix;

  return ret;
}

int main(void) {
  MATRIX *input_matrix;
  int *inverse_matrix;

  if( (input_matrix = make_square_matrix(INPUT_MATRIX)) == NULL) {
    return -1;
  }

#ifdef DBG_PRINT
  printf("Input %d Length Square Matrix\n", input_matrix->length);
  for(int i = 0; i < input_matrix->length; i++) {
    for(int j = 0; j < input_matrix->length; j++) {
      printf("%d ", input_matrix->matrix[i * input_matrix->length + j]);
    }
    printf("\n");
  }
  printf("\n");
#endif

  inverse_matrix = get_inverse_matrix (input_matrix->matrix, input_matrix->length);

  free_square_matrix(inverse_matrix);
  free_square_matrix(input_matrix->matrix);
  free(input_matrix);

  return 0;
}

