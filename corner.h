// #include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define NX 2000
#define NT 2000
#define L 1
#define T 1000

// typedef struct {
// 	int size;
// 	int offset;
// 	int *val;
// } arr_t;

typedef struct {
	double 	arr[NX][NT];												// ????????????????
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
