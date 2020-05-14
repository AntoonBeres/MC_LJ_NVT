OUTPUTLIBNAME=example
CFLAGS=-Wall -shared -std=c++11 -fPIC `python3 -m pybind11 --includes`
OPTLEVEL=-O3
main: monte_carlo.cpp monte_carlo.h
	c++ $(OPTLEVEL) $(CFLAGS) monte_carlo.cpp -o $(OUTPUTLIBNAME)`python3-config --extension-suffix`
