#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DBG_PRINT 1

#define INPUT_MATRIX "input_matrix.txt"
#define BUF_LEN 1024

struct matrix {
	int row;
	int line;
	int *matrix;
} typedef MATRIX;

extern int *init_matrix(int row, int line);
extern void free_square_matrix(int *matrix);
extern int get_matrix_elements(int *matrix, char *input, int now_line, int max_row);
extern int lu_decomposition (int *matrix, int row_max, int line_max);

MATRIX *make_matrix (char *filename) {
	FILE *fp;
	char input[BUF_LEN];
	int row, line;
	int now_input_line = 0;
	int now_matrix_line = 0;
	int *input_matrix;
	MATRIX *ret;

	if ( (fp = fopen(INPUT_MATRIX, "r") ) == NULL ) {
		printf("Error: Open %s\n", INPUT_MATRIX);
		return NULL;
	}

	while( fgets(input, sizeof(input), fp) != NULL) {

		if(now_input_line == 0) {
			line = atoi(input);

		} else if(now_input_line == 1) {
			row = atoi(input);

			// initialize square matrix
			input_matrix = init_matrix(row, line);

		} else {

			if(now_matrix_line < line) {

				if( !(get_matrix_elements(input_matrix, input, now_matrix_line, row)) ) {
					printf("Invalid Matrix\n");
					return NULL;
				}
				
			} else {
				printf("Max Matrix Line is %d, so line %d is ignored\n",
				       line, now_matrix_line);
			}
			
			now_matrix_line++;

		}

		now_input_line++;
	}

	fclose(fp);

	ret = memset(malloc(sizeof(MATRIX)), 0, sizeof(MATRIX));
	ret->row = row;
	ret->line = line;
	ret->matrix = input_matrix;

	return ret;
}

int main(void) {
	MATRIX *input_matrix;

	if( (input_matrix = make_matrix(INPUT_MATRIX)) == NULL) {
		return -1;
	}

#ifdef DBG_PRINT
	printf("Input Matrix : row %d, line %d\n", input_matrix->row, input_matrix->line);
	for(int i = 0; i < input_matrix->line; i++) {
		for(int j = 0; j < input_matrix->row; j++) {
			printf("%d ", input_matrix->matrix[i * input_matrix->row + j]);
		}
		printf("\n");
	}
	printf("\n");
#endif

//	lu_decomposition (input_matrix, row, line);

	free_square_matrix(input_matrix->matrix);
	free(input_matrix);

	return 0;
}

