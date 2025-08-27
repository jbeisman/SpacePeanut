
#include <vector>
#include <array>
#include <algorithm>
#include <execution>
#include "periodic_boundary_impl.hh"


void enforce_periodic_boundary_conditions_par(
	const float& GMAX,
	std::vector<std::array<float, 3>>& pPos)
{


	std::for_each(std::execution::par_unseq, pPos.begin(), pPos.end(), [GMAX](auto& pos) {
		pos[0] = boundary_condition(GMAX, pos[0]);
		pos[1] = boundary_condition(GMAX, pos[1]);
		pos[2] = boundary_condition(GMAX, pos[2]);
	});
}

