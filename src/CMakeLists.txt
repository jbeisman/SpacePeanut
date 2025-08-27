cmake_minimum_required(VERSION 3.23 FATAL_ERROR)

add_subdirectory(fftw3f)
add_subdirectory(parallel_utils)
add_subdirectory(particle_mesh)
add_subdirectory(simulator)

add_executable(spacepeanut main.cc)

target_link_libraries(spacepeanut PUBLIC simulator)

install(TARGETS spacepeanut)
