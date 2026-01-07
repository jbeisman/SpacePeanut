

#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>

#include "utils.hh"
#include <cmath>

// this file contains unfinished work
// TODO:
//      - write to binary format
//      - increase output field options
//      - improve upon random initialization routine
//      - improve cosmology and scale factor representation

void initialize_random(std::vector<std::array<float, 3>> &Vec,
                       const float min_value, const float max_value) {
  std::random_device rd;
  std::mt19937 generator(rd());
  std::uniform_real_distribution<float> distribution(min_value, max_value);
  for (auto &elem : Vec) {
    elem[0] = distribution(generator);
    elem[1] = distribution(generator);
    elem[2] = distribution(generator);
  }
}
