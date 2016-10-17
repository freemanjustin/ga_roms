//  evol - a genetic algorithm code
//
// freeman.justin@gmail.com

#include "evol.h"

double randInRange(double min, double max){
  if (min==max) return 0.0;
  else return (double)( min + (rand() / (double) (RAND_MAX + 1.0) * (max - min)));
}

int randIntInRange(int min, int max){
  if (min==max) return 0;
  else return (int)( min + (rand() / (double) (RAND_MAX + 1.0) * (max - min)));
}



int main( int argc, char *argv[] )
{
	int				i,j;
	E 	*p; 					// pointer to parameter block
	agent 		*q;						// pointer to the genotype information
	agent		*r;						// pointer to the next generation
	unsigned 		rseed ;
  FILE  *rmse_file;
  char  fname[256];
  char  str[256];

  int   breeders;
  int   parent1;
  int   parent2;

	// malloc memory for the run time parameters
	if ( (p = (E *)malloc( sizeof(E)) )== NULL){
		fprintf(stderr,"Error mallocing memory for the parameter struct.\n");
		exit(1);
	}

	// check the command line args ...
	if ( argc < 3 ){
		printf("usage: ./evol [expe dir] [generation as int] [number of agents int]\n");
		exit(0);
	}
	else{
    get_command_line_arg_as_string(&p->exp, argv[1]);
		get_command_line_arg_as_int(&p->current_gen, argv[2]);
		get_command_line_arg_as_int(&p->population, argv[3]);
    printf("Experiment directory = %s\n", p->exp);
		printf("Current generation = %d\n", p->current_gen);
		printf("Population size = %d\n", p->population);
	}

	// Set the random seed
	rseed = (unsigned)(time(NULL) % 65536L ); // Random seed from time
	sran4( rseed );


	// set the run time parameters...
	//p->population = 50;				// number of agents in the population
	p->generations = 200;				// the number of evolutionary cycles

	p->mutation = 0.8;					// the probablility of mutation
	p->selection_pressure = 0.5;		// the probability of breeding. 0.6 means that only the top 40% get to breed.
	p->elitism = TRUE;						// fittest from previous generation stays in the population

	// data constraints

	p->x_max = 0.1;
	p->x_min = 0.0001;


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

  // malloc memory to read in the model rmse values
  p->rmse = malloc2d_double(p->population, 2);
  // read in the rmse rankings for this generation
  sprintf(fname,"%s/%04d/all_rmse.txt", p->exp, p->current_gen);
  printf("opening %s\n", fname);
  rmse_file = fopen(fname,"r");
  if(rmse_file == NULL){
    fprintf(stderr,"Couldn't open file %s\n", fname);
    exit(1);
  }
  int count = 0;
  while ( fgets( str, 256, rmse_file ) != NULL ){
    sscanf(str,"%lf %lf\n", &p->rmse[count][0], &p->rmse[count][1]);
    count++;
  }

  // print it out to check
  for(i=0;i<p->population;i++)
    printf("%f %f\n", p->rmse[i][0], p->rmse[i][1]);

  // apply selection pressure criteria and elitism
  breeders = ceil((1.0-p->selection_pressure) * (double)p->population);
  printf("breeders = 0 to %d\n", breeders);
  // print it out to check
  for(i=0;i<breeders;i++)
    printf("breeders: %f %f\n", p->rmse[i][0], p->rmse[i][1]);


  // generate the rest of the population
  // malloc room for the bottom drag grids
  get_dims(p,p->rmse[0][0]);
  p->bottomDrag = malloc2d_double(p->eta_rho, p->xi_rho);
  p->bottomDragGrids = malloc(breeders*sizeof(bDrag));
  // now malloc room for each bottom drag fields
  for(i=0;i<breeders;i++){
    p->bottomDragGrids[i].botDrag = malloc2d_double(p->eta_rho, p->xi_rho);
  }


  // read in all of the bottom drag fields
  for(i=0;i<breeders;i++){
    read_grid(p,p->rmse[i][0],i);
  }

  // have a look at the bottomDrag values
  printf("rdgrag2[100][100] = %0.12f\n", p->bottomDragGrids[0].botDrag[100][100]);

  //elitism:
  // the first model of the new generation is the fittest from the previous
  // copy the grid file to the new location
  for(i=0;i<p->eta_rho;i++){
		for(j=0;j<p->xi_rho;j++){
      p->bottomDrag[i][j] = p->bottomDragGrids[0].botDrag[i][j];
    }
  }
  write_grid(p,0);


  // now generate the rest of the population using
  // crossover and mutation
  int randInt;
  srand(time(NULL));
  for(i=1;i<p->population;i++){

    printf("agent %d: crossover\n", i);
    // randomly select two parents from the breeding group
    parent1 = randIntInRange(0, breeders);
    printf("\tparent1 = %d\n", parent1);
    do{
      parent2 = randIntInRange(0, breeders);
    }while(parent2 == parent1);
    printf("\tparent2 = %d\n", parent2);

    crossover_grid(p, parent1, parent2, i);

    // write the new rdrag2 fields into the next generation directories
    write_grid(p,i);
  }

	return(0);
}
