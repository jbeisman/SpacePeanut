
#include "space_time.hh"
#include <cmath>

void SpaceTime::setup_time(double rdshft, int nstp) {
  this->redshft_z = 1.0 / (rdshft + 1.0);
  this->nsteps = nstp;
  this->time = std::log(this->redshft_z);
  this->t0 = std::log(1.0);
  this->dt = (this->t0 - this->time) / this->nsteps;
}

double SpaceTime::get_time_current() const { return this->time; }
double SpaceTime::get_time_start() const { return std::log(this->redshft_z); }
double SpaceTime::get_time_end() const { return this->t0; }
double SpaceTime::get_dt() const { return this->dt; }
double SpaceTime::get_time_scale() const { return std::exp(this->time); }
double SpaceTime::get_redshift_current() const { return 1.0 / std::exp(this->time) - 1.0; }
void SpaceTime::update_time(double dt) { this->time += dt; }
void SpaceTime::update_dt(double new_dt) { this->dt = new_dt; }
