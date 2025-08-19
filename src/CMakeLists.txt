cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

# List of source files
set(SP_CC_FILES
	acceleration.cc
	fftw3f_util.cc
	get_input.cc
	grid_utils.cc
	particle_mesh_system.cc
	periodic_boundary.cc
	simulator.cc
	time_logger.cc
	utils.cc
)

# List of header files
set(SP_HH_FILES
	acceleration.hh
	acceleration_kernels.hh
	acceleration_par.hh
	fftw3f_allocator.hh
	fftw3f_util.hh
	get_input.hh
	grid_constants.hh
	grid_utils.hh
	parallel_utils.hh
	particle_mesh_system.hh
	periodic_boundary.hh
	simulator.hh
	time_logger.hh
	utils.hh
)

if(USE_OPENMP)
    message(STATUS "Adding OpenMP specific files to target")
    list(APPEND SP_CC_FILES acceleration_omp.cc periodic_boundary_omp.cc)
    list(APPEND SP_HH_FILES par_utils_omp.hh)
endif()

if(USE_STD_PARALLEL)
	message(STATUS "Adding std::execution specific files to target")
    list(APPEND SP_CC_FILES acceleration_std.cc periodic_boundary_std.cc)
    list(APPEND SP_HH_FILES par_utils_std.hh)
endif()
	
add_library(simulator
	STATIC
	${SP_CC_FILES}
	${SP_HH_FILES}
)	

if(USE_STD_PARALLEL)
  target_link_libraries(simulator PUBLIC TBB::tbb ${FFTWF_LIBRARIES})
endif()

if(USE_OPENMP)
target_link_libraries(simulator PUBLIC OpenMP::OpenMP_CXX ${FFTWF_LIBRARIES})
endif()

target_include_directories(simulator PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${FFTWF_INCLUDE_DIR} )
