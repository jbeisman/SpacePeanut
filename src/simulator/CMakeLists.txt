cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

# List of source files
set(SIM_CC_FILES
	get_input.cc
	simulator.cc
)

# List of header files
set(SIM_HH_FILES
	get_input.hh
	simulator.hh
)

add_library(simulator SHARED)
target_sources(simulator PUBLIC ${SIM_CC_FILES} ${SIM_HH_FILES})
set_target_properties(simulator PROPERTIES INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib" LINKER_LANGUAGE CXX)

if(USE_STD_PARALLEL)
	target_link_libraries(simulator PUBLIC fft_utils pm_system PRIVATE TBB::tbb)
endif()

if(USE_OPENMP)
	target_link_libraries(simulator PUBLIC fft_utils pm_system PRIVATE OpenMP::OpenMP_CXX)
endif()

target_include_directories(simulator PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
