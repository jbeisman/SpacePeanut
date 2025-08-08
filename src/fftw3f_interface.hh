
#pragma once

#include <fftw3.h>


class FFTW3FInterface {
public:
	static void init_fftw3f();
	static void cleanup_fftw3f();
	void create_plans(const int NGRID, float *mass_dist, float *potential, fftwf_complex *mass_fft);
	void destroy_plans();
	fftwf_plan r2c_plan;
	fftwf_plan c2r_plan;
};
