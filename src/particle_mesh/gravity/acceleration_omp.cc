
#include "acceleration_kernels.hh"
#include "acceleration_par.hh"
#include <array>
#include <omp.h>
#include <vector>

void compute_grid_mass_distribution_par(
    const float CVOL_INV, const std::vector<std::array<float, 3>> &pPos,
    float *gMD) {
#pragma omp parallel for
  for (std::size_t i = 0; i < pPos.size(); i++) {
    kernel_grid_mass_CIC(CVOL_INV, pPos[i], gMD);
  }
}

void compute_grid_potential_par(
    const int NG, const int NGH, const float kfac,
    const std::vector<std::array<short int, 3>> &fft_i, fftwf_complex *fMass) {
#pragma omp parallel for
  for (int i = 0; i < NG; ++i) {
    for (int j = 0; j < NG; ++j) {
      for (int k = 0; k < NGH; ++k) {
        kernel_grid_potential(NG, NGH, kfac, i, j, k, fMass);
      }
    }
  }
}

void compute_grid_acceleration_par(
    const int NG, const float CLEN,
    const std::vector<float, fftwf_allocator<float>> &gPot,
    const std::vector<std::array<short int, 3>> &grid_i,
    std::vector<std::array<float, 3>> &gAcc) {
  const auto *gP = gPot.data();
  auto *gA = gAcc.data();
  const float dnom = 1.0 / (CLEN * 12.0);

#pragma omp parallel for
  for (int i = 0; i < NG; ++i) {
    for (int j = 0; j < NG; ++j) {
      for (int k = 0; k < NG; ++k) {
        kernel_grid_accel(i, j, k, dnom, gP, gA[i * NG * NG + j * NG + k]);
      }
    }
  }
}

void compute_particle_acceleration_par(
    const std::vector<std::array<float, 3>> &pPos,
    const std::vector<std::array<float, 3>> &gAcc,
    std::vector<std::array<float, 3>> &pAcc) {
  const auto *gA = gAcc.data();
  auto *pA = pAcc.data();

#pragma omp parallel for
  for (std::size_t i = 0; i < pPos.size(); i++) {
    kernel_part_accel_CIC(pPos[i], gA, pA[i]);
  }
}
