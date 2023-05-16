#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define NX 20
#define NT 20
#define L 1
#define T 1

// typedef struct {
// 	int size;
// 	int offset;
// 	int *val;
// } arr_t;

typedef struct {
	double 	**arr;												// ????????????????
	int 	len;
	int 	time;
	int 	nx;
	int 	nt;
	int 	left;
	int 	right;
	int		top;
	double	h;
	double	tau;
	int 	rank;
	int 	commsize;
} calc_info_t;

void Calc (calc_info_t *info);
double func (double x, double t);
double func_bottom (double x);
void Dump (const char *pathname, calc_info_t *info);
double func_left_edge (double t);
// arr_t *CreateArr (int left, int right);
