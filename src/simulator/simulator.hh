
#pragma once

#include "particle_mesh_system.hh"
#include "space_time.hh"
#include <cmath>

class ParticleMeshSimulator {

public:
  ParticleMeshSimulator();
  ~ParticleMeshSimulator();
  void initialize_simulation(const double redshift, const int ntimesteps,
                             const int nbods, const int ngrid,
                             const float gridlength);
  void advance_single_timestep();
  std::array<float, 3>* get_positions();
  float* get_mass_density();
  std::vector<float, fftwf_allocator<float>>& get_mass_density_ref();
  double get_redshift();

private:
  std::unique_ptr<ParticleMeshSystem> pm_system;
  SpaceTime space_time;
};
