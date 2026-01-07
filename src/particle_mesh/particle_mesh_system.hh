
#pragma once

#include "fftw3f_allocator.hh"
#include "fftw3f_util.hh"
#include "grid_utils.hh"
#include "time_logger.hh"
#include <array>
#include <vector>

class ParticleMeshSystem {
public:
  ParticleMeshSystem() = default;
  ParticleMeshSystem(const int nbods, const int ngrid, const float gridlength);
  void setup_system(const int nbods, const int ngrid, const float gridlength);
  void initialize_system(const float vmin, const float vmax,
                         const float time_scale);
  void integrate_timestep(const float dt, float time, float speed=1.0f);

  TimeLogger timer;                                 // timer to log performance data
  std::vector<std::array<float, 3>> pPos;           // particle positions
  std::vector<std::array<float, 3>> pVel;           // particle velocities
  std::vector<std::array<float, 3>> pAcc;           // particle accelerations
  std::vector<std::array<float, 3>> gAcc;           // grid acceleration
  std::vector<float, fftwf_allocator<float>> gMass; // grid mass distribution
private:
  // Calculate long-range acceleration using the PM method
  void get_acceleration(const float time_scale);

  std::vector<float, fftwf_allocator<float>> gPot;                  // grid gravitational potential
  std::vector<fftwf_complex, fftwf_allocator<fftwf_complex>> fMass; // fftw3 float32 complex type - used in DFTs
  std::vector<std::array<short int, 3>> fft_i;                      // triple short int index for fft  (NGRID * NGRID * NGRID_HALF)
  std::vector<std::array<short int, 3>> grid_i;                     // triple short int index for grid (NGRID * NGRID * NGRID)
  std::unique_ptr<fft_util::Plans> fft_plans;                       // struct containing fftw3 plans
  GridConst grid;                                                   // grid metadata
};
