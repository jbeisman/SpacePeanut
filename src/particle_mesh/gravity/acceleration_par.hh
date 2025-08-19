
#pragma once

#include <vector>
#include <array>
#include "fftw3f_allocator.hh"


/*
These function declarations serve as interfaces to the parallel backend.
The function defintions are implemented in separate source files for each backend.
*/

void compute_grid_mass_distribution_par(
	const float CVOL_INV,
	const std::vector<std::array<float,3> >& pPos,
	float *gMD);

void compute_grid_potential_par(
	const int NG,
	const int NGH,
	const float kfac,
	const std::vector<std::array<short int,3> >& fft_i,
	fftwf_complex *fMass);

void compute_grid_acceleration_par(
	const int NG,
	const float CLEN,
	const std::vector<float, fftwf_allocator<float> >& gPot,
	const std::vector<std::array<short int,3> >& grid_i,
	std::vector<std::array<float, 3> >& gAcc);

void compute_particle_acceleration_par(
	const std::vector<std::array<float,3> >& pPos,
	const std::vector<std::array<float,3> >& gAcc,
	std::vector<std::array<float,3> >& pAcc);
