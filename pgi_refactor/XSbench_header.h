#ifndef __XSBENCH_HEADER_H__
#define __XSBENCH_HEADER_H__

#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include<strings.h>
#include<math.h>
#include<omp.h>
#include<unistd.h>
#include<sys/time.h>
#include"occa_c.h"

// Papi Header
#ifdef PAPI
#include "papi.h"
#endif

// I/O Specifiers
#define INFO 1
#define DEBUG 1
#define SAVE 1

// Data type for RNG; necessary because NUKMOD is unsupported in PGI kernels
#ifdef ACC
#define RNG_INT unsigned
#else
#define RNG_INT unsigned long
#endif

// Structures
typedef struct{
	double energy;
	double total_xs;
	double elastic_xs;
	double absorbtion_xs;
	double fission_xs;
	double nu_fission_xs;
} NuclideGridPoint;

typedef struct{
	double energy;
	int xs_ptrs;
} GridPoint;

typedef struct{
	int nthreads;
	long n_isotopes;
	long n_gridpoints;
	int lookups;
	char * HM;
} Inputs;

// Function Prototypes
void logo(int version);
void center_print(const char *s, int width);
void border_print(void);
void fancy_int(long a);

NuclideGridPoint ** gpmatrix(size_t m, size_t n);

void gpmatrix_free( NuclideGridPoint ** M );

int NGP_compare( const void * a, const void * b );

void generate_grids( long n_isotopes, long n_gridpoints, 
    NuclideGridPoint (*nuclide_grids)[n_gridpoints]);

void generate_grids_v( long n_isotopes, long n_gridpoints, 
    NuclideGridPoint (*nuclide_grids)[n_gridpoints]);

void sort_nuclide_grids( long n_isotopes, long n_gridpoints, 
    NuclideGridPoint (*nuclide_grids)[n_gridpoints]);

int * generate_ptr_grid(int n_isotopes, int n_gridpoints);

GridPoint * generate_energy_grid( long n_isotopes, long n_gridpoints, 
    NuclideGridPoint (*nuclide_grids)[n_gridpoints], int * grid_ptrs);

void set_grid_ptrs( GridPoint * energy_grid, int * grid_ptrs, long n_isotopes,
    long n_gridpoints, NuclideGridPoint (*nuclide_grids)[n_gridpoints]);

int binary_search( NuclideGridPoint * A, double quarry, int n );

void calculate_macro_xs( double p_energy, int mat, long n_isotopes,
    long n_gridpoints, int * restrict num_nucs,
    double * restrict concs,
    GridPoint * restrict energy_grid,
    int * restrict grid_ptrs,
    NuclideGridPoint (* restrict nuclide_grids)[n_gridpoints],
    int * restrict mats, int * restrict mats_idx,
    double * restrict macro_xs_vector );

void calculate_micro_xs(   double p_energy, int nuc, long n_isotopes,
    long n_gridpoints,
    GridPoint * restrict energy_grid,
    int * restrict grid_ptrs,
    NuclideGridPoint (* restrict nuclide_grids)[n_gridpoints],
    int idx, double * restrict xs_vector );

#pragma acc routine seq
long grid_search( long n, double quarry, GridPoint * A);

int * load_num_nucs(long n_isotopes);
int * load_mats_idx(int * num_nucs);
int * load_mats( int * num_nucs, int * mats_idx, int size_mats, long n_isotopes );
double * load_concs( int size_mats );
double * load_concs_v( int size_mats );
#pragma acc routine seq
int pick_mat(double roll);
#pragma acc routine seq
double rn(RNG_INT * seed);
void counter_stop( int * eventset, int num_papi_events );
void counter_init( int * eventset, int * num_papi_events );
void do_flops(void);
void do_loads( int nuc,
               NuclideGridPoint ** restrict nuclide_grids,
		       long n_gridpoints );	
void read_CLI( int argc, char * argv[], int *nthreads, long *n_isotopes, long
    *n_gridpoints, int *lookups, char *HM );
void print_CLI_error(void);
double rn_v(void);
double round_double( double input );
unsigned int hash(unsigned char *str, int nbins);
size_t estimate_mem_usage(long n_isotopes, long n_gridpoints);
void print_inputs(int nthreads, long n_isotopes, 
    long n_gridpoints, int lookups, char *HM, int nprocs, int version );
void print_results( int nthreads, long n_isotopes, long n_gridpoints, int
    lookups, char *HM,  int mype, double runtime, int nprocs, unsigned long
    long vhash );
void binary_dump(long n_isotopes, long n_gridpoints, NuclideGridPoint ** nuclide_grids,
		 GridPoint * energy_grid, int * grid_ptrs);
void binary_read(long n_isotopes, long n_gridpoints, NuclideGridPoint ** nuclide_grids,
		 GridPoint * energy_grid, int * grid_ptrs);
double timer();

#endif
