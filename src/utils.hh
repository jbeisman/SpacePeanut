
#pragma once

#include <algorithm>
#include <execution>
#include <vector>
#include <array>


void initialize_random(
	std::vector<std::array<float,3>>& Vec,
	const float min_value,
	const float max_value);


void write_points(
	const int filenum,
	const std::vector<std::array<float, 3>>& Pos,
	const std::vector<std::array<float, 3>>& Vel,
	const float time_scale);

void write_vtp(
	const int filenum,
	const std::vector<std::array<float, 3>>& Pos,
	const std::vector<std::array<float, 3>>& Vel,
	const float time_scale);

float compute_H(const float a);
int get_max_threads();

