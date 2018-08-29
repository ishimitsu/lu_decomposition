#include "common.h"
#include <ctype.h>

int *malloc_matrix_list[64] ={0};
int malloc_matrix_cnt = 0;

int *malloc_square_matrix(int length) {
  int *matrix;
  matrix = malloc ( sizeof(int *) * length * length);
  memset (matrix, 0, sizeof(int *) *  length * length);

  malloc_matrix_list[malloc_matrix_cnt] = matrix;
  malloc_matrix_cnt++;

  return matrix;
}

void free_all_malloc_matrix (void) {
  for(int i = 0; i < malloc_matrix_cnt; i++) {
    free (malloc_matrix_list[i]);
  }
}

int get_input_matrix_length (char *filename) {
  FILE *fp;
  int matrix_length = 0;
  char buf[BUF_LEN];

  if ( (fp = fopen(filename, "r") ) == NULL ) {
    printf("Error: Open %s\n", filename);
    return 0;
  }
  
  fgets(buf, sizeof(buf), fp);

  for (int i = 0; i < strlen(buf) && buf[i] != '\0' && buf[i] != '\n'; i++) {
    if(!isdigit(buf[i])) {
      return 0;
    }
  }

  matrix_length = atoi(buf);

  fclose(fp);

  return matrix_length;
}


int split_string_to_int(char *target, char *split, int *ret, int ret_size) {
  char *p;
  int i = 1;

  p = strtok(target, split);

  for (int i = 0; i < strlen(p) && p[i] != '\0' && p[i] != '\n'; i++) {
    if(!isdigit(p[i])) {
      printf("Invalid Element %s\n", p);
      return 0;
    }
  }

  ret[0] = atoi(p);

  while( p != NULL) {
    p = strtok(NULL, split);
    
    if(p != NULL) {

      for (int i = 0; i < strlen(p) && p[i] != '\0' && p[i] != '\n'; i++) {
	if(!isdigit(p[i])) {
	  printf("Invalid Element %s\n", p);
	  return 0;
	}
      }

      if(i > ret_size) {
	printf("Too Many Element\n");
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
    printf("Invalid Matrix Element %s\n", input);
    return 0;
  }

  for(int i = 0; i < matrix_length; i++) {
    matrix[now_line * matrix_length + i] = int_array[i];
  }

  free(int_array);
	
  return 1;
}


int get_input_square_matrix (char *filename, int *matrix_a, int matrix_length) {
  FILE *fp;
  char buf[BUF_LEN];
  int now_line = 0;
  int now_matrix_line = 0;

  if ( (fp = fopen(filename, "r") ) == NULL ) {
    printf("Error: Open %s\n", filename);
    return 0;
  }

  while( fgets(buf, sizeof(buf), fp) != NULL) {

    if(now_line > 0) { 
      // Add elements to square matrix
      if(now_matrix_line < matrix_length) {

	if( !(get_matrix_elements(matrix_a, buf, now_matrix_line, matrix_length)) ) {
	  printf("Invalid Matrix\n");
	  return 0;
	}
				
      } else {
	printf("Matrix Length is %d, so line %d [%s] is ignored\n",
	       matrix_length, now_matrix_line, buf);
      }
      now_matrix_line++;

    }

    now_line++;
  }

  fclose(fp);

  return 1;
}

// Check M1 * M2 = Comp_M or not
int compare_matrix_multi (int *matrix_1, int *matrix_2, 
			  int *comp_matrix, int matrix_length) {
  int sum = 0;

  for(int i = 0; i < matrix_length; i++) {
    for(int j = 0; j < matrix_length; j++) {
      sum = 0;

      for(int k = 0; k < matrix_length; k++)
	sum += matrix_1[i * matrix_length + k] * matrix_2[k * matrix_length + j];

      if(sum != comp_matrix[i * matrix_length + j]) {
	return 0;
      }

    }
  }

  return 1;
}
