#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <mpi.h>
#pragma comment(lib, "mpi.lib")
#pragma comment(lib, "cxx.lib")
template <typename T>
T _min(T a, T b) {
	if (a + eps < b)
		return a;
	return b;
}
char in[100], out[100];
int n, p, id, start_row, num_row, k, t;
double INF, **matrix, *buf, eps = 1e-7;
MPI_Status status;
int main(int argc, char **argv) {
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &p);
	MPI_Comm_rank(MPI_COMM_WORLD, &id);
	if (!id) {
		if (argc != 5) {
			fputs("Wrong number of args", stderr);
			return 1;
		}
		for (int i = 1; i < 5; i += 2) {
			if (!strcmp("--input", argv[i]))
				strcpy(in, argv[i + 1]);
			else if (!strcmp("--output", argv[i]))
				strcpy(out, argv[i + 1]);
		}
		freopen(in, "rt", stdin);
		scanf("%d%lf", &n, &INF);
		freopen(out, "wt", stdout);
	}
	MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
	num_row = (n + p - 1) / p;
	start_row = id;
	matrix = (double **)malloc(num_row * sizeof(double *));
	for (int i = 0; i < num_row; ++i)
		matrix[i] = (double *)malloc(n * sizeof(double));
	buf = (double *)malloc(n * sizeof(double));
	MPI_Barrier(MPI_COMM_WORLD);
	if (!id) {
		for (int i = 0; i < n; ++i) {
			for (int j = 0; j < n; ++j)
				scanf("%lf", &buf[j]);
			if (i % p)
				MPI_Send(buf, n, MPI_DOUBLE, i % p, 0, MPI_COMM_WORLD);
			else
				memcpy(matrix[t++], buf, n * sizeof(double));
		}
	} else {
		for (int i = 0; i < (n / p) + (id < (n % p) ? 1 : 0); ++i) {
			MPI_Recv(buf, n, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);
			memcpy(matrix[t++], buf, n * sizeof(double));
		}
	}
	MPI_Barrier(MPI_COMM_WORLD);
	for (k = 0; k < n; ++k) {
		if (k % p == id)
			memcpy(buf, matrix[(k - id) / p], n * sizeof(double));
		MPI_Bcast(buf, n, MPI_DOUBLE, k % p, MPI_COMM_WORLD);
		for (int i = 0; i < t; ++i)
			for (int j = 0; j < n ; ++j)
				matrix[i][j] = _min(matrix[i][j], matrix[i][k] + buf[j]);
		MPI_Barrier(MPI_COMM_WORLD);
	}
	for (int i = 0; i < n; ++i) {
		if (!id) {
			if (i % p) {
				MPI_Recv(buf, n, MPI_DOUBLE, i % p, 0, MPI_COMM_WORLD, &status);
				for (int j = 0; j < n; ++j)
					printf("%.9lf ", buf[j]);
			} else
				for (int j = 0; j < n; ++j)
					printf("%.9lf ", matrix[i / p][j]);
			putchar('\n');
		} else {
			if (i % p == id)
				MPI_Send(matrix[(i - id) / p], n, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
		}
		MPI_Barrier(MPI_COMM_WORLD);
	}
	/*printf("Process %d - %d rows\n", id, t);
	for (int i = 0; i < t; ++i) {
		for (int j = 0; j < n ; ++j)
			printf("%lf ", matrix[i][j]);
		putchar('\n');
	}*/
	/*if (!id) {
		for (int i = 0; i < n; ++i) {
			for (int j = 0; j < n; ++j)
				if (matrix[i][j] == INF)
					printf("--- ");
				else
					printf("%.9lf ", matrix[i][j]);
			putchar('\n');
		}
	}*/
	MPI_Finalize();
	return 0;
}