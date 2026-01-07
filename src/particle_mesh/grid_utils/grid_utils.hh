
#pragma once
#include <array>

// Compute and store constant grid information
struct GridConst {
  GridConst(int ngrid, float gmax);
  GridConst() = default;
  int NGRID;      // number of cells in 1D - N from NxNxN
  float GMAX;     // grid 1D length
  float CLEN;     // cell length
  int NGRID_HALF; // half-ish of NGRID  --  NGRID / 2 + 1
};

// Cloud-in-cell particle <--> grid
class GridCIC {
  static float CLEN_INV;
  static float CLEN_HALF;

public:
  // Set grid for cloud-in-cell computation
  static void set_grid(int ngrid, float gmax);
  // Get cloud-in-cell weights and indices given a particle's position
  static auto get_weights_and_indices(const std::array<float, 3> &pPos);
};

// Grid indexing support
class GridIDX {
  static int NGRID;

public:
  // Set grid for index conversion
  static void set_grid(int ngrid);
  // Convert 3D index to 1D with periodic wrapping
  static int flatten(int i, int j, int k);
};

#include "grid_utils_impl.hh"
