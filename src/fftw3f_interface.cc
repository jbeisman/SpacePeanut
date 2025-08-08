
#include <fftw3.h>
#include <stdexcept>
#include <iostream>
#include "fftw3f_interface.hh"

#if defined(USE_STD_PARALLEL)
#include <thread>
int get_max_threads() { return std::thread::hardware_concurrency(); }
#elif defined(USE_OPENMP)
#include <omp.h>
int get_max_threads() { return omp_get_max_threads(); }
#endif


void FFTW3FInterface::init_fftw3f()
{
	fftwf_init_threads();
    fftwf_plan_with_nthreads(get_max_threads());
}


void FFTW3FInterface::cleanup_fftw3f()
{
	fftwf_cleanup_threads();
}


void FFTW3FInterface::create_plans(
	const int NGRID,
	float *grid_mass,
	float *grid_potential,
	fftwf_complex *fourier_mass_pot)
{
	r2c_plan = fftwf_plan_dft_r2c_3d(NGRID, NGRID, NGRID, grid_mass, fourier_mass_pot, FFTW_ESTIMATE);
	if (!r2c_plan)    { throw std::runtime_error("Failed to initialize FFTW float R2C plan."); }
	c2r_plan = fftwf_plan_dft_c2r_3d(NGRID, NGRID, NGRID, fourier_mass_pot, grid_potential, FFTW_ESTIMATE);
	if (!c2r_plan)    { throw std::runtime_error("Failed to initialize FFTW float C2R plan."); }
}


void FFTW3FInterface::destroy_plans()
{
	fftwf_destroy_plan(r2c_plan);
	fftwf_destroy_plan(c2r_plan);
}

