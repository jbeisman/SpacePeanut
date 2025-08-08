


#include <random>
#include <iostream>
#include <iomanip>
#include <fstream>

#include <cmath>
#include "utils.hh"

// this file contains unfinished work
// TODO:
//      - write to binary format
//      - increase output field options
//      - improve upon random initialization routine
//      - improve cosmology and scale factor representation

void initialize_random(std::vector<std::array<float,3>>& Vec, const float min_value, const float max_value) {
	std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_real_distribution<float> distribution(min_value, max_value);
	for (auto& elem : Vec) {
		elem[0] = distribution(generator);
		elem[1] = distribution(generator);
		elem[2] = distribution(generator);
    }
}


void write_points(const int filenum, const std::vector<std::array<float, 3>>& Pos, const std::vector<std::array<float, 3>>& Vel, const float time_scale) {
	auto vel_mg = std::vector<float>(Vel.size());
    
    for (std::size_t i = 0; i < Vel.size(); i++) {
        vel_mg[i] = sqrtf( (Vel[i][0]*Vel[i][0] + Vel[i][1]*Vel[i][1] + Vel[i][2]*Vel[i][2]) / (time_scale*time_scale));
    }
	    //std::transform(std::execution::par_unseq, Vel.begin(), Vel.end(), vel_mg.begin(),
        //[=](const auto vel) {
        //  	return sqrtf( (vel[0]*vel[0] + vel[1]*vel[1] + vel[2]*vel[2]) / (time_scale*time_scale) );
    	//});
  std::ofstream outfile("velocity_magnitude." + std::to_string(filenum) + ".3D");
  outfile << std::setprecision(8);
  outfile << "x y z velocity\n";
  outfile << "#coordflag xyzm\n";
  for (std::size_t i = 0; i < Pos.size(); i++) {
    outfile << Pos[i][0] << " " << Pos[i][1] << " "
            << Pos[i][2] << " " << vel_mg[i] << "\n";
  }
}


float compute_H(const float a)
{
    constexpr float H0 = 0.67321 * 100.0;
    constexpr float omega_lambda = 0.6842;
    constexpr float omega_m = 0.3158;
    constexpr float omega_k = 1.0 - omega_m - omega_lambda;
    return H0 * sqrtf(
        omega_lambda
        + omega_m / (a * a * a)
        + omega_k / (a * a)
    );
}
