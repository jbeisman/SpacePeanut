
#pragma once

#include <array>
#include <limits>
#include <omp.h>
#include <utility>
#include <vector>

template <typename T> void assign_vec_par(T &vec, const float val) {
#pragma omp parallel for
  for (std::size_t i = 0; i < vec.size(); ++i) {
    vec[i] = val;
  }
}

template <typename T> void mult_vec_par(T &vec, const float val) {
#pragma omp parallel for
  for (std::size_t i = 0; i < vec.size(); ++i) {
    vec[i] *= val;
  }
}

template <typename T> void mult_vec3_par(T &vec3, const float val) {
#pragma omp parallel for
  for (std::size_t i = 0; i < vec3.size(); ++i) {
    vec3[i][0] *= val;
    vec3[i][1] *= val;
    vec3[i][2] *= val;
  }
}

template <typename T>
void update_particle_par(const float const_factor, const T &inVec3,
                         T &outVec3) {
#pragma omp parallel for
  for (std::size_t i = 0; i < inVec3.size(); ++i) {
    outVec3[i][0] += inVec3[i][0] * const_factor;
    outVec3[i][1] += inVec3[i][1] * const_factor;
    outVec3[i][2] += inVec3[i][2] * const_factor;
  }
}

template <typename T, typename U>
void magnitude_vec3_par(const float time_scale, const T &inVec3, U &outVec) {
  float ts2_inv = 1.0f / (time_scale * time_scale);
#pragma omp parallel for
  for (std::size_t i = 0; i < inVec3.size(); ++i) {
    outVec[i] =
        sqrtf((inVec3[i][0] * inVec3[i][0] + inVec3[i][1] * inVec3[i][1] +
               inVec3[i][2] * inVec3[i][2]) *
              ts2_inv);
  }
}

template <typename T>
std::pair<float, float> minmax_vec_elems_par(const T &inVec) {

  float min_val = std::numeric_limits<float>::max(); // Initialize min_val to a
                                                     // very large number
  float max_val = std::numeric_limits<float>::min(); // Initialize max_val to a
                                                     // very small number

#pragma omp parallel for reduction(min : min_val) reduction(max : max_val)
  for (size_t i = 0; i < inVec.size(); ++i) {
    if (inVec[i] < min_val) {
      min_val = inVec[i];
    }
    if (inVec[i] > max_val) {
      max_val = inVec[i];
    }
  }

  return std::make_pair(min_val, max_val);
}
