#include <cmath>
#include <chrono>
#ifndef CALCULATIONS_H
#define CALCULATIONS_H
#endif

const static double sigma = 1.; //distance at which inter-particle potential is zero
const static double epsilon = 1.; //well depth


struct Atom {
  double x,y,z;
};

double distance (const Atom& atom1, const Atom& atom2);

double LJ_energy (const Atom& atom1, const Atom& atom2);
/*
double total_energy(Atom (&atoms) [NUMBER_OF_ATOMS])
{
  double energy = 0;
  for(int i=0; i < NUMBER_OF_ATOMS; i++){
    for(int j = 0; j < NUMBER_OF_ATOMS; j++){
      if(i != j){
        energy += (LJ_energy(atoms[i], atoms[j]))/2;
      }
    }
  }
  return energy;
}
*/
