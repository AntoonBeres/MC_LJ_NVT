#include "rng.h"
using namespace std;
double gen_rng(int seed) {
  long m = pow(2, 45);
  long a = 25214903917;
  int c = 11;
  long result = (a * seed + c) % m;

  std::string rng_string;
  while(result>100) {
    rng_string.append(std::to_string(result%10));
    result /=10;
  }
  std::string return_string = "0.";
  return_string.append(rng_string);

  double result_val = std::stod(return_string);
  return result_val;
}
