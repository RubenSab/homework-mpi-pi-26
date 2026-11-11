.DEFAULT_GOAL := all

CC = mpicc
CPPFLAGS =
CFLAGS = -Wall -pedantic
LDFLAGS =
LDLIBS = 
# Legacy extra compiler flags remain supported.
FLAGS =

include student.mk

all: pi_seq pi_parallel

pi_seq: pi_seq.c Makefile student.mk
	$(CC) $(CPPFLAGS) $(CFLAGS) $(FLAGS) $(LDFLAGS) -o $@ $(filter %.c,$^) $(LDLIBS)

pi_parallel: pi_parallel.c student.c student.h Makefile student.mk
	$(CC) $(CPPFLAGS) $(CFLAGS) $(FLAGS) $(LDFLAGS) -o $@ $(filter %.c,$^) $(LDLIBS)

# FULL=1 enables all fixtures; MPIEXEC may override the MPI launcher.
test: pi_parallel
	FULL="$(FULL)" python3 test_smoke.py

clean:
	rm -f pi_seq pi_parallel

.PHONY: all clean test help 
