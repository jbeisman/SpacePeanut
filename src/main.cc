
#include "simulator.hh"


int main() {

ParticleMeshSimulator pm_sim = ParticleMeshSimulator();
pm_sim.initialize_simulation(50.0, 1000, 32768, 64, 16.0);
pm_sim.advance_simulation();

}
