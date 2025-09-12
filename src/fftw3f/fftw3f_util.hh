
#pragma once

#include <fftw3.h>
#include <memory>

namespace fft_util {

struct Plans {
  fftwf_plan r2c_plan;
  fftwf_plan c2r_plan;
};

void init_fftw3f();
void cleanup_fftw3f();
std::unique_ptr<Plans> create_plans(const int NGRID, float *mass,
                                    float *potential, fftwf_complex *mass_fft);
void destroy_plans(Plans *plans);
} // namespace fft_util
