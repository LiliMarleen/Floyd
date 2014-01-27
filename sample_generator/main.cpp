#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
int n = 1000, absent_prob_nom = 1, absent_prob_denom = 2;
bool directed = false;
double maxw = 1e2, INF = 1e9, **matrix = NULL;
char out[100];
double gen_rand_double() {
	double w;
	if (rand() % absent_prob_denom < absent_prob_nom)
		return INF;
	else {
		w = rand() / 0x7fffffff;
		w += rand() % int(maxw);
		return w;
	}
}
int main(int argc, char **argv) {
	srand(time(NULL));
	if (argc != 9) {
		fputs("Wrong number of args\n", stderr);
		return 0;
	}
	for (int i = 1; i < 9; i += 2) {
		if (!strcmp("--n", argv[i]))
			n = atoi(argv[i + 1]);
		else if (!strcmp("--d", argv[i]))
			directed = !atoi(argv[i + 1]) ? false : true;
		else if (!strcmp("--maxw", argv[i]))
			maxw = atof(argv[i + 1]);
		else if (!strcmp("--output", argv[i]))
			strcpy(out, argv[i + 1]);
	}
	if (!directed) {
		matrix = (double **)malloc(n * sizeof(double *));
		for (int i = 0; i < n; ++i) {
			matrix[i] = (double *)malloc(n * sizeof(double));
			memset(matrix[i], 0, n * sizeof(double));
		}
	}
	freopen(out, "wt", stdout);
	printf("%d %lf\n", n, INF);
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j)
			if (directed) {
				if (i == j)
					printf("0 ");
				else
					printf("%lf ", gen_rand_double());
			} else if (!directed && i < j) {
				matrix[i][j] = matrix[j][i] = gen_rand_double();
			}
		if (directed)
			putchar('\n');
	}
	if (!directed)
		for (int i = 0; i < n; ++i) {
			for (int j = 0; j < n; ++j)
				printf("%lf ", matrix[i][j]);
			putchar('\n');
		}
	return 0;
}