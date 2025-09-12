
#include "grid_utils.hh"

// grid constants initialization
GridConst::GridConst(int ngrid, float gmax)
    : NGRID(ngrid), GMAX(gmax), CLEN(gmax / ngrid), NGRID_HALF(ngrid / 2 + 1) {}

// cloud-in-cell initialization
float GridCIC::CLEN_HALF = 0.0;
float GridCIC::CLEN_INV = 0.0;
void GridCIC::set_grid(int ngrid, float gmax) {
  float cell_length = gmax / ngrid;
  GridCIC::CLEN_HALF = cell_length / 2;
  GridCIC::CLEN_INV = 1.0 / cell_length;
}

// grid index initialization
int GridIDX::NGRID = 0;
void GridIDX::set_grid(int ngrid) { GridIDX::NGRID = ngrid; }

// explicit grid index template instantiations
template int grid_util::grid_index_3d_to_1d<32>(int, int, int);
template int grid_util::grid_index_3d_to_1d<64>(int, int, int);
template int grid_util::grid_index_3d_to_1d<128>(int, int, int);
template int grid_util::grid_index_3d_to_1d<256>(int, int, int);
template int grid_util::grid_index_3d_to_1d<512>(int, int, int);
template int grid_util::grid_index_3d_to_1d<1024>(int, int, int);
