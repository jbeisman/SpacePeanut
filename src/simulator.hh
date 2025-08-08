
#pragma once

#include <cmath>
#include "particle_mesh_system.hh"


struct CosmicTime {

	CosmicTime() = delete;
	CosmicTime(int rdshft, int nstps) : redshft_z(rdshft), nsteps(nstps)
	{
		time  = log(1.0/(rdshft+1.0));
		t_end = log(1.0);
		dt    = (t_end - (float)time) / nsteps;
	}
	
	double get_time() const
	{ return time; }

	float get_dt() const
	{ return dt; }

	float get_end_time() const
	{ return t_end; }

	float get_time_scale() const
	{ return exp(static_cast<float>(time)); }
	
	void update_time(double val)
	{ time += val; }

	void update_dt(float val)
	{ dt = val; }

private:
	int redshft_z;
	int nsteps;
	double time;
	float dt;
	float t_end;
};


class ParticleMeshSimulator {

public:
	ParticleMeshSimulator();
	~ParticleMeshSimulator();
	void initialize_simulation();
	void advance_single_timestep();
	void advance_simulation();
	void sim_pause();
	void sim_resume();
	void sim_change_dt(float new_dt);
	void sim_set_delay(int new_delay);

private:
	std::unique_ptr<CosmicTime> space_time;
	std::unique_ptr<ParticleMeshSystem> pm_system;
	int filenum{0};
	bool PAUSED{false};
	int delay_ms{0};
};

