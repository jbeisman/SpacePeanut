
#pragma once

#include <vector>
#include <array>
#include "fftw3f_util.hh"
#include "fftw3f_allocator.hh"
#include "grid_constants.hh"
#include "time_logger.hh"

class ParticleMeshSystem {
public:
	ParticleMeshSystem(const int nbods, const int ngrid, const float gridlength);
	ParticleMeshSystem() = delete;
	~ParticleMeshSystem();
	void initialize_system(const float vmin, const float vmax, const float time_scale);
	void integrate_timestep(const float dt, float time);

	TimeLogger timer;													// timer to log performance data
	GridConst grid;														// grid metadata
	std::vector<std::array<float, 3>> pPos;								// particle positions
	std::vector<std::array<float, 3>> pVel;								// particle velocities
	std::vector<std::array<float, 3>> pAcc;								// particle accelerations
	std::vector<std::array<float, 3>> gAcc;								// grid acceleration
private:
	void get_acceleration(const float time_scale);						// calculate long-range acceleration using the PM method

	std::vector<float, fftwf_allocator<float>> gPot; 					// grid gravitational potential
	std::vector<float, fftwf_allocator<float>> gMass; 					// grid mass distribution
	std::vector<fftwf_complex, fftwf_allocator<fftwf_complex>> fMass; 	// fftw3 float32 complex type - used in DFTs
	std::vector<std::array<short int,3>> fft_i;							// triple short int index for fft  (NGRID * NGRID * NGRID_HALF)
	std::vector<std::array<short int,3>> grid_i;						// triple short int index for grid (NGRID * NGRID * NGRID)
	std::unique_ptr<fft_util::Plans> fft_plans;							// struct containing fftw3 plans
};
