#include "corner.h"


int main (int argc, char **argv) {
	int rank, commsize, len, time;
	int x, t, nx, nt;
	double tau, h;
	calc_info_t info;

	// rank = 0;
	// commsize = 1;

	// MPI_Init(&argc, &argv);
	// MPI_Comm_size(MPI_COMM_WORLD, &commsize);
	// MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	printf("info init\n");

	// info = {
	// 	.len = L,
	// 	.time = T,
	// 	.nx = NX,
	// 	.nt = NT,
	// 	.left = rank * (nt / commsize),
	// 	.right = (rank + 1) * (nt / commsize) - 1,
	// 	.top = time - 1,
	// 	.h = (double) len / nx,
	// 	.tau = (double) time / nt,
	// 	.rank = rank,
	// 	.commsize = commsize
	// };

	printf("calc start\n");

	// Calc(&info);

	printf("dump start\n");

	// Dump("dump.txt", &info);

	// MPI_Finalize();
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
	// MPI_Status status;
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
			// MPI_Recv(&(info->arr[info->left - 1][t]), 1, MPI_DOUBLE, info->rank - 1, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		if (info->rank < info->commsize - 1)
			// MPI_Send(&(info->arr[info->right][t]), 1, MPI_DOUBLE, info->rank + 1, info->rank, MPI_COMM_WORLD);

		for (int x = info->left; x <= info->right; x ++) {
			info->arr[x][t + 1] = info->tau * func(x, t) + ((double) 1 - info->tau / info->h) * info->arr[x][t] + (info->tau / info->h) * info->arr[x - 1][t];
		}
	}

	return;
}


void Dump (const char *pathname, calc_info_t *info) {
	// MPI_Status status;

	if (info->rank == 0) {
		FILE *out;
		for (int i = 1; i < info->commsize; i ++) {
			calc_info_t recv_info;
			// MPI_Recv(&recv_info, 1, MPI_INT, i, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
			for (int x = recv_info.left; x <= recv_info.right; x ++) {
				for (int t = 0; t < recv_info.top; t ++) {
					info->arr[x][t] = recv_info.arr[x][t];
				}
			}
			info->right = recv_info.right;
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
		// MPI_Send(info, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD);
	}

	return;
}
