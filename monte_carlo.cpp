#ifndef MONTE_CARLO_H
#include "monte_carlo.h"
#endif
#include <iostream>

const static double boltzmann = 0.008134;

Monte_carlo::Monte_carlo(int nr_of_atoms, double box_len, int seed) : box_length(box_len), nr_atoms(nr_of_atoms) {
  std::uniform_real_distribution<double> dist(0., box_length);
  rng.seed(seed);
  energy_cache_set = false;
  list_cache_set = false;
  for (int i=0; i < nr_of_atoms; i++){
    Atom new_atom;
    new_atom.x = dist(rng);
    new_atom.y = dist(rng);
    new_atom.z = dist(rng);
    atom_list.push_back(new_atom);
  }
}


double Monte_carlo::total_energy() {
  if(energy_cache_set) {
    return energy_cache;
  }

  double energy = 0;
  for(size_t i=0; i < atom_list.size(); i++){
    for(size_t j = i+1; j < atom_list.size(); j++){
      if(i != j){
        energy += LJ_energy(atom_list[i], atom_list[j]);
      }
    }
  }

  energy_cache = energy;
  return energy;
}


double Monte_carlo::energy_from_atom(size_t list_index) {
  double energy = 0;
  for(size_t i = 0; i < atom_list.size(); i++){
    if(i != list_index){
      energy += LJ_energy(atom_list[list_index], atom_list[i]);
    }
  }
  return energy;
}

// Returns true if move is accepted, false if move is rejected
bool Monte_carlo::try_move(double max_step) {
  int random_index = rand() % nr_atoms;
  //std::cout << "random index: " << random_index << std::endl;
  std::uniform_real_distribution<double> accept_move_dist(0, 1);
  if (!list_cache_set){
  cache_list = atom_list;
  list_cache_set=true;
  }
  std::mt19937 rng2;
  double old_energy = total_energy();
  double new_energy = old_energy - energy_from_atom((size_t)random_index);
  //std::cout << "old energy: " << old_energy << "\n";
  std::uniform_real_distribution<double> dist(-(max_step/2), (max_step/2));
  energy_cache_set = false;

  atom_list[random_index].x += dist(rng);
  if (atom_list[random_index].x < 0) atom_list[random_index].x = box_length - atom_list[random_index].x;
  atom_list[random_index].y += dist(rng);
  if (atom_list[random_index].y < 0) atom_list[random_index].y = box_length - atom_list[random_index].y;
  atom_list[random_index].z += dist(rng);
  if (atom_list[random_index].z < 0) atom_list[random_index].z = box_length-atom_list[random_index].z;

  new_energy += energy_from_atom((size_t)random_index);
  //std::cout << "new energy: " << new_energy << "\n";

  if(new_energy <= old_energy) {
    list_cache_set = false;
    return true;
  }
  else {
    double accept_probability = exp((-new_energy + old_energy)/(boltzmann));
    //std::cout << accept_probability << "\n";
    if (accept_move_dist(rng) <= accept_probability) {
      list_cache_set = false;
      return true;
    }
    else {
      atom_list = cache_list;
      return false;
    }
  }
}

double distance (const Atom& atom1, const Atom& atom2)
{
  return sqrt(pow((atom1.x - atom2.x),2) + pow((atom1.y - atom2.y),2) + pow((atom1.z * atom2.z),2));
}

double LJ_energy (const Atom& atom1, const Atom& atom2)
{
  return 4*epsilon*(pow((sigma/distance(atom1,atom2)),12) - pow((sigma/distance(atom1,atom2)),6));
}
