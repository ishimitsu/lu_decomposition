#include "common.h"
#include "matrix.h"

double *malloc_matrix_list[64] ={0};
int malloc_matrix_cnt = 0;

double *malloc_square_matrix(int length) {
  double *matrix;

  matrix = calloc ( length * length, sizeof(double) );

  malloc_matrix_list[malloc_matrix_cnt] = matrix;
  malloc_matrix_cnt++;

  return matrix;
}

void free_all_malloc_matrix (void) {

  if(malloc_matrix_cnt > 0) {

    for(int i = 0; i < malloc_matrix_cnt; i++) {
      free (malloc_matrix_list[i]);
    }

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
      printf("Invalid Matrix Length [%s]\n", buf);
      return 0;
    }
  }

  matrix_length = atoi(buf);

  fclose(fp);

  return matrix_length;
}


// return element count in line
int get_matrix_element_cnt(char *line, char *split) {
  char *p;
  int ret = 0;
  char *target;

  target = malloc (sizeof(char) * strlen(line));
  strncpy (target, line, strlen(line));

  if ( (p = strtok(target, split) ) != NULL)
    ret++;

  while( p != NULL) {
    if ( (p = strtok(NULL, split) ) != NULL)
      ret++;
  }

  free(target);

  return ret;
}


int get_matrix_element_from_line(char *line, char *split, double *elements, int element_cnt) {
  int ret = 1;
  char *p;
  int cnt, decimal_point;
  char *target;

  target = malloc (sizeof(char) * strlen(line));
  strncpy (target, line, strlen(line));

  p = strtok(target, split);
  decimal_point = 0;
  for (int i = 0; i < strlen(p) && p[i] != '\0' && p[i] != '\n'; i++) {

    if( i == 0 && p[i] == '-') {
    } else if(p[i] == '.' && !decimal_point) {
      decimal_point++;
    } else {
      if(!isdigit(p[i])) {
	ret = 0;
	printf("Invalid Element %s\n", p);
	goto end;
      }
    }
    
  }
  elements[0] = atof(p);

  cnt = 1;
  while( p != NULL) {
    p = strtok(NULL, split);
    
    if(p != NULL) {

      decimal_point = 0;
      for (int i = 0; i < strlen(p) && p[i] != '\0' && p[i] != '\n'; i++) {

	if( i == 0 && p[i] == '-') {
	} else if(p[i] == '.' && !decimal_point) {
	  decimal_point++;
	} else {
	  if(!isdigit(p[i])) {
	    ret = 0;
	    printf("Invalid Element %s\n", p);
	    goto end;
	  }
	}
	
      }

      if(cnt > element_cnt) {
	printf("Too Many Element\n");
	ret = 0;
	goto end;
      }

      elements[cnt] = atof(p);
      cnt++;
    }
  }

 end: 
  free(target);

  return ret;
}

int get_matrix_elements(double *matrix, char *input, int now_line, int matrix_length) {
  double *elements;
  int element_cnt;

  if( (element_cnt = get_matrix_element_cnt (input, " ")) != matrix_length) {
    printf("Matrix Line-%d [%s] total elements=[%d] is Invalid\n", now_line, input, element_cnt);
    return 0;
  }

  elements = calloc(matrix_length, sizeof(double));

  if ( !(get_matrix_element_from_line (input, " ", elements, matrix_length)) ) {
    printf("Matrix Line-%d [%s] includes Invalid elements\n", now_line, input);
    free(elements);
    return 0;
  }

  for(int i = 0; i < matrix_length; i++) {
    matrix[now_line * matrix_length + i] = elements[i];
  }

  free(elements);
	
  return 1;
}


int get_input_square_matrix_element (char *filename, double *matrix_a, int matrix_length) {
  FILE *fp;
  char buf[BUF_LEN];
  int now_line = 0;
  int now_matrix_line = 0;
  int ret = 1;

  if ( (fp = fopen(filename, "r") ) == NULL ) {
    printf("Error: Open %s\n", filename);
    return 0;
  }

  while( fgets(buf, sizeof(buf), fp) != NULL) {

    if(now_line > 0) { 
      // skip line0, it checked at get_input_matrix_length()

      if(now_matrix_line < matrix_length) {

	if( !(get_matrix_elements(matrix_a, buf, now_matrix_line, matrix_length)) ) {
	  return 0;
	}
				
      } else {
	// Too many rows, Invalid Matrix 
	break;
      }
      now_matrix_line++;

    }

    now_line++;
  }

  if(now_matrix_line != matrix_length) {
    printf("Number of Matrix Line = [%d] is Invalid, needs [%d] Line\n ", now_matrix_line, matrix_length);
    ret = 0;
  }

  fclose(fp);

  return ret;
}


// For pivot selection, make permulation matrix P
int pivot_selection (double *matrix_a, double *matrix_p, int matrix_length) {
  double max;
  double *temp;
  int swap;

  temp = calloc(matrix_length, sizeof(double));
  for(int i = 0; i < matrix_length; i++) {
    matrix_p[i * matrix_length + i] = 1;
  }

  for(int i = 0; i < matrix_length; i++) {

    // Check pivot selection need or not
    max = matrix_a[i * matrix_length + i];
    swap = i;
    for(int j = i; j < matrix_length; j++) {
      if(fabs(matrix_a[j * matrix_length + i] - max) > 0.0L) {
	max = matrix_a[j * matrix_length + i];
	swap = j;
      }
    }

    if(fabs(max) < EPSILON) {
      printf("Max pivot [%f], Can't select pivot\n", max);
      return 0;
    }

    if(swap != i) {
      printf("Change %d <-> %d, max %f\n", i, swap, max);
      memcpy(temp, &matrix_a[i * matrix_length], matrix_length * sizeof(double));
      memcpy(&matrix_a[i * matrix_length], &matrix_a[swap * matrix_length], matrix_length * sizeof(double));
      memcpy(&matrix_a[swap * matrix_length], temp, matrix_length * sizeof(double));

      memcpy(temp, &matrix_p[i * matrix_length], matrix_length * sizeof(double));
      memcpy(&matrix_p[i * matrix_length], &matrix_p[swap * matrix_length], matrix_length * sizeof(double));
      memcpy(&matrix_p[swap * matrix_length], temp, matrix_length * sizeof(double));
    }
    
  }

  free(temp);

  return 1;
}



// Check M1 * M2 = Comp_M or not
int compare_matrix_multi (double *matrix_1, double *matrix_2, double *comp_matrix, int matrix_length) {
  double sum = 0;

  for(int i = 0; i < matrix_length; i++) {
    for(int j = 0; j < matrix_length; j++) {
      sum = 0;

      for(int k = 0; k < matrix_length; k++)
	sum += matrix_1[i * matrix_length + k] * matrix_2[k * matrix_length + j];

      if ( fabs(sum - comp_matrix[i * matrix_length + j]) > EPSILON) {
	return 0;
      }

    }
  }

  return 1;
}


void print_matrix(double *matrix, int matrix_length) {
  double element, integer, fraction;

  for(int i = 0; i < matrix_length; i++) {
    for(int j = 0; j < matrix_length; j++) {
      element = matrix[i * matrix_length + j];
      fraction = modf (element, &integer);
      if(fraction == 0)
	printf("%.0f ", element);
      else
	printf("%.4f ", element);
      
    }
    printf("\n");
  }
  printf("\n");

}
