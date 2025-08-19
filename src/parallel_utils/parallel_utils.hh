
#pragma once

#include <vector>
#include <array>

// simple parallel utilities

// declarations of user-facing functions
// Assign val to every element in vec 
template <typename T>
void assign_vec(
	T& vec,
	const float val);

// Multiply every element in vec with val
template <typename T>
void mult_vec(
	T& vec,
	const float val);

// Multiply every element in vec with val
// hardcoded for std::vector<array[3]> types
template <typename T>
void mult_vec3(
	T& vec3,
	const float val);

// Particle update
// For every element invec += outvec * const_factor
template <typename T>
void update_particle(
	const float const_factor,
	const T& inVec3,
	T& outVec3);

// Magnitude of vector quantity 
template <typename T, typename U>
void magnitude_vec3(
	const float time_scale,
	const T& inVec3,
	U& outVec);





// declarations of backend interface functions
template <typename T>
void assign_vec_par(
	T& vec,
	const float val);

template <typename T>
void mult_vec_par(
	T& vec,
	const float val);

template <typename T>
void mult_vec3_par(
	T& vec3,
	const float val);

template <typename T>
void update_particle_par(
	const float const_factor,
	const T& inVec3,
	T& outVec3);

template <typename T, typename U>
void magnitude_vec3_par(
	const float time_scale,
	const T& inVec3,
	U& outVec);




// definitions of user-facing functions
template <typename T>
void assign_vec(
	T& vec,
	const float val)
{
	assign_vec_par(vec, val);
}

template <typename T>
void mult_vec(
	T& vec,
	const float val)
{
	mult_vec_par(vec, val);
}

template <typename T>
void mult_vec3(
	T& vec3,
	const float val)
{
	mult_vec3_par(vec3, val);
}

template <typename T>
void update_particle(
	const float const_factor,
	const T& inVec3,
	T& outVec3)
{
	update_particle_par(const_factor, inVec3, outVec3);
}

template <typename T, typename U>
void magnitude_vec3(
	const float time_scale,
	const T& inVec3,
	U& outVec)
{
	magnitude_vec3_par(time_scale, inVec3, outVec);
}


// definitions of backend functions included here
#if defined(USE_STD_PARALLEL)
#include "par_utils_std.hh"
#elif defined(USE_OPENMP)
#include "par_utils_omp.hh"
#endif
