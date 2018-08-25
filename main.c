#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT_MATRIX "input_matrix.txt"

int split_string_to_int(char *target, char *split, int *ret) {
	char *p;
	int i = 1;

	p = strtok(target, split);
	ret[0] = atoi(p);

	while( p != NULL) {
		p = strtok(NULL, split);

		if(p != NULL) {
			ret[i] = atoi(p);
			i++;
		}
	}

	return 0;
}

int main(void) {
	char input[1000];

	FILE *fp;
	if ( (fp = fopen(INPUT_MATRIX, "r") ) == NULL ) {
		printf("Error: Open %s\n", INPUT_MATRIX);
		return -1;
	}
	
	while( fgets(input, sizeof(input), fp) != NULL) {
		printf("%s\n", input);
	}
	fclose(fp);

	return 0;
}

