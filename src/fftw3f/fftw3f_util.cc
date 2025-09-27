
#include "fftw3f_util.hh"
#include <stdexcept>

#if defined(USE_STD_PARALLEL)
#include <thread>
int get_max_threads() { return std::thread::hardware_concurrency(); }
#elif defined(USE_OPENMP)
#include <omp.h>
int get_max_threads() { return omp_get_max_threads(); }
#endif

void fft_util::init_fftw3f() {
  fftwf_init_threads();
  fftwf_plan_with_nthreads(get_max_threads());
}

void fft_util::cleanup_fftw3f() { fftwf_cleanup_threads(); }

std::unique_ptr<fft_util::Plans>
fft_util::create_plans(const int NGRID, float *mass, float *potential,
                       fftwf_complex *mass_fft) {

  fftwf_plan r2c_plan =
      fftwf_plan_dft_r2c_3d(NGRID, NGRID, NGRID, mass, mass_fft, FFTW_ESTIMATE);

  if (!r2c_plan) {
    throw std::runtime_error("Failed to initialize FFTW float R2C plan.");
  }

  fftwf_plan c2r_plan =
      fftwf_plan_dft_c2r_3d(NGRID, NGRID, NGRID, mass_fft, potential, FFTW_ESTIMATE);

  if (!c2r_plan) {
    throw std::runtime_error("Failed to initialize FFTW float C2R plan.");
  }

  return std::make_unique<fft_util::Plans>(r2c_plan, c2r_plan);
}

fft_util::Plans::Plans(fftwf_plan r2c, fftwf_plan c2r)
  : r2c_plan(r2c), c2r_plan(c2r) {}

fft_util::Plans::~Plans() {
  fftwf_destroy_plan(this->r2c_plan);
  fftwf_destroy_plan(this->c2r_plan);
}
