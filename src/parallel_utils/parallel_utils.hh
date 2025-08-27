
#pragma once

#include <vector>
#include <array>

// Simple parallel utilities

// Declarations of user-facing functions
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

#include "par_utils_impl.hh"
