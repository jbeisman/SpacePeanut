
// g++ -O3 -ffast-math -fopenmp -std=c++23 -DUSE_OPENMP -o test  fftw3f_interface.cc time_logger.cc get_input.cc utils.cc grid_utils.cc periodic_boundary_omp.cc periodic_boundary.cc acceleration_omp.cc acceleration.cc particle_mesh_system.cc simulator.cc main.cc -lfftw3f_omp -lfftw3f -fopenmp -lm
// g++ -O3 -ffast-math  -std=c++23 -DUSE_STD_PARALLEL -o test  fftw3f_interface.cc time_logger.cc get_input.cc utils.cc grid_utils.cc periodic_boundary_std.cc periodic_boundary.cc acceleration_std.cc acceleration.cc particle_mesh_system.cc simulator.cc main.cc -lfftw3f_threads -lfftw3f -ltbb -lm

#include "simulator.hh"


int main() {

ParticleMeshSimulator pm_sim = ParticleMeshSimulator();
pm_sim.initialize_simulation();
pm_sim.advance_simulation();

}
