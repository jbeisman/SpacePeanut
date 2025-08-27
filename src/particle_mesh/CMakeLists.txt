cmake_minimum_required(VERSION 3.23 FATAL_ERROR)

add_subdirectory(grid_utils)
add_subdirectory(boundary_condition)
add_subdirectory(gravity)

# List of source files
set(PM_CC_FILES
	particle_mesh_system.cc
	time_logger.cc
	utils.cc
)

# List of public header files
set(PM_PUBLIC_HH_FILES
	particle_mesh_system.hh
)

add_library(pm_system SHARED)

target_sources(pm_system PRIVATE ${PM_CC_FILES})
target_sources(pm_system PUBLIC FILE_SET HEADERS FILES ${PM_PUBLIC_HH_FILES})

if(USE_STD_PARALLEL)
	target_link_libraries(pm_system PRIVATE TBB::tbb)
endif()

if(USE_OPENMP)
	target_link_libraries(pm_system PRIVATE OpenMP::OpenMP_CXX)
endif()

target_link_libraries(pm_system PUBLIC fft_utils par_utils grid_utils boundary_cond pm_gravity PRIVATE ${FFTWF_LIBRARIES})

install(TARGETS pm_system FILE_SET HEADERS)
