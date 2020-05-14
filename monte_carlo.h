#include <vector>
#include <random>
#include <algorithm>
#include <cmath>
#include <chrono>

#ifndef MONTE_CARLO_H
#define MONTE_CARLO_H
#endif

#include <pybind11/pybind11.h>
namespace py = pybind11;


const static double sigma = 1.; //distance at which inter-particle potential is zero
const static double epsilon = 1.; //well depth


struct Atom {
  double x,y,z;
};

double distance (const Atom& atom1, const Atom& atom2);

double LJ_energy (const Atom& atom1, const Atom& atom2);

class Monte_carlo {
public:
  Monte_carlo(int nr_atoms, double box_len);
  double total_energy();
  bool try_move(double max_step);

  double energy_from_atom(size_t list_index);

private:
  std::vector<Atom> atom_list;
  std::vector<Atom> cache_list;
  const double box_length;
  double energy_cache;
  bool energy_cache_set;
  bool list_cache_set;
  std::mt19937 rng;

  int nr_atoms;
};


PYBIND11_MODULE(example, m) {
    py::class_<Monte_carlo>(m, "Monte_carlo")
        .def(py::init<int &, double&>())
        .def("total_energy", &Monte_carlo::total_energy)
        .def("try_move", &Monte_carlo::try_move);
        //.def("getName", &Pet::getName);
    py::class_<Atom>(m, "Atom");
}
