# Use this Makefile to compile the code
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

tutor: tutor.o
	$(CC) -o tutor tutor.o $(C_LINK)

tutor.o: tutor.c
	$(CC) $(C_ARGS) -c tutor.c
