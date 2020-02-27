
CSRCS 	:= $(wildcard *.c)
CPPSRCS	:= $(wildcard *.cpp)
CCSRCS	:= $(wildcard *.cc)
OBJS 	:= $(CSRCS:.c=.o) $(CPPSRCS:.cpp=.o) $(CCSRCS:.cc=.o)
RM   	:= rm -rf
MPI  	:=
MPICC	:= /usr/lib/mpich/bin/mpicc
MPICPP	:= /usr/lib/mpich/bin/mpic++
CFLAGS 	:=
CPPFLAGS:= 

all: clean check build

build: $(OBJS)
	@echo ''
	@echo 'Building target: $(OBJS)'
	$(MPICC) -o a.out $(OBJS)
	@echo 'Finished building target: $(OBJS)'
	@echo ''
%.o: %.cpp
	$(MPICPP) $(CPPFLAGS) -c $<
%.o: %.c
	$(MPICC) $(CFLAGS) -c $<
%.o: %.cc
	$(MPICC) $(CFLAGS) -c $<
clean:
	@echo ''
	-$(RM) a.out
	@echo ''
check:
	@test -s $(MPICC) || { echo "mpicc does not eists!"; exit 1; }
	@test -s $(MPICPP) || { echo "mpic++ does not eists!"; exit 1; }
