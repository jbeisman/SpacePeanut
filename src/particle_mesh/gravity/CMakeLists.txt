cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

# List of source files
set(PM_GRAV_CC_FILES
	acceleration.cc
)

# List of header files
set(PM_GRAV_HH_FILES
	acceleration.hh
	acceleration_kernels.hh
	acceleration_par.hh
)

if(USE_STD_PARALLEL)
    list(APPEND PM_GRAV_CC_FILES acceleration_std.cc)
endif()

if(USE_OPENMP)
    list(APPEND PM_GRAV_CC_FILES acceleration_omp.cc)
endif()

add_library(pm_gravity SHARED)
target_sources(pm_gravity PUBLIC ${PM_GRAV_CC_FILES} ${PM_GRAV_HH_FILES})
set_target_properties(pm_gravity PROPERTIES INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib" LINKER_LANGUAGE CXX)
target_link_libraries(pm_gravity PUBLIC fft_utils par_utils grid_utils boundary_cond PRIVATE ${FFTWF_LIBRARIES})

if(USE_STD_PARALLEL)
	target_link_libraries(pm_gravity PRIVATE TBB::tbb)
endif()

if(USE_OPENMP)
	target_link_libraries(pm_gravity PRIVATE OpenMP::OpenMP_CXX)
endif()

target_include_directories(pm_gravity PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
