#include "corner.h"


int main (int argc, char **argv) {
	int rank, commsize, len, time;
	int x, t, nx, nt;
	double tau, h;
	double 	**arr;

	len = L;
	time = T;
	nx = NX;
	nt = NT;

	arr = (double **) calloc(NX, sizeof(double *));
	for (int i = 0; i < NT; i ++) {
		arr[i] = (double *) calloc(NT, sizeof(double));
	}

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &commsize);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	calc_info_t info = {
		.arr = arr,
		.len = L,
		.time = T,
		.nx = NX,
		.nt = NT,
		.left = rank * (nt / commsize),
		.right = (rank + 1) * (nt / commsize) - 1,
		.top = NT - 1,
		.h = (double) len / nx,
		.tau = (double) time / nt,
		.rank = rank,
		.commsize = commsize
	};

	// rank = 0;
	// commsize = 1;


	printf("info init\n");

	

	printf("calc start\n");

	Calc(&info);

	Dump("dump.txt", &info);


	for (int i = 0; i < NT; i ++) {
		free(arr[i]);
	}
	free(arr);

	MPI_Finalize();
	return 0;
}


double func (double x, double t) {
	return (double) 0;
}


double func_bottom (double x) {
	return sin(x);
}

double func_left_edge (double t) {
	return (double) 0;
}


void Calc (calc_info_t *info) {
	MPI_Status status;
	printf("left = %d\n", info->left);
	printf("right = %d\n", info->right);

	if (info->rank == 0) {
		for (int t = 0; t < info->top; t ++) {
			info->arr[0][t] = func_left_edge(t);
		}
	}

	for (int x = info->left; x <= info->right; x ++) {
		info->arr[x][0] = func_bottom(x);
	}

	for (int t = 0; t < info->top - 1; t ++) {
		if (info->rank != 0) 
			MPI_Recv(&(info->arr[info->left - 1][t]), 1, MPI_DOUBLE, info->rank - 1, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		if (info->rank < info->commsize - 1)
			MPI_Send(&(info->arr[info->right][t]), 1, MPI_DOUBLE, info->rank + 1, info->rank, MPI_COMM_WORLD);
		if (info->rank == 0) {
			for (int x = info->left + 1; x <= info->right; x ++) {
				info->arr[x][t + 1] = info->tau * func(x, t) + ((double) 1 - info->tau / info->h) * info->arr[x][t] + (info->tau / info->h) * info->arr[x - 1][t];
			}
		} else {
			for (int x = info->left; x <= info->right; x ++) {
				info->arr[x][t + 1] = info->tau * func(x, t) + ((double) 1 - info->tau / info->h) * info->arr[x][t] + (info->tau / info->h) * info->arr[x - 1][t];
			}

		}
	}

	return;
}


void Dump (const char *pathname, calc_info_t *info) {
	printf("dump\n");
	MPI_Status status;
	if (info->rank == 0) {
		FILE *out;
		int left, right;
		for (int i = 1; i < info->commsize; i ++) {
			MPI_Recv(&left, 1, MPI_INT, i, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
			MPI_Recv(&right, 1, MPI_INT, i, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

			for (int x = left; x <= right; x ++) {
				MPI_Recv(info->arr[x], info->top, MPI_DOUBLE, i, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
			}
			info->right =right;
		}

		out = fopen(pathname, "w+");
		for (int t = 0; t < info->top; t ++) {
			for (int x = info->left; x <= info->right; x ++) {
				fprintf(out, "%.3lf  ", info->arr[x][t]);
			}
			fprintf(out, "\n");
		}
		fclose(out);

	} else {
		MPI_Send(&(info->left), 1, MPI_INT, 0, info->rank, MPI_COMM_WORLD);
		MPI_Send(&(info->right), 1, MPI_INT, 0, info->rank, MPI_COMM_WORLD);

		for (int i = info->left; i <= info->right; i ++) {
			MPI_Send(info->arr[i], info->top, MPI_DOUBLE, 0, info->rank, MPI_COMM_WORLD);
		}
	}

	return;
}
