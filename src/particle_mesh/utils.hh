
#pragma once

#include <algorithm>
#include <execution>
#include <vector>
#include <array>


void initialize_random(
	std::vector<std::array<float,3>>& Vec,
	const float min_value,
	const float max_value);


float compute_H(const float a);

