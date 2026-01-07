
#include "particle_mesh_system.hh"
#include "acceleration.hh"
#include "grid_utils.hh"
#include "parallel_utils.hh"
#include "periodic_boundary.hh"
#include "utils.hh"
#include "lambdaCDM.hh"
#include <cmath>

ParticleMeshSystem::ParticleMeshSystem(const int nbods, const int ngrid,
                                       const float gridlength) {
  ParticleMeshSystem::setup_system(nbods, ngrid, gridlength);
}

void ParticleMeshSystem::setup_system(const int nbods, const int ngrid,
                                      const float gridlength) {
  grid = std::make_unique<GridConst>(ngrid, gridlength);
  pPos = std::vector<std::array<float, 3>>(nbods);
  pVel = std::vector<std::array<float, 3>>(nbods);
  pAcc = std::vector<std::array<float, 3>>(nbods);
  gAcc = std::vector<std::array<float, 3>>(ngrid * ngrid * ngrid);
  gPot = std::vector<float, fftwf_allocator<float>>(ngrid * ngrid * ngrid);
  gMass = std::vector<float, fftwf_allocator<float>>(ngrid * ngrid * ngrid);
  fMass = std::vector<fftwf_complex, fftwf_allocator<fftwf_complex>>(ngrid * ngrid * (ngrid / 2 + 1));
  fft_i = std::vector<std::array<short int, 3>>(ngrid * ngrid * (ngrid / 2 + 1));
  grid_i = std::vector<std::array<short int, 3>>(ngrid * ngrid * ngrid);
}

void ParticleMeshSystem::initialize_system(const float vmin, const float vmax,
                                           const float time_scale) {
  // Set plans for FFTW3 foward and backward 3D DFTs
  fft_plans = fft_util::create_plans(grid->NGRID, gMass.data(), gPot.data(), fMass.data());

  // Initialize grid utilities
  GridCIC::set_grid(grid->NGRID, grid->GMAX);
  GridIDX::set_grid(grid->NGRID);

  // Set random position and velocity perturbation
  initialize_random(pPos, 0.0f, grid->GMAX);
  initialize_random(pVel, vmin, vmax);

  // Set pre-formed triplets for iterating with std parallel
  for (short int i = 0; i < grid->NGRID; ++i) {
    for (short int j = 0; j < grid->NGRID; ++j) {

      // fft index (NGRID * NGRID * NGRID_HALF)
      for (short int k = 0; k < grid->NGRID_HALF; ++k) {
        int idx = i * grid->NGRID * grid->NGRID_HALF + j * grid->NGRID_HALF + k;
        fft_i[idx][0] = i;
        fft_i[idx][1] = j;
        fft_i[idx][2] = k;
      }

      // full grid index (NGRID * NGRID * NGRID)
      for (short int k = 0; k < grid->NGRID; ++k) {
        int idx = i * grid->NGRID * grid->NGRID + j * grid->NGRID + k;
        grid_i[idx][0] = i;
        grid_i[idx][1] = j;
        grid_i[idx][2] = k;
      }
    }
  }

  // enforce periodic boundaries and get initial acceleration
  enforce_periodic_boundary_conditions(grid->GMAX, pPos);
  get_acceleration(time_scale);

  // scale_velocity
  float time_scale2 = time_scale * time_scale;
  mult_vec3(pVel, time_scale2);
}

void ParticleMeshSystem::integrate_timestep(const float dt, float time, float speed) {
  const float half_dt = dt / 2;
  float time_scale = exp(time);
  float const_factor = speed * half_dt / compute_H(time_scale);

  // Update velocity to time t + dt/2 using time t acceleration
  timer.start("update_particle_velocity");
  update_particle(const_factor, pAcc, pVel);
  timer.end("update_particle_velocity");

  // Update position to time t + dt using time t + dt/2 velocity
  time += half_dt;
  time_scale = exp(time);
  const_factor = dt / (compute_H(time_scale) * time_scale * time_scale);
  timer.start("update_particle_position");
  update_particle(const_factor, pVel, pPos);
  timer.end("update_particle_position");

  // Enforce periodic boundary conditions
  timer.start("enforce_periodic_boundary_conditions");
  enforce_periodic_boundary_conditions(grid->GMAX, pPos);
  timer.end("enforce_periodic_boundary_conditions");

  // Calculate particle accelerations at time t + dt using positions at time t + dt
  timer.start("get_acceleration");
  get_acceleration(time_scale);
  timer.end("get_acceleration");

  // Update velocity to time t + dt using time t + dt acceleration
  const_factor = speed * half_dt / compute_H(time_scale);
  timer.start("update_particle_velocity");
  update_particle(const_factor, pAcc, pVel);
  timer.end("update_particle_velocity");
}

void ParticleMeshSystem::get_acceleration(const float time_scale) {
  timer.start("compute_grid_mass_distribution");
  compute_grid_mass_distribution(*(grid.get()), pPos, gMass);
  timer.end("compute_grid_mass_distribution", false);

  timer.start("compute_grid_potential");
  compute_grid_potential(*(grid.get()), fft_plans.get(), time_scale, fft_i,
                         fMass, gPot);
  timer.end("compute_grid_potential", false);

  timer.start("compute_grid_acceleration");
  compute_grid_acceleration(*(grid.get()), gPot, grid_i, gAcc);
  timer.end("compute_grid_acceleration", false);

  timer.start("compute_particle_acceleration");
  compute_particle_acceleration(pPos, gAcc, pAcc);
  timer.end("compute_particle_acceleration", false);
}
