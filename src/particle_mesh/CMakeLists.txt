cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

add_subdirectory(grid_utils)
add_subdirectory(boundary_condition)
add_subdirectory(gravity)

# List of source files
set(PM_CC_FILES
	particle_mesh_system.cc
	time_logger.cc
	utils.cc
)

# List of header files
set(PM_HH_FILES
	particle_mesh_system.hh
	time_logger.hh
	utils.hh
)

add_library(pm_system SHARED)
target_sources(pm_system PUBLIC ${PM_CC_FILES} ${PM_HH_FILES})
set_target_properties(pm_system PROPERTIES INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib" LINKER_LANGUAGE CXX)

target_link_libraries(pm_system PUBLIC fft_utils par_utils grid_utils boundary_cond pm_gravity PRIVATE ${FFTWF_LIBRARIES})

if(USE_STD_PARALLEL)
	target_link_libraries(pm_system PRIVATE TBB::tbb)
endif()

if(USE_OPENMP)
	target_link_libraries(pm_system PRIVATE OpenMP::OpenMP_CXX)
endif()

target_include_directories(pm_system PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
