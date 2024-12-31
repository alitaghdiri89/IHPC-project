/*
Group participants
1) Aliakbar Taghdiri
2) Bijaya Mandel
3) Morteya Feizbakhsh
4) Priyanka Mukeimpi Santhosh
*/

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define SEED 350
#define M 5
#define N 10000
#define P 1000
#define BLOCKSIZE 5

struct matrix{
	int rows;
	int columns;
	double *data;
};

void init_matrix(struct matrix *a, int rows, int columns);
void fill_matrix(struct matrix a);

void multiply_matrices(struct matrix a, struct matrix b, struct matrix c);
void multiply_single_blocks(double a_block[BLOCKSIZE][BLOCKSIZE],
			    double b_block[BLOCKSIZE][BLOCKSIZE],
			    double x_block[BLOCKSIZE][BLOCKSIZE]);

int main(){
	struct matrix A, B, X;
	init_matrix(&A, N, M);
	fill_matrix(A);
	init_matrix(&B, M, P);
	fill_matrix(B);
	init_matrix(&X, N, P);

	double start = omp_get_wtime();
	multiply_matrices(A, B, X);
	double end = omp_get_wtime();

	printf("\nCalculation time: %f ms\n", (end - start) * 1000);

	return 0;
}

void init_matrix(struct matrix *a, int rows, int columns){
	a->rows = rows;
	a->columns = columns;
	a->data = malloc(rows * columns * sizeof(double));
}

void fill_matrix(struct matrix a){
	for (int i = 0; i < a.rows; i++){
		for (int j = 0; j < a.columns; j++){
			*(a.data + i * a.columns + j) = (double)rand() / RAND_MAX;
		}
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
						a_block[ib][jb] = *(a.data + (i + ib) * a.columns + (k + jb));
						b_block[ib][jb] = *(b.data + (k + ib) * b.columns + (j + jb));
					}
				}
				multiply_single_blocks(a_block, b_block, x_block);
			}
			// copying the x_block into x
			for (int ib = 0; ib < BLOCKSIZE; ib++){
				for (int jb = 0; jb < BLOCKSIZE; jb++){
					*(x.data + (i + ib) * x.columns + (j + jb)) = x_block[ib][jb];
				}
			}
		}
	}
}

void multiply_single_blocks(double a_block[BLOCKSIZE][BLOCKSIZE],
			    double b_block[BLOCKSIZE][BLOCKSIZE],
			    double x_block[BLOCKSIZE][BLOCKSIZE]){
	for (int i = 0; i < BLOCKSIZE; i++){
		for (int j = 0; j < BLOCKSIZE; j++){
			for (int k = 0; k < BLOCKSIZE; k++){
				x_block[i][j] += a_block[i][k] * b_block[k][j];
			}
		}
	}
}
