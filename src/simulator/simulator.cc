
#include "simulator.hh"
#include "fftw3f_util.hh"
#include "get_input.hh"
#include <fstream>
#include <iomanip>
#include <iostream>

ParticleMeshSimulator::ParticleMeshSimulator() {
  fft_util::init_fftw3f();
  pm_system = std::make_unique<ParticleMeshSystem>();
  space_time = std::make_unique<CosmicTime>();
}

ParticleMeshSimulator::~ParticleMeshSimulator() { fft_util::cleanup_fftw3f(); }

void ParticleMeshSimulator::initialize_simulation(const float redshift,
                                                  const int ntimesteps,
                                                  const int nbods,
                                                  const int ngrid,
                                                  const float gridlength) {
  pm_system->setup_system(nbods, ngrid, gridlength);
  space_time->setup_time(redshift, ntimesteps);
  float time_scale = space_time->get_time_scale();
  pm_system->initialize_system(
      -10000.0, 10000.0,
      time_scale); // TODO - better initialization system and user options
}

void ParticleMeshSimulator::advance_single_timestep() {
  float dt = space_time->get_dt();
  pm_system->integrate_timestep(dt, space_time->get_time());
  space_time->update_time(dt);
}

void ParticleMeshSimulator::sim_change_dt(float new_dt) {
  space_time->update_dt(new_dt);
}

std::array<float, 3> *ParticleMeshSimulator::get_positions() {
  return pm_system->pPos.data();
}

float *ParticleMeshSimulator::get_mass_density() {
  return pm_system->gMass.data();
}

std::vector<float, fftwf_allocator<float>> &
ParticleMeshSimulator::get_mass_density_ref() {
  return pm_system->gMass;
}

