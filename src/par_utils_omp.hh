
#pragma once

#include <vector>
#include <array>
#include <omp.h>


template <typename T>
void assign_vec_par(
	T& vec,
	const float val)
{
    #pragma omp parallel for 
    for (std::size_t i = 0; i < vec.size(); ++i) {
    	vec[i] = val;
    }
}


template <typename T>
void mult_vec_par(T& vec,
	const float val)
{
	#pragma omp parallel for 
  	for (std::size_t i = 0; i < vec.size(); ++i) {
  		vec[i] *= val;
	}
}


template <typename T>
void mult_vec3_par(
	T& vec3,
	const float val)
{
	#pragma omp parallel for 
  	for (std::size_t i = 0; i < vec3.size(); ++i) { 
  		vec3[i][0] *= val;
    	vec3[i][1] *= val;
    	vec3[i][2] *= val;
  	}
}


template <typename T>
void update_particle_par(
	const float const_factor,
	const T& inVec3, T& outVec3)
{
	#pragma omp parallel for 
  	for (std::size_t i = 0; i < inVec3.size(); ++i) { 
  		outVec3[i][0] += inVec3[i][0] * const_factor;
        outVec3[i][1] += inVec3[i][1] * const_factor;
        outVec3[i][2] += inVec3[i][2] * const_factor;
  	}
}


template <typename T, typename U>
void magnitude_vec3_par(
	const float time_scale,
	const T& inVec3,
	U& outVec)
{
	float ts2_inv = 1.0f / (time_scale*time_scale);
	#pragma omp parallel for 
  	for (std::size_t i = 0; i < inVec3.size(); ++i) {
  		outVec[i] = sqrtf( (inVec3[i][0]*inVec3[i][0] + inVec3[i][1]*inVec3[i][1] + inVec3[i][2]*inVec3[i][2]) * ts2_inv );
  	}
}