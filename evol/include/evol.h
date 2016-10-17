//  evol - a genetic algorithm code
//
// freeman.justin@gmail.com


#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <math.h>
#include <time.h>
#include <signal.h>
#include <ctype.h>

//#include "nrutil.h"
#include "rand.h"
#ifdef HAVEPGPLOT
#include "cpgplot.h"
#endif

#include <netcdf.h>

#include "jutil.h"

#define FALSE (0)
#define TRUE  (1)


#define	MAX(a,b)	( ((a) > (b)) ? (a) : (b) )
#define	MIN(a,b)	( ((a) < (b)) ? (a) : (b) )

#define GENE_LENGTH	(12)
#define GENE_SCALE	(1e12)

#define DIMENSIONS	(1)


// struct agent
typedef struct agent { 	double	xVal;
					double	yVal;
					double	zVal;
					double		fitness;
	       } agent ;	// agent consisting of chromosomes

typedef struct{
	double	**botDrag;
}bDrag;


// Set up a type that holds the run time parameters
typedef struct {
			char	*exp;	// experiment directory
			int		current_gen;
			int 	population;  	// number of agents in population
			int 	generations;  	// number of generations we evolve for
			double	crossover;		// crossover probability
			double	mutation; 		// how much mutation we have
			int 	elitism; 		// persistence of the fittest?
			double	selection_pressure;	// who gets to breed?
			int		imax;
			int		jmax;
			float	fittest;
			float	ave;
			float	adev;
			float	sdev;
			float	var;
			float	skew;
			float	curt;
			double	x_max;
			double	x_min;
			double	y_max;
			double	y_min;
			double	z_max;
			double	z_min;

			double	xVal;
			double	yVal;
			double	zVal;

			double	**rmse;


		// netcdf stuff
		int	ncid;
		int	varid;
		int 	retval;
		int	dimIds[NC_MAX_VAR_DIMS];

		// dimensions
		size_t	eta_rho;
		size_t	xi_rho;

		// variable ids
		int	drag;

		// variable data
		double	**bottomDrag;
		double  **mask;
		double	**parent1;
		double	**parent2;
		double	**daughter;

		bDrag	*bottomDragGrids;
       } E ;

// function prototypes
double obj_func(double x, double y, double z);

void rank_population(E *p, agent *q);

void sort2(unsigned long n, float arr[], float brr[]);

void copy_struct(E *p, int m, agent *r, int n, agent *q);

void breed(E *p, agent *q, agent *r, int j);

void breed_xover(E *p, agent *q, char *genome1, char *genome2);

#ifdef HAVEPGPLOT
void setvp();

void palett(int TYPE,float CONTRA,float BRIGHT);

int image(E *p, char* fname, float** fort_array, int c_map);
int contour(E *p, char* fname, float** val, int c_map);
int contour_movie(E *p, char* fname, float** val, int c_map);
int image_cont(E *p, char* fname, float** val, int c_map);
int movie(E *p, char* fname, float** val, int c_map, char* title);
int vector_movie(E *p, char *fname, float** u, float **v);
int jvector(E *p, char *fname, float** u, float **v);
int movie_frames(E *p, char* fname, float** val, int c_map, char* title, agent *q);
#endif

float** malloc2d (int x, int y);

void get_stats(float *data, int n, float *ave, float *adev, float *sdev,
	float *var, float *skew, float *curt);

int compare(const void *p1, const void *p2);

void get_field(E*, char*, void*);
void get_attribute(E*, char*, char*, double*);
void get_dimension(E*, char*, size_t*);

void get_dims(E *p, int whichGrid);
void read_grid(E *p, int  whichGrid, int thisGrid);
void write_grid(E *p, int  whichGrid);

void crossover_grid(E *p, int parent1, int parent2, int whichGrid);


int randIntInRange(int min, int max);
double randInRange(double min, double max);
