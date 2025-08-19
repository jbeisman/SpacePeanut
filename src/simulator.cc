
#include <iostream>
#include "fftw3f_util.hh"
#include "utils.hh"
#include "get_input.hh"
#include "simulator.hh"
	
ParticleMeshSimulator::ParticleMeshSimulator()
{ 
	fft_util::init_fftw3f();
	auto [npart, ngrid, nstep, gmax, redshift] = get_input();
	pm_system = std::make_unique<ParticleMeshSystem>(npart, ngrid, gmax);
	space_time = std::make_unique<CosmicTime>(redshift, nstep);
}


ParticleMeshSimulator::~ParticleMeshSimulator()
{ 
	fft_util::cleanup_fftw3f();
}


void ParticleMeshSimulator::initialize_simulation()
{
	float time_scale = space_time->get_time_scale();
	pm_system->initialize_system(-100.0, 100.0, time_scale);
	write_points(filenum++, pm_system->pPos, pm_system->pVel, time_scale);
}


void ParticleMeshSimulator::advance_single_timestep()
{
	float dt = space_time->get_dt();
	pm_system->integrate_timestep(dt, space_time->get_time());
	space_time->update_time(dt);
}


void ParticleMeshSimulator::advance_simulation()
{
	int cnt = 0;
	while ((space_time->get_time() < space_time->get_end_time()) && (PAUSED == false))
	{
		advance_single_timestep();

		++cnt;
 		if (cnt % 20 == 0) {
 	  		std::cout << "writing file at time: " << space_time->get_time() << "\n";
 	  		write_points(filenum++, pm_system->pPos, pm_system->pVel, exp(space_time->get_time())); // every 20 timestep
 	  		pm_system->timer.print_results();
 	  	}
	}
}


void ParticleMeshSimulator::sim_pause() { PAUSED = true; }
void ParticleMeshSimulator::sim_resume() { PAUSED = false; } 
void ParticleMeshSimulator::sim_change_dt(float new_dt) { space_time->update_dt(new_dt); }
void ParticleMeshSimulator::sim_set_delay(int new_delay) { delay_ms = new_delay; }
