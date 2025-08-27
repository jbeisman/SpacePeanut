cmake_minimum_required(VERSION 3.23 FATAL_ERROR)

# List of source files
set(BC_CC_FILES
	periodic_boundary.cc
)

# List of public header files
set(BC_PUBLIC_HH_FILES
	periodic_boundary.hh
)

if(USE_STD_PARALLEL)
    list(APPEND BC_CC_FILES periodic_boundary_std.cc)
endif()

if(USE_OPENMP)
    list(APPEND BC_CC_FILES periodic_boundary_omp.cc)
endif()

add_library(boundary_cond SHARED)

target_sources(boundary_cond PRIVATE ${BC_CC_FILES})
target_sources(boundary_cond PUBLIC FILE_SET HEADERS FILES ${BC_PUBLIC_HH_FILES})

if(USE_STD_PARALLEL)
	target_link_libraries(boundary_cond PRIVATE TBB::tbb)
endif()

if(USE_OPENMP)
	target_link_libraries(boundary_cond PRIVATE OpenMP::OpenMP_CXX)
endif()

install(TARGETS boundary_cond FILE_SET HEADERS)
