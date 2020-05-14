#ifndef CALCULATIONS_H
#include "calculations.h"
#endif

double distance (const Atom& atom1, const Atom& atom2)
{
  return sqrt(pow((atom1.x - atom2.x),2) + pow((atom1.y - atom2.y),2) + pow((atom1.z * atom2.z),2));
}

double LJ_energy (const Atom& atom1, const Atom& atom2)
{
  return 4*epsilon*(pow((sigma/distance(atom1,atom2)),12) - pow((sigma/distance(atom1,atom2)),6));
}
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
