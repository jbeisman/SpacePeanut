
#pragma once

#include <array>
#include <cmath>
#include <tuple>

namespace grid_util {

// Bit-hack index conversion from 3D to 1D
template <int N> inline int grid_index_3d_to_1d(int i, int j, int k) {
  // Periodic wrap-around for i,j,k that are outside of 0..N
  // Valid only for N == power of two
  // Assumes row-major storage
  constexpr int MASK = N - 1;
  return ((i + N) & MASK) * N * N + ((j + N) & MASK) * N + ((k + N) & MASK);
}

// Runtime version used when N != power of two
inline int grid_index_3d_to_1d_runtime(int i, int j, int k, int N) {
  return ((i + N) % N) * N * N + ((j + N) % N) * N + ((k + N) % N);
}

} // namespace grid_util

inline auto GridCIC::get_weights_and_indices(const std::array<float, 3> &pPos) {
  // Assumes pPos represents a point at the center of a mass cloud with width
  // CLEN operate on minimum coords of mass cloud
  float lX = pPos[0] - GridCIC::CLEN_HALF;
  float lY = pPos[1] - GridCIC::CLEN_HALF;
  float lZ = pPos[2] - GridCIC::CLEN_HALF;

  // integer index into grid
  // std::floor ensures correct index if position is negative
  int iX = static_cast<int>(std::floor(lX * GridCIC::CLEN_INV));
  int iY = static_cast<int>(std::floor(lY * GridCIC::CLEN_INV));
  int iZ = static_cast<int>(std::floor(lZ * GridCIC::CLEN_INV));

  // weights for index i+1,j+1,k+1 contributions
  float nXX = lX * GridCIC::CLEN_INV - static_cast<float>(iX);
  float nYY = lY * GridCIC::CLEN_INV - static_cast<float>(iY);
  float nZZ = lZ * GridCIC::CLEN_INV - static_cast<float>(iZ);

  // weights for index i,j,k contributions
  float nX = 1.0 - nXX;
  float nY = 1.0 - nYY;
  float nZ = 1.0 - nZZ;

  return std::make_tuple(nX, nY, nZ, nXX, nYY, nZZ, iX, iY, iZ);
}

inline int GridIDX::flatten(int i, int j, int k) {
  // Flatten 3D index from NxNxN grid into 1D linear index
  // grid_index_3d_to_1d<N> must be explicitly instantiated for each N listed
  // here
  switch (GridIDX::NGRID) {
  case 32:
    return grid_util::grid_index_3d_to_1d<32>(i, j, k);
  case 64:
    return grid_util::grid_index_3d_to_1d<64>(i, j, k);
  case 128:
    return grid_util::grid_index_3d_to_1d<128>(i, j, k);
  case 256:
    return grid_util::grid_index_3d_to_1d<256>(i, j, k);
  case 512:
    return grid_util::grid_index_3d_to_1d<512>(i, j, k);
  case 1024:
    return grid_util::grid_index_3d_to_1d<1024>(i, j, k);
  default:
    return grid_util::grid_index_3d_to_1d_runtime(i, j, k, GridIDX::NGRID);
  }
}
