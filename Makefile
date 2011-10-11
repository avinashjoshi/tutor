# Use this Makefile to compile tutor program
# Collaborators: Avinash Joshi <axj107420@utdallas.edu>
#                Sandeep Shenoy <sxs115220@utdallas.edu>

# Use this if pthread is needed to be
# linked explicit
C_LINK = -pthread

# Comment the following line while not
# using the gnu c compiler
C_ARGS = -Wall

CC = gcc

# No more changes below this line

all: tutor

clean:
	@echo Removing object files: rm -f *.o
	@rm -f *.o

cleanall: clean
	@echo Removing compiled files: rm -f tutor
	@rm -f tutor

tutor: tcp.o udp.o join.o services.o tutor.o 
	$(CC) -o tutor join.o tcp.o udp.o services.o tutor.o $(C_LINK)

tutor.o: tutor.c
	$(CC) $(C_ARGS) -c tutor.c

tcp.o: tcp.c
	$(CC) $(C_ARGS) -c tcp.c

udp.o: udp.c
	$(CC) $(C_ARGS) -c udp.c $(C_LINK)

join.o: join.c
	$(CC) $(C_ARGS)	-c join.c $(C_LINK)

services.o: services.c
	$(CC) $(C_ARGS)	-c services.c $(C_LINK)
