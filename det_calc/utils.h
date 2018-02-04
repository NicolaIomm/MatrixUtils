#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>

#define BUFFER_LENGTH 1024

float ** extract_submatrix(float ** martix, int dim, int pass);
float calculate_determinant_recursive(float ** matrix, int dim);
void print_matrix(float ** matrix, int dim);
void matrix_dealloc(float ** matrix, int dim);
void compile_matrix(float ** matrix, int dim, FILE * file);
float** allocate_matrix(int dim);
int get_dimension(FILE * file);
int choose_input(char* path);

void print_error(char* msg){
	fprintf(stderr, "\nERRORE: %s\n\n",msg);
	exit(EXIT_FAILURE);
}

void print_errno(){
	print_error(strerror(errno));
}
	
	// Perform a readline() by reading 1 byte at times, until '\n' occurs
int readline(int desc, char* buffer){
	int ret, read_bytes = 0;

	while (1){
		ret = read(desc, buffer+read_bytes, 1);
		if (ret == -1 && errno == EINTR) continue;
		read_bytes++;
		if (buffer[read_bytes-1] == '\0' || buffer[read_bytes-1] == '\n') break;
	}

	buffer[read_bytes-1] = '\0';

	return read_bytes-1;
}

float * parseline(int dim, char* line){

	float ** matrix_line = malloc(dim * sizeof(float));

	int i;
	for (i = 0; i < dim; i++){
		scanf("%f", matrix_line[i]);
	}

	return *matrix_line;
}
