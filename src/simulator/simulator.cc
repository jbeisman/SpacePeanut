
#include "simulator.hh"
#include "fftw3f_util.hh"
#include "get_input.hh"
#include <fstream>
#include <iomanip>
#include <iostream>


ParticleMeshSimulator::ParticleMeshSimulator() {
  fft_util::init_fftw3f();
  this->pm_system = std::make_unique<ParticleMeshSystem>();
}

ParticleMeshSimulator::~ParticleMeshSimulator() { fft_util::cleanup_fftw3f(); }

void ParticleMeshSimulator::initialize_simulation(const double redshift,
                                                  const int ntimesteps,
                                                  const int nbods,
                                                  const int ngrid,
                                                  const float gridlength) {
  this->pm_system->setup_system(nbods, ngrid, gridlength);
  this->space_time.setup_time(redshift, ntimesteps);
  this->pm_system->initialize_system(
      -10000.0, 10000.0,
      this->space_time.get_time_scale()); // TODO - better initialization system and user options
}

void ParticleMeshSimulator::advance_single_timestep() {

  double dt = space_time.get_dt();
  this->pm_system->integrate_timestep(static_cast<float>(dt), static_cast<float>(space_time.get_time_current()));
  this->space_time.update_time(dt);

  if (this->space_time.get_time_current() >= this->space_time.get_time_end() ||
      this->space_time.get_time_current() <= this->space_time.get_time_start())
  {
    this->space_time.update_dt(-1.0 * dt);
  }
}

std::array<float, 3>*
ParticleMeshSimulator::get_positions() {
  return this->pm_system->pPos.data();
}

float*
ParticleMeshSimulator::get_mass_density() {
  return this->pm_system->gMass.data();
}

std::vector<float, fftwf_allocator<float>>&
ParticleMeshSimulator::get_mass_density_ref() {
  return this->pm_system->gMass;
}

double ParticleMeshSimulator::get_redshift() {
  return this->space_time.get_redshift_current();
}
