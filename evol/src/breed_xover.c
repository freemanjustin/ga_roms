//  evol - a genetic algorithm code
//
// freeman.justin@gmail.com

#include "evol.h"

void crossover_grid(E *p, int parent1, int parent2, int whichGrid){

	double	rnum;
	double	mutated;
	int			rnum2, rnum3;
	int			xover_point, mutation_point,mutation;
	int			i,j;
	char		mutation_str[2];

	char	gene_string1[GENE_LENGTH*DIMENSIONS+1],gene_string2[GENE_LENGTH*DIMENSIONS+1];
	char	daughter[GENE_LENGTH*DIMENSIONS+1];
	char	str1[GENE_LENGTH+1],str2[GENE_LENGTH+1], str3[GENE_LENGTH+1];

	// randomly choose a crossover point
	//rnum = ran4();
	rnum = randInRange(0.1, 0.9);
	xover_point = (int)ceil(GENE_LENGTH*rnum); // added the ceil() call so we never x-over at zero in the string
	//fprintf(stdout,"genome1: %s\tgenome2: %s\txover_point: %d\n",genome1,genome2,xover_point);
	//printf("* crossover_grid: rnum = %f, xover_point = %d\n", rnum, xover_point);


	// generate the new grid by copying parent2 string from xover point to
	// parent1 string at xover
	for(i=0;i<p->eta_rho;i++){
		for(j=0;j<p->xi_rho;j++){
			// create the two genes for each parent as strings
			sprintf(&gene_string1[0],"%012ld", (long int)(GENE_SCALE*p->bottomDragGrids[parent1].botDrag[i][j]) );
			sprintf(&gene_string2[0],"%012ld", (long int)(GENE_SCALE*p->bottomDragGrids[parent2].botDrag[i][j]) );
			//printf("gene string 1 = %s (was: %12f, scaled = %f)\n", gene_string1, p->bottomDragGrids[parent1].botDrag[i][j],
		//GENE_SCALE*p->bottomDragGrids[parent1].botDrag[i][j]);
			//printf("gene string 2 = %s (was: %12f, scaled = %f)\n", gene_string2, p->bottomDragGrids[parent2].botDrag[i][j],
		//GENE_SCALE*p->bottomDragGrids[parent2].botDrag[i][j]);

			// apply crossover between parents. we always copy from parent2 into parent1
			memcpy(&gene_string1[xover_point],&gene_string2[xover_point],(DIMENSIONS*GENE_LENGTH-xover_point));

			// mutate the string
			// single, randomly chosen mutation point
			do{
				// copy the crossover'd gene into daughter and mutate
				memcpy(daughter, gene_string1, GENE_LENGTH*DIMENSIONS+1);

				// now mutate the string
				rnum2 = randIntInRange(1,10);	// mutation_point
				rnum3 = randIntInRange(0,9);	// mutation_value
				sprintf(mutation_str,"%d",rnum3);
				memcpy(&daughter[rnum2],mutation_str,1);
				// convert back to floating point number
				sscanf(daughter,"%lf",&mutated);
				mutated = mutated/GENE_SCALE;
				//printf("# daughter = %s, mutation point = %d, mutation val = %d, as float = %0.12f\n", daughter, rnum2, rnum3, mutated);
			} while ( (mutated < p->x_min) && (mutated > p->x_max));
			//p->bottomDrag[i][j] = p->bottomDrag[i][j]/GENE_SCALE;
			p->bottomDrag[i][j] = mutated;
			//printf("--> new gene      = %s\n\txover point = %d, mutation point = %d,  mutation value = %d, as float = %0.12f\n",daughter, xover_point, rnum2, rnum3, p->bottomDrag[i][j]);
		}
	}




}


void breed_xover(E *p, agent *q, char *genome1, char *genome2)
{
	double		rnum;
	int			xover_point, mutation_point,mutation;
	int			i,j;
	char		daughter[80];
	char		mutation_srt[2];

	// randomly choose a crossover point
	rnum = ran4();
	xover_point = (int)ceil(GENE_LENGTH*rnum); // added the ceil() call so we never x-over at zero in the string
	//fprintf(stdout,"genome1: %s\tgenome2: %s\txover_point: %d\n",genome1,genome2,xover_point);

	// now xover the 2 strings to make the new offspring
	// either we copy genetic information from A->B or B->A.
	rnum = ran4();
	if(rnum < 0.5 ) {
		memcpy(&genome1[xover_point],&genome2[xover_point],(DIMENSIONS*GENE_LENGTH-xover_point));
		sprintf(daughter,"%s",genome1);
		//fprintf(stdout,"\nafter xover event:\n%s\t%s\ndaughter = %s\n",genome1,genome2,daughter);
	}
	else {
		memcpy(&genome2[xover_point],&genome1[xover_point],(DIMENSIONS*GENE_LENGTH-xover_point));
		sprintf(daughter,"%s",genome2);
		//fprintf(stdout,"\nafter xover event:\n%s\t%s\ndaughter = %s\n",genome1,genome2,daughter);
	}

	// now mutate a gene if the probability test is satisfied
	rnum = ran4();
	if(rnum < p->mutation) {
		rnum = ran4();
		j = (int)ceil(2*GENE_LENGTH*rnum);
		for(i=0;i<j;i++) {
			rnum = ran4();	// randomly pick a gene to mutate
			mutation_point = (int)(DIMENSIONS*GENE_LENGTH*rnum);
			rnum = ran4();	// now mutate that gene
			mutation = (int)floor(10.0*rnum); 	// mutation is an integer between 0 and 9
			sprintf(mutation_srt,"%d",mutation);
			memcpy(&daughter[mutation_point],mutation_srt,1);
			//fprintf(stdout,"\n--- mutated a gene ---\ngene %d\t%s\n",mutation_point,daughter);
		}
	}
	sprintf(genome1,"%s",daughter);
}
