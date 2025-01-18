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

#define SEED 1200
#define SIZE 1000

struct matrix{
	int rows;
	int columns;
	double *data;
};

enum boolean {NO, YES};

void init_matrix(struct matrix *a, int rows, int columns);
void fill_matrix(struct matrix a);

int multiply_two_matrices(struct matrix a, struct matrix b, struct matrix ab, enum boolean parallel);
int multiply_three_matrices(struct matrix a, struct matrix b, struct matrix c, struct matrix x, enum boolean parallel);

int main(){
	struct matrix A, B, C, X;
	init_matrix(&A, SIZE, SIZE);
	fill_matrix(A);
	init_matrix(&B, SIZE, SIZE);
	fill_matrix(B);
	init_matrix(&C, SIZE, SIZE);
	fill_matrix(C);
	init_matrix(&X, SIZE, SIZE);
	// sequential mutiplication
	double start = omp_get_wtime();
	multiply_three_matrices(A, B, C, X, NO);
	double sequential_time = omp_get_wtime() - start;
	// parallel multiplication
	start = omp_get_wtime();
	int thread_count = multiply_three_matrices(A, B, C, X, YES);
	double parallel_time = omp_get_wtime() - start;

	printf("This is a matrix multiplication of %d by %d matrices\n", SIZE, SIZE);
	printf("CPU time (sequential code): %.2f seconds\n", sequential_time);
	printf("CPU time (parallel code with %d threads): %.2f seconds\n", thread_count, parallel_time);
	printf("\nThe parallel program is %.1f-times faster than the sequential code.\n", sequential_time / parallel_time);

	free(A.data);
	free(B.data);
	free(C.data);
	free(X.data);

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

int multiply_two_matrices(struct matrix a, struct matrix b, struct matrix ab, enum boolean parallel){
	int thread_count = 1;
	#pragma omp parallel if (parallel)
	{
		thread_count = omp_get_num_threads();
		#pragma omp for collapse(2)
		for (int i = 0; i < ab.rows; i++){
			for (int j = 0; j < ab.columns; j++){
				double ab_ij = 0;
				for (int k = 0; k < a.columns; k++){
					ab_ij += *(a.data + i * a.columns + k) * *(b.data + k * b.columns + j);
				}
				*(ab.data + i * ab.columns + j) = ab_ij;
			}
		}
	}
	return thread_count;
}

int multiply_three_matrices(struct matrix a, struct matrix b, struct matrix c, struct matrix x, enum boolean parallel){
	struct matrix ab;
	init_matrix(&ab, a.rows, b.columns);
	int thread_count = multiply_two_matrices(a, b, ab, parallel);
	multiply_two_matrices(ab, c, x, parallel);
	free(ab.data);
	return thread_count;
}
