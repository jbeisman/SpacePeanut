
#include "periodic_boundary.hh"
#include "periodic_boundary_impl.hh"

// Periodic wrap for particle positions
void enforce_periodic_boundary_conditions(
	const float& GMAX,
	std::vector<std::array<float, 3> >& pPos)
{
	enforce_periodic_boundary_conditions_par(GMAX, pPos);
}