###################################################################
#
# freeman.justin@gmail.com 
#
##################################################################

OBJ=	\
	./src/main.o \
	./src/jutil.o \
	./src/netcdfIO.o \
	./src/rand.o

# compliler flags
INC=    -I./include 

CFLAGS=	-g -O3 -Wall

#-DHAVEPGPLOT
CC=	gcc $(CFLAGS) $(INC) 

# Libraries 
LFLAGS= -lnetcdf	

# executable
EXEC=	../run_it/init_drag	

$(EXEC):$(OBJ)
	$(CC) -o $(EXEC) $(OBJ) $(LFLAGS)

clean:
	rm $(OBJ)
	rm $(EXEC)
