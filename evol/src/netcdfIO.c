#include "evol.h"

void get_field(E *p, char *field_name, void* field){

    if(( p->retval = nc_inq_varid(p->ncid, field_name, &p->varid))){
        printf("inq_varid failed: %d\n", p->retval);
	exit(1);
    }

    if ((p->retval = nc_get_var_double(p->ncid, p->varid, (double*)field))){
        printf("get var_double failed: %d\n",p->retval);
	exit(1);
    }
}


void get_dimension(E *p, char *dim_name, size_t *dim){


    if((p->retval = nc_inq_dimid(p->ncid, dim_name, &p->varid))){
        printf("get_dimid failed for dim_name: %s (error code is %d)\n", dim_name, p->retval);
	exit(1);
    }

    if((p->retval = nc_inq_dimlen(p->ncid,p->varid,dim))){
        printf("inq_dimlen failed: %d\n", p->retval);
	exit(1);
    }
}

void get_dims(E *p, int whichGrid){
  char  fname[256];
  // read the initial drag grid from a netcdf file

  sprintf(fname,"%s/%04d/%04d/roms_grid.nc", p->exp, p->current_gen, whichGrid);
  printf("from read_grid(): opening %s\n", fname);

  nc_open(fname, NC_WRITE, &p->ncid);
  get_dimension(p, "eta_rho", &p->eta_rho);
  get_dimension(p, "xi_rho", &p->xi_rho);
  printf("\teta_rho = %d (should be 541)\n", p->eta_rho);
  printf("\txi_rho = %d (should be 197)\n", p->xi_rho);
  nc_close(p->ncid);

}

void read_grid(E *p, int  whichGrid, int thisGrid){

  char  fname[256];
  // read the initial drag grid from a netcdf file

  sprintf(fname,"%s/%04d/%04d/roms_grid.nc", p->exp, p->current_gen, whichGrid);
  printf("from read_grid(): opening %s\n", fname);

  nc_open(fname, NC_WRITE, &p->ncid);

  // read in the rdrag2 field: double rdrag2(eta_rho, xi_rho) ;
  get_field(p, "rdrag2", &p->bottomDragGrids[thisGrid].botDrag[0][0]);
  printf("\tgot rdrag2\n");

  nc_close(p->ncid);

}

void write_grid(E *p, int  whichGrid){

  char  fname[256];
  // read the initial drag grid from a netcdf file

  sprintf(fname,"%s/%04d/%04d/roms_grid.nc", p->exp, p->current_gen+1, whichGrid);
  printf("from write_grid(): opening %s\n", fname);

  nc_open(fname, NC_WRITE, &p->ncid);

  // read in the rdrag2 field: double rdrag2(eta_rho, xi_rho) ;

  // get the varid for rdrag2
  if(( p->retval = nc_inq_varid(p->ncid, "rdrag2", &p->varid))){
      printf("inq_varid failed: %d\n", p->retval);
      exit(1);
  }
  // splat it over the original field values
  //nc_put_var_double(p->ncid, p->varid, &p->bottomDragGrids[whichGrid].botDrag[0][0]);
  nc_put_var_double(p->ncid, p->varid, &p->bottomDrag[0][0]);
  nc_close(p->ncid);

}


/*
// splat it over the original field values
nc_put_var_double(p->ncid, p->varid, &p->botDrag[0][0]);
nc_close(p->ncid);
*/
