//  init_drag
//
//  randomly peturb bottom drag 2d field
//
// freeman.justin@gmail.com

#include "evol.h"

double randInRange(double min, double max){
  if (min==max) return 0.0;
  else return (double)( min + (rand() / (double) (RAND_MAX + 1.0) * (max - min)));
}



int main( int argc, char *argv[] )
{
	int				i,j,k;
	E 	*p; 					// pointer to parameter block
	agent 		*q;						// pointer to the genotype information
	agent		*r;						// pointer to the next generation
	unsigned 		rseed ;
  char  grid_file[260];
  double	rnum;


	// malloc memory for the run time parameters
	if ( (p = (E *)malloc( sizeof(E)) )== NULL){
		fprintf(stderr,"Error mallocing memory for the parameter struct.\n");
		exit(1);
	}

	// check the command line args ...
	if ( argc < 5 ){
		printf("usage: ./init [min] [max] [exp_name] [generation] [population]\n");
		exit(0);
	}
	else{
    get_command_line_arg_as_double(&p->x_min, argv[1]);
    get_command_line_arg_as_double(&p->x_max, argv[2]);
    get_command_line_arg_as_string(&p->exp_name, argv[3]);
		get_command_line_arg_as_int(&p->current_gen, argv[4]);
		get_command_line_arg_as_int(&p->population, argv[5]);

    printf("xmin = %f\n", p->x_min);
    printf("xmax = %f\n", p->x_max);
    printf("Experiment = %s\n", p->exp_name);
		printf("Current generation = %d\n", p->current_gen);
		printf("Population size = %d\n", p->population);
	}

	// Set the random seed
	//rseed = (unsigned)(time(NULL) % 65536L ); // Random seed from time
	//sran4( rseed );

	// Set up some memory that is used to store values temporarily
	if ( (q=(agent *) malloc( sizeof(agent) * p->population )) == NULL){
		printf(	"\nError mallocing memory for the population.\n");
		exit(1);
	}
	// Set up some memory that is used to store values temporarily
	if ( (r=(agent *) malloc( sizeof(agent) * p->population )) == NULL){
		printf(	"\nError mallocing memory for the population.\n");
		exit(1);
	}


	// read the initial drag grid from a netcdf file
  // as a test, randomly peturb the rdrag2 field and write it out to the grid file
  // init random number generator
  srand(time(NULL));

  for(k=0;k<p->population;k++){
    sprintf(grid_file,"./%s/%04d/%04d/roms_grid.nc", p->exp_name, p->current_gen, k);
    printf("%d opening: %s\n",k, grid_file);

  	nc_open(grid_file, NC_WRITE, &p->ncid);
  	get_dimension(p, "eta_rho", &p->eta_rho);
    get_dimension(p, "xi_rho", &p->xi_rho);
  	printf("eta_rho = %d (should be 541)\n", p->eta_rho);
  	printf("xi_rho = %d (should be 197)\n", p->xi_rho);

  	// read in the rdrag2 field: double rdrag2(eta_rho, xi_rho) ;
  	p->botDrag = malloc2d_double(p->eta_rho, p->xi_rho);
    p->mask = malloc2d_double(p->eta_rho, p->xi_rho);
    get_field(p, "mask_rho", &p->mask[0][0]);
    printf("got mask\n");
  	get_field(p, "rdrag2", &p->botDrag[0][0]);
  	printf("got rdrag2\n");

  	for(i=0;i<p->eta_rho;i++){
  	    for(j=0;j<p->xi_rho;j++){

          if(p->mask[i][j] == 1){
  		  //    p->botDrag[i][j] = randInRange(p->x_min, p->x_max);
        //  }


        		do{
        		    rnum = randInRange(-0.05, 0.05);
        		}while( (p->botDrag[i][j]+rnum < p->x_min) || (p->botDrag[i][j]+rnum > p->x_max));

        		p->botDrag[i][j] += rnum;

          }
  	    }
  	}
  	// splat it over the original field values
  	nc_put_var_double(p->ncid, p->varid, &p->botDrag[0][0]);
  	nc_close(p->ncid);
  }

	return 0;

}
