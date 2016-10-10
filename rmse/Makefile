###################################################################
#
#  j.freeman@bom.gov.au
#
##################################################################

OBJ=	main.o 
	    
# compliler flags
INC=	

CFLAGS= -O3

LIBS=	-lnetcdf 

CC=	gcc

# Libraries for linking

LFLAGS=	-lm $(LIBS)
# Name of executable goes here:

EXEC=	../run_it/rmse	

default:$(OBJ)
	$(CC) $(CFLAGS) $(INC) -o $(EXEC) $(OBJ) $(LFLAGS)
	@echo 
	@echo "build complete"
	@echo "usage:" 
	@echo " "  $(EXEC) "[input ofam file]"
	@echo 

clean:
	-rm $(OBJ)
	-rm $(EXEC)

# suffix rules
$(OBJ) : %.o : %.c
	$(CC) $(INC) $(CFLAGS) -c $< -o $@
