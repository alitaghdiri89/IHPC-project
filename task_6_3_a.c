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
#define M 4
#define N 5
#define P 6

struct matrix{
	int rows;
	int columns;
	double *data;
};

void init_matrix(struct matrix *a, int rows, int columns);
void fill_matrix(struct matrix a);
void print_matrix(struct matrix a);

void multiply_two_matrices(struct matrix a, struct matrix b, struct matrix ab);
void multiply_three_matrices(struct matrix a, struct matrix b, struct matrix c, struct matrix x);

int main(){
	struct matrix A, B, C, X;
	init_matrix(&A, N, M);
	fill_matrix(A);
	init_matrix(&B, M, P);
	fill_matrix(B);
	init_matrix(&C, P, M);
	fill_matrix(C);
	init_matrix(&X, N, M);
	
	double start = omp_get_wtime();
	multiply_three_matrices(A, B, C, X);
	double end = omp_get_wtime();

	printf("This is a triple matrix multiplication (sequential code).\n");
	printf("A=\n");
	print_matrix(A);
	printf("B=\n");
	print_matrix(B);
	printf("C=\n");
	print_matrix(C);
	printf("ABC=\n");
	print_matrix(X);
	printf("\nCalculation time: %f ms\n", (end - start) * 1000);

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

void print_matrix(struct matrix a){
	for (int i = 0; i < a.rows; i++){
		printf("    ");
		for (int j = 0; j < a.columns; j++){
			printf("%.2f ", *(a.data + i * a.columns + j));
		}
		printf("\n");
	}
}

void multiply_two_matrices(struct matrix a, struct matrix b, struct matrix ab){
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

void multiply_three_matrices(struct matrix a, struct matrix b, struct matrix c, struct matrix x){
	struct matrix ab;
	init_matrix(&ab, a.rows, b.columns);
	multiply_two_matrices(a, b, ab);
	multiply_two_matrices(ab, c, x);
}