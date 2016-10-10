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

