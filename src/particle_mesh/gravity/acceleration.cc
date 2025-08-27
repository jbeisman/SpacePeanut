
#include <chrono>
#include <iostream>
#include <numbers>
#include "acceleration.hh"
#include "acceleration_par.hh"
#include "parallel_utils.hh"


// Gravitational constant - used locally
// TODO - unnamed namespace instead of static?
static inline consteval float G() {
	constexpr double unit_length = 3.08568e+24;
	constexpr double unit_time = 3.08568e+19;
	constexpr double unit_mass = 1.98841e+43;
	return static_cast<float>(6.67430e-8 * unit_mass * unit_time * unit_time
							 / (unit_length * unit_length * unit_length) );
}


void compute_grid_mass_distribution(
	const GridConst& grid,
	const std::vector<std::array<float, 3>>& pPos,
	std::vector<float, fftwf_allocator<float>>& gMass)
{
	const float CVOL_INV = 1.0 / (grid.CLEN * grid.CLEN * grid.CLEN);

	// Set grid mass to 0.0
	assign_vec(gMass, 0.0f);
	// Distribute particle mass onto grid
	compute_grid_mass_distribution_par(CVOL_INV, pPos, gMass.data());
}


void compute_grid_potential(
	const GridConst& grid,
	const fft_util::Plans *fft_plans,
	const float time_scale,
	const std::vector<std::array<short int,3>>& fft_i,
	std::vector<fftwf_complex, fftwf_allocator<fftwf_complex>>& fMass,
	std::vector<float, fftwf_allocator<float>>& gPot)
{
	const int NG = grid.NGRID;
    const int NGH = grid.NGRID_HALF;

	// Set potential to 0.0
	assign_vec(gPot, 0.0f);
    
    // Transform density field to Fourier space with forward DFT
    fftwf_execute(fft_plans->r2c_plan);
 
	// Compute potential field by solving Poisson's equation in Fourier space
	const float kfac = std::numbers::pi_v<float> * std::numbers::pi_v<float> * 4.0 / (grid.GMAX * grid.GMAX);
	compute_grid_potential_par(NG, NGH, kfac, fft_i, fMass.data());

    // Transform potential field to real space with inverse DFT
    fftwf_execute(fft_plans->c2r_plan);

    // Normalize the potential field after inverse DFT
    const float normfactor = 4.0 * G() * std::numbers::pi_v<float> / (time_scale * (float)(NG * NG * NG));
    mult_vec(gPot, normfactor);
}

void compute_grid_acceleration(
	const GridConst& grid,
	const std::vector<float, fftwf_allocator<float>>& gPot,
	const std::vector<std::array<short int,3>>& g_i,
	std::vector<std::array<float, 3>>& gAcc)
{
	const int NG = grid.NGRID;
	const float CLEN = grid.CLEN;
	
	// Calculate grid acceleration from gradient of grid potential
	compute_grid_acceleration_par(NG, CLEN, gPot, g_i, gAcc);
}


void compute_particle_acceleration(
	const std::vector<std::array<float, 3>>& pPos,
	const std::vector<std::array<float, 3>>& gAcc,
	std::vector<std::array<float, 3>>& pAcc)
{
	// Interpolate acceleration onto particles
	compute_particle_acceleration_par(pPos, gAcc, pAcc);
}

