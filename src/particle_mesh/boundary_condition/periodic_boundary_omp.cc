
#include <vector>
#include <array>
#include <omp.h>
#include "periodic_boundary_impl.hh"


void enforce_periodic_boundary_conditions_par(
	const float& GMAX,
	std::vector<std::array<float, 3>>& pPos)
{
    #pragma omp parallel for 
  	for (std::size_t i = 0; i < pPos.size(); ++i)
  	{ 
		pPos[i][0] = boundary_condition(GMAX, pPos[i][0]);
		pPos[i][1] = boundary_condition(GMAX, pPos[i][1]);
		pPos[i][2] = boundary_condition(GMAX, pPos[i][2]);
  	}
}

