###################################################################
#
# freeman.justin@gmail.com 
#
##################################################################

OBJ=	./src/breed.o \
	./src/breed_xover.o \
	./src/copy_struct.o \
	./src/graphics.o \
	./src/main.o \
	./src/malloc2d.o \
	./src/stats.o \
	./src/obj_func.o \
	./src/rand.o \
	./src/rank_population.o \
	./src/sort.o \
	./src/jutil.o \
	./src/netcdfIO.o

# compliler flags
INC=    -I./include 

CFLAGS=	-g -O3 -Wall

#-DHAVEPGPLOT
CC=	gcc $(CFLAGS) $(INC) 

# Libraries 
LFLAGS= -lnetcdf	

# executable
EXEC=	./bin/evol	

$(EXEC):$(OBJ)
	$(CC) -o $(EXEC) $(OBJ) $(LFLAGS)

clean:
	rm $(OBJ)
	rm $(EXEC)
