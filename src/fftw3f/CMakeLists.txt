cmake_minimum_required(VERSION 3.23 FATAL_ERROR)

# List of source files
set(FFT_CC_FILES
	fftw3f_util.cc
)

# List of public header files
set(FFT_PUBLIC_HH_FILES
	fftw3f_allocator.hh
	fftw3f_util.hh
)

add_library(fft_utils SHARED)

target_sources(fft_utils PRIVATE ${FFT_CC_FILES})
target_sources(fft_utils PUBLIC FILE_SET HEADERS FILES ${FFT_PUBLIC_HH_FILES})

target_link_libraries(fft_utils PRIVATE ${FFTWF_LIBRARIES})

install(TARGETS fft_utils FILE_SET HEADERS)
