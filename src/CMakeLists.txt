cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

add_subdirectory(fftw3f)
add_subdirectory(parallel_utils)
add_subdirectory(particle_mesh)
add_subdirectory(simulator)

# Build executable
add_executable(spacepeanut main.cc)
set_target_properties(spacepeanut PROPERTIES INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/bin" LINKER_LANGUAGE CXX)
target_link_libraries(spacepeanut PUBLIC fft_utils par_utils grid_utils boundary_cond pm_gravity pm_system simulator PRIVATE ${FFTWF_LIBRARIES})

if(USE_STD_PARALLEL)
  target_link_libraries(spacepeanut PRIVATE TBB::tbb)
endif()

if(USE_OPENMP)
target_link_libraries(spacepeanut PRIVATE OpenMP::OpenMP_CXX)
endif()

target_include_directories(spacepeanut PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} )

# Install libraries and executables
install(TARGETS fft_utils par_utils grid_utils boundary_cond pm_gravity pm_system simulator spacepeanut)
