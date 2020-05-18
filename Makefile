OUTPUTLIBNAME=MC_LIB
CFLAGS=-Wall -shared -std=c++11 -fPIC `python3 -m pybind11 --includes`
OPTLEVEL=-O3
NPROCS=12
main: monte_carlo.o rng.o
	c++ $(OPTLEVEL) $(CFLAGS) monte_carlo.o rng.o -o $(OUTPUTLIBNAME)`python3-config --extension-suffix`
monte_carlo.o:
	c++ $(CFLAGS) -c monte_carlo.cpp
rng.o:
	c++ $(CFLAGS) -c rng.cpp
run: main.py
	mpiexec -n $(NPROCS) --use-hwthread-cpus python main.py
