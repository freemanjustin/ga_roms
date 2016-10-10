#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdarg.h>

// netCDF header
#include <netcdf.h>

// macros
#define TRUE 1
#define FALSE 0

#define max(x,y) (((x) > (y)) ? (x) : (y))
#define min(x,y) (((x) < (y)) ? (x) : (y))

// Handle errors by printing an error message and exiting with a
// non-zero status.
#define ERRCODE 2
#define ERR(e) {printf("Error: %s\nFunction: %s\nFile: %s\nLine %d\n", nc_strerror(e),__func__,__FILE__,__LINE__); exit(ERRCODE);}

#define fail(...) my_fail(__LINE__,__func__,__FILE__,__VA_ARGS__)


typedef struct {

    // netcdf variables
    int     ncid;
    int     varid;
    int     retval;

    // index variables
    size_t      ocean_time;
    size_t      eta_rho;
    size_t	xi_rho;

    // field
    double      ***u_ref;
    double	***v_ref;
    double	***u;
    double	***v;
    double	***vel_ref;
    double	***vel;
    double  	*rmse;

}e;



// prototypes
void my_fail( const int, const char*, const char*, const char*, ... );
void get_field( e* , char*, void* );
void get_attribute(e*, char*, char*, double* );
double ****malloc4d_double(int dim1, int dim2, int dim3, int dim4);
void calc_vrms( e *E, double ****field );


void my_fail( const int line, const char *func, const char *file, const char *msg, ... )
{
    va_list args;

    fprintf( stdout, "mdbcat error: file %s in function %s at line %d \n", file, func, line );

    va_start(args, msg);
    vprintf(msg, args);
    va_end(args);

    fflush( stdout );

    exit(1);

}

void get_field(e *E, char *field_name, void* field){

    if(( E->retval = nc_inq_varid(E->ncid, field_name, &E->varid)))
        ERR(E->retval);

    if ((E->retval = nc_get_var_double(E->ncid, E->varid, (double*)field)))
        ERR(E->retval);

}

void get_attribute(e *E, char *var, char *att_name, double *att){

    if(( E->retval = nc_inq_varid(E->ncid, var, &E->varid)))
        ERR(E->retval);

    if((E->retval = nc_get_att_double(E->ncid, E->varid, att_name, att)))
        ERR(E->retval);

}

void get_dimension(e *E, char *dim_name, size_t *dim){


    if((E->retval = nc_inq_dimid(E->ncid, dim_name, &E->varid)))
        ERR(E->retval);
    if((E->retval = nc_inq_dimlen(E->ncid,E->varid,dim)))
        ERR(E->retval);
}

double ****malloc4d_double(int dim1, int dim2, int dim3, int dim4)
{
	size_t		layer1_count = dim1;
	size_t		layer2_count = dim1 * dim2;
	size_t		layer3_count = dim1 * dim2 * dim3;
	size_t		layer1_size = sizeof(double ***) * layer1_count;
	size_t		layer2_size = sizeof(double **) * layer2_count;
	size_t	 	layer3_size = sizeof(double *) * layer3_count;
	size_t 	layers_size = layer1_size + layer2_size + layer3_size;

	size_t	 	data_count = dim1 * dim2 * dim3 * dim4;
	size_t 	data_size = sizeof(double) * data_count;

	char 	*raw_bytes = (void*)malloc(layers_size + data_size);

	double ****layer1 = (double ****)(raw_bytes);
	double ***layer2 = (double ***)(raw_bytes + layer1_size);
	double **layer3 = (double **)(raw_bytes + layer1_size + layer2_size);

	double *double_data = (double *)(raw_bytes + layers_size);

	int i, j, k;
	double ***this_layer2;
	double **this_layer3;

	for (i = 0; i < dim1; i++) {
		layer1[i] = layer2 + (i * dim2);
		this_layer2 = layer1[i];

		for (j = 0; j < dim2; j++) {
			this_layer2[j] = layer3 + (i * dim2 * dim3 + j * dim3);
			this_layer3 = this_layer2[j];

			for (k = 0; k < dim3; k++) {
				this_layer3[k] = double_data + (i * dim2 * dim3 * dim4 + j * dim3 * dim4 + k * dim4);
			}
		}
	}
	return layer1;
}

