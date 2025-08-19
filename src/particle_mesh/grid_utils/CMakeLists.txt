cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

# List of source files
set(GRID_CC_FILES
	grid_utils.cc
)

# List of header files
set(GRID_HH_FILES
	grid_constants.hh
	grid_utils.hh
)

add_library(grid_utils SHARED)
target_sources(grid_utils PUBLIC ${GRID_CC_FILES} ${GRID_HH_FILES})
set_target_properties(grid_utils PROPERTIES INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib" LINKER_LANGUAGE CXX)

if(USE_STD_PARALLEL)
	target_link_libraries(grid_utils PRIVATE TBB::tbb)
endif()

if(USE_OPENMP)
	target_link_libraries(grid_utils PRIVATE OpenMP::OpenMP_CXX)
endif()

target_include_directories(grid_utils PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
