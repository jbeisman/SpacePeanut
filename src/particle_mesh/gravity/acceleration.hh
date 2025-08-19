
#pragma once

#include <vector>
#include <array>
#include "grid_constants.hh"
#include "fftw3f_util.hh"
#include "fftw3f_allocator.hh"


/*
These functions compute particle accleration as a 4-step process.
1. compute_grid_mass_distribution() distributes particles masses onto the grid.
2. compute_grid_potential() computes the grid gravitational potential field from the grid mass distribution.
3. compute_grid_acceleration() calculates the grid acceleration field from the grid potential field.
4. compute_particle_acceleration() interpolates the grid acceleration field onto the particles.
*/
void compute_grid_mass_distribution(
	const GridConst& grid,
	const std::vector<std::array<float, 3>>& pPos,
	std::vector<float, fftwf_allocator<float>>& gMass);


void compute_grid_potential(
	const GridConst& grid,
	const fft_util::Plans *fft_plans,
	const float time_scale,
	const std::vector<std::array<short int,3>>& fft_i,
	std::vector<fftwf_complex, fftwf_allocator<fftwf_complex>>& fMass,
	std::vector<float, fftwf_allocator<float>>& gPot);


void compute_grid_acceleration(
	const GridConst& grid,
	const std::vector<float, fftwf_allocator<float>>& gPot,
	const std::vector<std::array<short int,3>>& grid_i,
	std::vector<std::array<float, 3>>& gAcc);


void compute_particle_acceleration(
	const std::vector<std::array<float, 3>>& pPos,
	const std::vector<std::array<float, 3>>& gAcc,
	std::vector<std::array<float, 3>>& pAcc);
