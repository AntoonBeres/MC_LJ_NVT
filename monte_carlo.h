#include <vector>
#include <random>
#include <algorithm>
#include <cmath>
#include <chrono>
#include "rng.h"

#ifndef MONTE_CARLO_H
#define MONTE_CARLO_H


#include <pybind11/pybind11.h>
namespace py = pybind11;

//Some constants used in the LJ-energy calculations, can be changed
const static double sigma = 1.; //distance at which inter-particle potential is zero
const static double epsilon = 1.; //well depth

// An arbitrary atom, defind by 3 carthesian coordinates
struct Atom {
  double x,y,z;
};

//Returns the distance between 2 atoms
double distance (const Atom& atom1, const Atom& atom2);

//returns the Lennard-Jones interaction energy between 2 atoms
double LJ_energy (const Atom& atom1, const Atom& atom2);


//define class to be exported to and used in python 
class Monte_carlo {
public:
  Monte_carlo(int nr_atoms, double box_len, int seed);
  double total_energy();  //function for calculation the total energy of the system
  bool try_move(double max_step); //returns true if move is accepted, false if move was rejected

  double energy_from_atom(size_t list_index); //The energy contribution of an individual atom, used to reduce amount of calculations of total energy

private:
  std::vector<Atom> atom_list; //holds the list of atoms
  std::vector<Atom> cache_list; //cache-list so that when a move is rejected, positions can be reverted to positions from cache
  const double box_length;
  double energy_cache;  //energy_cache to reduce amount of energy-calculations when possible

  bool energy_cache_set; //true if energy_cache is usable
  bool list_cache_set; //true if list_cache is usable

  std::mt19937 rng; //c++ RNG

  int nr_atoms; //nr_of atoms to initialize
};


// Define python bindings
PYBIND11_MODULE(MC_LIB, m) {
    py::class_<Monte_carlo>(m, "Monte_carlo")
        .def(py::init<int &, double&, int&>())
        .def("total_energy", &Monte_carlo::total_energy)
        .def("try_move", &Monte_carlo::try_move);
    py::class_<Atom>(m, "Atom");
    m.def("gen_rng", &gen_rng);
}
#endif
