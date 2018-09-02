#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUF_LEN 1024

#define DBG_PRINT 1
#if DBG_PRINT == 1
#define DBG_PRINT_INPUT_MATRIX 1
#define DBG_PRINT_MATRIX_PVT_SELECT 0
#define DBG_PRINT_LU_MATRIX 0
#define DBG_PRINT_LU_INV_MATRIX 0
#endif

#define CHECK_LU_DECOMP_RESULT 1
#define CHECK_LU_INV_RESULT 1
#define CHECK_A_INV_RESULT 1
