cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

# List of source files
set(BC_CC_FILES
	periodic_boundary.cc
)

# List of header files
set(BC_HH_FILES
	periodic_boundary.hh
)

if(USE_STD_PARALLEL)
    list(APPEND BC_CC_FILES periodic_boundary_std.cc)
endif()

if(USE_OPENMP)
    list(APPEND BC_CC_FILES periodic_boundary_omp.cc)
endif()

add_library(boundary_cond SHARED)
target_sources(boundary_cond PUBLIC ${BC_CC_FILES} ${BC_HH_FILES})
set_target_properties(boundary_cond PROPERTIES INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib" LINKER_LANGUAGE CXX)

if(USE_STD_PARALLEL)
	target_link_libraries(boundary_cond PRIVATE TBB::tbb)
endif()

if(USE_OPENMP)
	target_link_libraries(boundary_cond PRIVATE OpenMP::OpenMP_CXX)
endif()

target_include_directories(boundary_cond PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
