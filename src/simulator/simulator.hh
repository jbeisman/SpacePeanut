
#pragma once

#include <cmath>
#include "particle_mesh_system.hh"


struct CosmicTime {

	CosmicTime() : redshft_z(50.0f), nsteps(1000)
	{
		setup_time(redshft_z, nsteps);
	}
	
	CosmicTime(int rdshft, int nstp) : redshft_z(rdshft), nsteps(nstp)
	{
		setup_time(redshft_z, nsteps);
	}

	void setup_time(int rdshft, int nstp)
	{
		time  = log(1.0/(rdshft+1.0));
		t_end = log(1.0);
		dt    = (t_end - (float)time) / nstp;
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
	void initialize_simulation(
		const float redshift,
		const int ntimesteps,
		const int nbods,
		const int ngrid,
		const float gridlength);
	void advance_single_timestep();
	void advance_simulation();
	void sim_change_pause_state(bool p);
	bool sim_is_paused();
	void sim_change_dt(float new_dt);
	void sim_set_delay(int new_delay);
	void sim_set_write_output(bool write_output);

private:
	std::unique_ptr<CosmicTime> space_time;
	std::unique_ptr<ParticleMeshSystem> pm_system;
	int filenum{0};
	int delay_ms{0};
	bool PAUSED{false};
	bool WRITE_OUTPUT{true};
};


// TODO -  improve and move
void write_points(
	const int filenum,
	const std::vector<std::array<float, 3>>& Pos,
	const std::vector<std::array<float, 3>>& Vel,
	const float time_scale);