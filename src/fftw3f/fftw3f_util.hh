
#pragma once

#include <fftw3.h>
#include <memory>

namespace fft_util {

class Plans {
public:
  Plans() = default;
  Plans(fftwf_plan r2c, fftwf_plan c2r);
  ~Plans();
  fftwf_plan r2c_plan;
  fftwf_plan c2r_plan;
};

void init_fftw3f();
void cleanup_fftw3f();
std::unique_ptr<Plans> create_plans(const int NGRID, float *mass,
                                    float *potential, fftwf_complex *mass_fft);
} // namespace fft_util
