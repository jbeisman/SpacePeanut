
#include <vector>
#include <array>
#include <algorithm>
#include <execution>
#include "acceleration_par.hh"
#include "acceleration_kernels.hh"


void compute_grid_mass_distribution_par(
	const float CVOL_INV,
	const std::vector<std::array<float,3> >& pPos,
	float *gMD)
{
    std::for_each(std::execution::par_unseq, pPos.begin(), pPos.end(),
    [CVOL_INV, gMD](const auto& pos) {
    	kernel_grid_mass_CIC(CVOL_INV, pos, gMD);
	});
}


void compute_grid_potential_par(
	const int NG,
	const int NGH,
	const float kfac,
	const std::vector<std::array<short int,3> >& fft_i,
	fftwf_complex *fMass)
{
	std::for_each(std::execution::par_unseq, fft_i.begin(), fft_i.end(),
	[NG,NGH,kfac,fMass](const auto& ijk) {
		auto [i,j,k] = ijk;
		kernel_grid_potential(NG, NGH, kfac, static_cast<int>(i), static_cast<int>(j), static_cast<int>(k), fMass);
	});
}


void compute_grid_acceleration_par(
	const int NG,
	const float CLEN,
	const std::vector<float, fftwf_allocator<float> >& gPot,
	const std::vector<std::array<short int,3> >& grid_i,
	std::vector<std::array<float, 3> >& gAcc)
{
	const auto *gP = gPot.data();
	const float dnom = 1.0 / (CLEN * 12.0);
	
	std::transform(std::execution::par_unseq, grid_i.begin(), grid_i.end(), gAcc.begin(), gAcc.begin(),
	[dnom,gP](const auto& ijk, auto& gA)
	{
		auto [i,j,k] = ijk;
    	kernel_grid_accel(static_cast<int>(i), static_cast<int>(j), static_cast<int>(k), dnom, gP, gA);
    	return gA;
	});
}


void compute_particle_acceleration_par(
	const std::vector<std::array<float,3> >& pPos,
	const std::vector<std::array<float,3> >& gAcc,
	std::vector<std::array<float,3> >& pAcc)
{
	const auto *gA = gAcc.data();
    std::transform(std::execution::par_unseq, pPos.begin(), pPos.end(), pAcc.begin(), pAcc.begin(), [gA](const auto& pos, auto& pA) {
    	kernel_part_accel_CIC(pos, gA, pA);
    	return pA;
	});
}

