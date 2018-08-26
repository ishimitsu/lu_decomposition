#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int *init_square_matrix(int length) {
	int *matrix;
	matrix = malloc ( sizeof(int *) * length * length);
	memset (matrix, 0, sizeof(int *) *  length * length);

	return matrix;
}

void free_square_matrix(int *matrix) {
	free(matrix);
}

int split_string_to_int(char *target, char *split, int *ret, int ret_size) {
	char *p;
	int i = 1;

	p = strtok(target, split);
	ret[0] = atoi(p);

	while( p != NULL) {
		p = strtok(NULL, split);

		if(p != NULL) {

			if(i > ret_size) {
				return 0;
			}

			ret[i] = atoi(p);
			i++;
		}
	}

	return 1;
}

int get_matrix_elements(int *matrix, char *input, int now_line, int matrix_length) {
	int *int_array;

	int_array = malloc(sizeof(int) * matrix_length);
	memset(int_array, 0, sizeof(int) * matrix_length);

	if ( !(split_string_to_int (input, " ", int_array, matrix_length)) ) {
		printf("Max Matrix Length is %d, this Matrix[%s] Length is over\n", matrix_length, input);
		return 0;
	}

	for(int i = 0; i < matrix_length; i++) {
		matrix[now_line * matrix_length + i] = int_array[i];
	}

	free(int_array);
	
	return 1;
}
