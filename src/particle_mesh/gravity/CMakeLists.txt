cmake_minimum_required(VERSION 3.23 FATAL_ERROR)

# List of source files
set(PM_GRAV_CC_FILES
	acceleration.cc
	lambdaCDM.cc
)

# List of header files
set(PM_GRAV_PUBLIC_HH_FILES
	acceleration.hh
	lambdaCDM.hh
)

if(USE_STD_PARALLEL)
    list(APPEND PM_GRAV_CC_FILES acceleration_std.cc)
endif()

if(USE_OPENMP)
    list(APPEND PM_GRAV_CC_FILES acceleration_omp.cc)
endif()

add_library(pm_gravity SHARED)

target_sources(pm_gravity PRIVATE ${PM_GRAV_CC_FILES})
target_sources(pm_gravity PUBLIC FILE_SET HEADERS FILES ${PM_GRAV_PUBLIC_HH_FILES})

if(USE_STD_PARALLEL)
	target_link_libraries(pm_gravity PRIVATE TBB::tbb)
endif()

if(USE_OPENMP)
	target_link_libraries(pm_gravity PRIVATE OpenMP::OpenMP_CXX)
endif()

target_link_libraries(pm_gravity PRIVATE fft_utils par_utils grid_utils ${FFTWF_LIBRARIES})

install(TARGETS pm_gravity FILE_SET HEADERS)
