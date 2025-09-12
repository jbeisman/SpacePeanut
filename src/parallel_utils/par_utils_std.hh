
#pragma once

#include <algorithm>
#include <array>
#include <execution>
#include <utility>
#include <vector>

template <typename T> void assign_vec_par(T &vec, const float val) {
  std::fill(std::execution::par_unseq, vec.begin(), vec.end(), val);
}

template <typename T> void mult_vec_par(T &vec, const float val) {
  std::transform(std::execution::par_unseq, vec.begin(), vec.end(), vec.begin(),
                 [val](auto el) {
                   el *= val;
                   return el;
                 });
}

template <typename T> void mult_vec3_par(T &vec3, const float val) {
  std::transform(std::execution::par_unseq, vec3.begin(), vec3.end(),
                 vec3.begin(), [val](auto v) {
                   v[0] *= val;
                   v[1] *= val;
                   v[2] *= val;
                   return v;
                 });
}

template <typename T>
void update_particle_par(const float const_factor, const T &inVec3,
                         T &outVec3) {
  std::transform(std::execution::par_unseq, inVec3.begin(), inVec3.end(),
                 outVec3.begin(), outVec3.begin(),
                 [const_factor](const auto &in, auto &out) {
                   out[0] += in[0] * const_factor;
                   out[1] += in[1] * const_factor;
                   out[2] += in[2] * const_factor;
                   return out;
                 });
}

template <typename T, typename U>
void magnitude_vec3_par(const float time_scale, const T &inVec3, U &outVec) {
  float ts2_inv = 1.0f / (time_scale * time_scale);
  std::transform(std::execution::par_unseq, inVec3.begin(), inVec3.end(),
                 outVec.begin(), [ts2_inv](const auto &vel) {
                   return sqrtf(
                       (vel[0] * vel[0] + vel[1] * vel[1] + vel[2] * vel[2]) *
                       ts2_inv);
                 });
}

template <typename T>
std::pair<float, float> minmax_vec_elems_par(const T &inVec) {
  auto minmax = std::minmax_element(std::execution::par_unseq, inVec.begin(),
                                    inVec.end());
  float min_val = *minmax.first;
  float max_val = *minmax.second;

  return std::make_pair(min_val, max_val);
}
