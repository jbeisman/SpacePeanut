
#include "grid_utils.hh"

// grid constants initialization
GridConst::GridConst(int ngrid, float gmax)
    : NGRID(ngrid), GMAX(gmax), CLEN(gmax / ngrid), NGRID_HALF(ngrid / 2 + 1) {}

// cloud-in-cell initialization
float GridCIC::CLEN_HALF = 0.0f;
float GridCIC::CLEN_INV = 0.0f;
void GridCIC::set_grid(int ngrid, float gmax) {
  float cell_length = gmax / ngrid;
  GridCIC::CLEN_HALF = cell_length / 2.0f;
  GridCIC::CLEN_INV = 1.0f / cell_length;
}

// grid index initialization
int GridIDX::NGRID = 0;
void GridIDX::set_grid(int ngrid) { GridIDX::NGRID = ngrid; }

