/*
Group participants
1) Aliakbar Taghdiri
2) Bijaya Mandel
3) Morteza Feizbakhsh
4) Priyanka Mukeimpi Santhosh
*/

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define SEED 350
#define MATRIXSIZE 10
#define BLOCKSIZE 5

struct matrix{
	int rows;
	int columns;
	double *data;
};


void init_matrix(struct matrix *a);
void fill_matrix(struct matrix a);
void print_matrix(struct matrix a);

void multiply_matrices(struct matrix a, struct matrix b, struct matrix x);

int main(){
	struct matrix A, B, X;
	init_matrix(&A);
	fill_matrix(A);
	init_matrix(&B);
	fill_matrix(B);
	init_matrix(&X);

	double start = omp_get_wtime();
	multiply_matrices(A, B, X);
	double end = omp_get_wtime();

	printf("This is a matrix multiplication (squential code).\n");
	printf("A=\n");
	print_matrix(A);
	printf("B=\n");
	print_matrix(B);
	printf("AB=\n");
	print_matrix(X);

	printf("\nCalculation time: %f ms\n", (end - start) * 1000);

	free(A.data);
	free(B.data);
	free(X.data);

	return 0;
}

void init_matrix(struct matrix *a){
	a->rows = MATRIXSIZE;
	a->columns = MATRIXSIZE;
	a->data = malloc(MATRIXSIZE * MATRIXSIZE * sizeof(double));
}

void fill_matrix(struct matrix a){
	for (int i = 0; i < a.rows; i++){
		for (int j = 0; j < a.columns; j++){
			*(a.data + i * a.columns + j) = (double)rand() / RAND_MAX;
		}
	}
}

void print_matrix(struct matrix a){
	for (int i = 0; i < a.rows; i++){
		printf("   ");
		for (int j = 0; j < a.columns; j++){
			printf("%.2f ", *(a.data + i * a.columns + j));
		}
		printf("\n");
	}
}

void multiply_matrices(struct matrix a, struct matrix b, struct matrix x){
	double a_block[BLOCKSIZE][BLOCKSIZE],
	       b_block[BLOCKSIZE][BLOCKSIZE],
	       x_block[BLOCKSIZE][BLOCKSIZE];

	for (int i = 0; i < x.rows; i += BLOCKSIZE){
		for (int j = 0; j < x.columns; j += BLOCKSIZE){
			//initilalize x_block
			for (int ib = 0; ib < BLOCKSIZE; ib++){
				for (int jb = 0; jb < BLOCKSIZE; jb++){
					x_block[ib][jb] = 0;
				}
			}
			// The main multiplication loop
			for (int k = 0; k < a.columns; k += BLOCKSIZE){
				//initilaizing a_block and b_block
				for (int ib = 0; ib < BLOCKSIZE; ib++){
					for (int jb = 0; jb < BLOCKSIZE; jb++){
						if (((i+ib) < a.rows) && ((k+jb) < a.columns)){
							a_block[ib][jb] = *(a.data + (i + ib) * a.columns + (k + jb));
						} else {
							a_block[ib][jb] = 0;
						}
						if (((k+ib) < b.rows) && ((j+jb) < b.columns)){
							b_block[ib][jb] = *(b.data + (k + ib) * b.columns + (j + jb));
						} else {
							b_block[ib][jb] = 0;
						}
					}
				}
				//block multipication
				for (int ib = 0; ib < BLOCKSIZE; ib++) {
					for (int jb = 0; jb < BLOCKSIZE; jb++) {
						for (int kb = 0; kb < BLOCKSIZE; kb++) {
							x_block[ib][jb] += a_block[ib][kb] * b_block[kb][jb];
						}
					}
				}
			}
			// copying the x_block into x
			for (int ib = 0; ib < BLOCKSIZE && (i+ib) < x.rows; ib++){
				for (int jb = 0; jb < BLOCKSIZE && (j+jb) < x.columns; jb++){
					*(x.data + (i + ib) * x.columns + (j + jb)) = x_block[ib][jb];
				}
			}
		}
	}
}