double ***malloc3d_double(int dim1, int dim2, int dim3)
{

        size_t          layer1_count = dim1;
        size_t          layer2_count = dim1 * dim2;
        size_t          layer1_size = sizeof(double ***) * layer1_count;
        size_t          layer2_size = sizeof(double **) * layer2_count;

        size_t  layers_size = layer1_size + layer2_size ;

        size_t          data_count = dim1 * dim2 * dim3 ;
        size_t  data_size = sizeof(double) * data_count;

        void    *raw_bytes = (void*)malloc(layers_size + data_size);

        double ***layer1 = (double ***)(raw_bytes);
        double **layer2 = (double **)(raw_bytes + layer1_size);
        double *double_data = (double *)(raw_bytes + layers_size);

        int i, j;
        double **this_layer2;
        //double *this_layer3;

        for (i = 0; i < dim1; i++) {
                layer1[i] = layer2 + (i * dim2);
                this_layer2 = layer1[i];

                for (j = 0; j < dim2; j++) {
                        this_layer2[j] = double_data + (i * dim2 * dim3 + j * dim3);
                }
        }
        return layer1;

}

/*
void calc_vrms( e *E, double ****field )
{

	int	k,j,i;
	double n;


	E->rms = 0.0;
	n = 0.0;

	for(i=0;i<E->zw_ocean;i++){
		for(j=0;j<E->yt_ocean;j++){
			for(k=0;k<E->xt_ocean;k++){
				if(field[0][i][j][k]!=-10.0){
					E->rms += pow(field[0][i][j][k], 2.0);
					n+=1.0;
				}
			}
		}
	}

	E->rms = sqrt( E->rms/n );
}
*/

void calc_rmse(e *E){

	int	i,j,t;
	double n;
	for(t=0;t<E->ocean_time;t++){
		E->rmse[t] = 0.0;
		n = 0.0;
		for(i=0;i<E->eta_rho;i++){
			for(j=0;j<E->xi_rho;j++){

			//	if( (i==25) && (j== 7)){
			//		printf("vel = %f, vel_ref = %f\n", E->vel[t][i][j], E->vel_ref[t][i][j]);
			//		exit(1);
			//	}
				if(E->vel[t][i][j] < 500.0){
					E->rmse[t] += pow(( E->vel[t][i][j] - E->vel_ref[t][i][j]), 2.0);
			//		printf("(%d, %d) n = %f, rmse sum = %f\n",i,j, n, E->rmse[t]);
					n = n + 1.0;
				}

			}
		}
		E->rmse[t] = E->rmse[t]/n;
	}
}



int main(int argc, char** argv)
{
  e   *E;
  FILE  *out;
  int t,i,j;

  E = malloc(sizeof(e));
  if(E==NULL) fail("Malloc failed\n");

	if ((E->retval = nc_open(argv[1], NC_NOWRITE, &E->ncid)))
		ERR(E->retval);

	get_dimension(E, "eta_rho", &E->eta_rho);
	get_dimension(E, "xi_rho", &E->xi_rho);
	get_dimension(E, "ocean_time", &E->ocean_time);



	E->u_ref = malloc3d_double(E->ocean_time,E->eta_rho, E->xi_rho);
	get_field(E,"ubar_eastward", &E->u_ref[0][0][0]);

  E->v_ref = malloc3d_double(E->ocean_time,E->eta_rho, E->xi_rho);
  get_field(E,"vbar_northward", &E->v_ref[0][0][0]);


	nc_close(E->ncid);

	if ((E->retval = nc_open(argv[2], NC_NOWRITE, &E->ncid)))
    ERR(E->retval);

	E->u = malloc3d_double(E->ocean_time,E->eta_rho, E->xi_rho);
  get_field(E,"ubar_eastward", &E->u[0][0][0]);

  E->v = malloc3d_double(E->ocean_time,E->eta_rho, E->xi_rho);
  get_field(E,"vbar_northward", &E->v[0][0][0]);


  nc_close(E->ncid);

	E->vel = malloc3d_double(E->ocean_time,E->eta_rho, E->xi_rho);
	E->vel_ref = malloc3d_double(E->ocean_time,E->eta_rho, E->xi_rho);

	// cal velocity magnitude
	for(t=0;t<E->ocean_time;t++){
	  for(i=0;i<E->eta_rho;i++){
	    for(j=0;j<E->xi_rho;j++){
		      if( (E->u[t][i][j]<1000.0) && (E->u_ref[t][i][j]<1000.0) ){
            E->vel_ref[t][i][j] = sqrt(pow(E->u_ref[t][i][j],2.0) + pow(E->v_ref[t][i][j],2.0));
		          E->vel[t][i][j] = sqrt(pow(E->u[t][i][j],2.0) + pow(E->v[t][i][j],2.0));
		      }
		      else{
		          E->vel_ref[t][i][j] = E->vel[t][i][j] = 1001.0;
		      }
      }
	  }
  }


	// malloc the rmse array
	E->rmse = malloc(E->ocean_time*sizeof(double));

	calc_rmse( E );

  out = fopen("rmse.txt","w");
	for(t=0;t<E->ocean_time;t++)
		fprintf(out,"%d\t%f\n", t, E->rmse[t]);
  fclose(out);

	return 0;
}
