cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

# List of source files
set(FFT_CC_FILES
	fftw3f_util.cc
)

# List of header files
set(FFT_HH_FILES
	fftw3f_allocator.hh
	fftw3f_util.hh
)

add_library(fft_utils SHARED)
target_sources(fft_utils PUBLIC ${FFT_CC_FILES} ${FFT_HH_FILES})
set_target_properties(fft_utils PROPERTIES INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib" LINKER_LANGUAGE CXX)
target_link_libraries(fft_utils PRIVATE ${FFTWF_LIBRARIES})
target_include_directories(fft_utils PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} PRIVATE ${FFTWF_INCLUDE_DIR} )
