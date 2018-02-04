#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>

#include "utils.h"

/*
	Programma che calcola ricorsivamente il determinanente di una matrice N x N a coefficienti reali
*/

#define MAX_PATH_LENGTH 32

char path[MAX_PATH_LENGTH];

float ** extract_submatrix(float ** matrix, int dim, int pass){
	float ** submatrix = allocate_matrix(dim-1);

	int i, j;
	for (i = 1; i < dim; i++){
		int flag = 0;
		for (j = 0; j < dim; j++){
			if (j == pass){
				flag = 1;
				continue;
			}
			if (flag)
				submatrix[i-1][j-1] = matrix[i][j];
			else 
				submatrix[i-1][j] = matrix[i][j];
		}
	}

	return submatrix;
}

float calculate_determinant_recursive(float ** matrix, int dim){

	if (dim == 1)	// caso base : matrice 1x1
		return matrix[0][0];

	int i;
	float det = 0;
	for (i = 0; i < dim; i++){
		int sign = (i % 2) ? -1 : 1;	// moltiplicatore positivo o negativo in base all'indice della colonna
		int molt = matrix[0][i];

		float ** submatrix = extract_submatrix(matrix, dim, i);
		float sub_det = calculate_determinant_recursive(submatrix,dim-1);
		matrix_dealloc(submatrix, dim-1);

		det += sign * molt * sub_det;
	}

	return det;
}

void print_matrix(float ** matrix, int dim){
	int i, j;
	for (i = 0; i < dim; i++){
		for (j = 0; j < dim; j++){
			printf("%f ", matrix[i][j]);
		}
		printf("\n");
	}
}
	
void matrix_dealloc(float ** matrix, int dim){
	int i;
	for (i = 0; i < dim; i++){
		free(matrix[i]);
	}
	free(matrix);
}

	// Read a matrix dim*dim from descriptor file
void compile_matrix(float ** matrix, int dim, FILE * file) {
	int i, j;

	for (i = 1; i <= dim; i++){
		printf("\t[matrixDet] Insert line #%d: ", i);
		for (j = 0; j < dim; j++){
			fscanf(file, "%f",&matrix[i-1][j]);
		}
	}

	printf("[matrixDet] Matrix successfully read\n\n");
}

float** allocate_matrix(int dim){
	float** matrix = calloc(dim, sizeof(float*));
	int i;
	for (i = 0; i < dim; i++){
		matrix[i] = calloc(dim, sizeof(float));
	}
	return matrix;
}

int get_dimension(FILE * file){
	int dim;
	printf("\n[matrixDet] Insert the dimension of matrix: "); fflush(stdout);

	fscanf(file, "%d", &dim);

	return dim;
}

	// Verify if path is present and return fd (returns -1 if not present a path)
int choose_input(char* path){
	if (strlen(path) == 0){
		printf("[matrixDet] Set the input_strem on STDIN : file_desc = 1 \n");
		return 1;	// STDIN stream chosen
	}

	int fd = open(path, O_RDONLY);
	if (fd == -1)
		print_errno();	

	printf("[matrixDet] Set the input stream on file : file_desc = %d \n", fd);
	return fd;		// path file descriptor
}

int main(int argc, char ** argv){

	printf("\n");
	int ret;

		// Parsing arguments
	if (argc == 2){
		if (strlen(argv[1]) > MAX_PATH_LENGTH)
			print_error("Path is too long !!");
		strncpy(path, argv[1], MAX_PATH_LENGTH);
	}

		// Set the descriptor for (stdin or external file) and create a FILE pointer to file
 	int desc = choose_input(path);
 	FILE * input_file = fdopen(desc, "r");
 	if (input_file == NULL)
 		print_errno();

 		// Read dimension of matrix from desc
 	int dim = get_dimension(input_file);

 		// Allocate a matrix dim*dim
	float ** matrix = allocate_matrix(dim);
	printf("[matrixDet] Matrix %dx%d successfully allocated\n", dim, dim);

		// Read and compile matrix reading it from desc
	compile_matrix(matrix, dim, input_file);

		// Print matrix on stdout
	print_matrix(matrix, dim);

		// Calculate determinant 
	float det = calculate_determinant_recursive(matrix, dim);
	printf("\t--> DETERMINANT = %f\n", det);
	
		// Deallocate matrix
	matrix_dealloc(matrix, dim);

		// Close descriptor and FILE *
	ret = fclose(input_file);
	if (ret == -1)
		print_errno();

	return EXIT_SUCCESS;
}