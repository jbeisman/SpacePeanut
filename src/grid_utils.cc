
#include "grid_utils.hh"

// CIC initialization
float CIC::CLEN_HALF = 0.0;
float CIC::CLEN_INV  = 0.0;
void CIC::set_grid(int ngrid, float gmax)
{
    float cell_length = gmax / ngrid;
    CIC::CLEN_HALF = cell_length / 2;
    CIC::CLEN_INV  = 1.0 / cell_length; 
}


// IDX initialization
int IDX::NGRID = 0;
void IDX::set_grid(int ngrid) { IDX::NGRID = ngrid; }


// explicit template instantiations
template int grid_util::grid_index_3d_to_1d<32>(int, int, int);
template int grid_util::grid_index_3d_to_1d<64>(int, int, int);
template int grid_util::grid_index_3d_to_1d<128>(int, int, int);
template int grid_util::grid_index_3d_to_1d<256>(int, int, int);
template int grid_util::grid_index_3d_to_1d<512>(int, int, int);
template int grid_util::grid_index_3d_to_1d<1024>(int, int, int);
