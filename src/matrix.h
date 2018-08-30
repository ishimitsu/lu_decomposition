extern double *malloc_square_matrix(int length);
extern void free_all_malloc_matrix(void);
extern int get_input_matrix_length (char *filename);
extern int get_input_square_matrix (char *filename, double *matrix_a, int matrix_length);

extern int compare_matrix_multi (double *matrix_1, double *matrix_2, double *comp_matrix, int matrix_length);
extern void print_matrix(double *matrix, int matrix_length);
