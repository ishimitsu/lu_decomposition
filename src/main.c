#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT_MATRIX "input_matrix.txt"
#define BUF_LEN 1024

extern int *init_matrix(int row, int line);
extern void free_square_matrix(int *matrix);
extern int get_matrix_elements(int *matrix, char *input, int now_line, int max_row);

int main(void) {
	char input[BUF_LEN];
	int *input_matrix;
	int now_input_line = 0;
	int now_matrix_line = 0;
	int row, line;
	FILE *fp;

	if ( (fp = fopen(INPUT_MATRIX, "r") ) == NULL ) {
		printf("Error: Open %s\n", INPUT_MATRIX);
		return -1;
	}
	
	while( fgets(input, sizeof(input), fp) != NULL) {

		if(now_input_line == 0) {
			line = atoi(input);

		} else if(now_input_line == 1) {
			row = atoi(input);

			// initialize square matrix
			printf("matrix size, line %d row %d\n", line, row);
			input_matrix = init_matrix(row, line);

		} else {

			if(now_matrix_line < line) {
				printf("Line %d : %s", now_matrix_line, input);

				if( !(get_matrix_elements(input_matrix, input, now_matrix_line, row)) ) {
					printf("Invalid Matrix\n");
					return -1;
				}
				
			} else {
				printf("Max Matrix Line is %d, so line %d is ignored\n",
				       line, now_matrix_line);
			}
			
			now_matrix_line++;

		}

		now_input_line++;
	}

	printf("Input Matrix Element : %d %d\n", row, line);
	for(int i = 0; i < row; i++) {
		for(int j = 0; j < line; j++) {
			printf("%d ", input_matrix[i * row + j]);
		}
		printf("\n");
	}
	printf("\n");


	fclose(fp);
	free_square_matrix(input_matrix);

	return 0;
}

