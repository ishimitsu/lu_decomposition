#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DBG_PRINT 1

extern void print_matrix(int *matrix, int matrix_length);

#if DBG_PRINT == 1
#define DBG_PRINT_INPUT_MATRIX 1
#define DBG_PRINT_LU_MATRIX 1
#define DBG_PRINT_LU_INV_MATRIX 1
#endif


#define BUF_LEN 1024
