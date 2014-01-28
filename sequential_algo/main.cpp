#include <stdio.h>
#include <string.h>
#include <stdlib.h>
template <typename T>
T _min(T a, T b) {
	if (a + eps < b)
		return a;
	return b;
}
char in[100], out[100];
int n;
double INF, **matrix, eps = 1e-7;
int main(int argc, char **argv) {
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
	matrix = (double **)malloc(n * sizeof(double *));
	for (int i = 0; i < n; ++i)
		matrix[i] = (double *)malloc(n * sizeof(double));
	for (int i = 0; i < n; ++i)
		for (int j = 0; j < n; ++j)
			scanf("%lf", &matrix[i][j]);
	freopen(out, "wt", stdout);
	for (int k = 0; k < n; ++k)
		for (int i = 0; i < n; ++i)
			for (int j = 0; j < n; ++j)
				matrix[i][j] = _min(matrix[i][j], matrix[i][k] + matrix[k][j]);
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j)
			if (matrix[i][j] == INF)
				printf("--- ");
			else
				printf("%.9lf ", matrix[i][j]);
		putchar('\n');
	}
	return 0;
}