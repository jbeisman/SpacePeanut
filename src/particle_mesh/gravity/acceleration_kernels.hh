
#pragma once

#include <array>
#include <utility>
#include <limits>
#include "grid_utils.hh"


/*
Acceleration kernel code that gets called from inner loops.
*/

// Cloud-in-cell mass density distribution
inline void kernel_grid_mass_CIC(
	float CVOL_INV,
	const std::array<float,3>& pos,
	float* gMD)
{	
	// Get cloud-in-cell weights (6 float) and indices (3 int) 
	auto [fX, fY, fZ, fXX, fYY, fZZ, iX, iY, iZ] = GridCIC::get_weights_and_indices(pos);
	// Distribute particle mass to 8 grid cells in stencil
	// 10.0 represents constant mass   TODO -- improve mass handling 
	gMD[GridIDX::flatten(iX,   iY,   iZ  )] += (10.0 * fX  * fY  * fZ  * CVOL_INV);
	gMD[GridIDX::flatten(iX+1, iY,   iZ  )] += (10.0 * fXX * fY  * fZ  * CVOL_INV);
	gMD[GridIDX::flatten(iX,   iY+1, iZ  )] += (10.0 * fX  * fYY * fZ  * CVOL_INV);
	gMD[GridIDX::flatten(iX,   iY,   iZ+1)] += (10.0 * fX  * fY  * fZZ * CVOL_INV);
	gMD[GridIDX::flatten(iX+1, iY+1, iZ  )] += (10.0 * fXX * fYY * fZ  * CVOL_INV);
	gMD[GridIDX::flatten(iX+1, iY,   iZ+1)] += (10.0 * fXX * fY  * fZZ * CVOL_INV);
	gMD[GridIDX::flatten(iX,   iY+1, iZ+1)] += (10.0 * fX  * fYY * fZZ * CVOL_INV);
	gMD[GridIDX::flatten(iX+1, iY+1, iZ+1)] += (10.0 * fXX * fYY * fZZ * CVOL_INV);
}



// Solve Poisson's equation for gravitational potential in Fourier Space
inline void kernel_grid_potential(
	int NG,
	int NGH,
	float kfac,
	int i,
	int j,
	int k,
	fftwf_complex* fMass)
{
	// Frequency domain requires indices to fall into [-NGRID/2, NGRID/2]
	// Manipulation of k index not required due to FFTW idiosyncrasies 
	int wX = (i < NGH) ? i : i - NG;
	int wY = (j < NGH) ? j : j - NG;

	// Compute wave numbers
	float k_squared = (wX*wX + wY*wY + k*k) * kfac;
	float k_squared_inv = (k_squared < std::numeric_limits<float>::epsilon()) ? 0.0f : 1.0f / k_squared;
	
	// Compute potential in Fourier space
	int fft_idx = i*NG*NGH + j*NGH + k;
	fMass[fft_idx][0] *= k_squared_inv;
	fMass[fft_idx][1] *= k_squared_inv;
}


// Calculate grid acceleration from gradient of potential via 4th-order central finite difference
inline void kernel_grid_accel(
	int i,
	int j,
	int k,
	float dnom,
	const float* gP,
	std::array<float, 3>& gA)
{
	gA[0] = dnom * (gP[GridIDX::flatten(i-2,j,k)] - 8*gP[GridIDX::flatten(i-1,j,k)] + 8*gP[GridIDX::flatten(i+1,j,k)] - gP[GridIDX::flatten(i+2,j,k)]);
	gA[1] = dnom * (gP[GridIDX::flatten(i,j-2,k)] - 8*gP[GridIDX::flatten(i,j-1,k)] + 8*gP[GridIDX::flatten(i,j+1,k)] - gP[GridIDX::flatten(i,j+2,k)]);
	gA[2] = dnom * (gP[GridIDX::flatten(i,j,k-2)] - 8*gP[GridIDX::flatten(i,j,k-1)] + 8*gP[GridIDX::flatten(i,j,k+1)] - gP[GridIDX::flatten(i,j,k+2)]);
}


// Cloud-in-cell interpolation of acceleration onto particles
inline void kernel_part_accel_CIC(
	const std::array<float,3>& pos,
	const std::array<float,3>* gA,
	std::array<float, 3>& pA)
{
	// Get cloud-in-cell weights (6 float) and indices (3 int) 
	auto [fX, fY, fZ, fXX, fYY, fZZ, iX, iY, iZ] = GridCIC::get_weights_and_indices(pos);
	// Collect accelerations from 8 grid cells in stencil
	for (int j = 0; j < 3; j++) {
		pA[j]  = gA[GridIDX::flatten(iX,   iY,   iZ  )][j] * fX  * fY  * fZ;
		pA[j] += gA[GridIDX::flatten(iX+1, iY,   iZ  )][j] * fXX * fY  * fZ;
		pA[j] += gA[GridIDX::flatten(iX,   iY+1, iZ  )][j] * fX  * fYY * fZ;
		pA[j] += gA[GridIDX::flatten(iX,   iY,   iZ+1)][j] * fX  * fY  * fZZ;
		pA[j] += gA[GridIDX::flatten(iX+1, iY+1, iZ  )][j] * fXX * fYY * fZ;
		pA[j] += gA[GridIDX::flatten(iX+1, iY,   iZ+1)][j] * fXX * fY  * fZZ;
		pA[j] += gA[GridIDX::flatten(iX,   iY+1, iZ+1)][j] * fX  * fYY * fZZ;
		pA[j] += gA[GridIDX::flatten(iX+1, iY+1, iZ+1)][j] * fXX * fYY * fZZ;
	}
}

