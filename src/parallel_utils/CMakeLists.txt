cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

# List of header files
set(PAR_HH_FILES
	parallel_utils.hh
)

if(USE_STD_PARALLEL)
    list(APPEND PAR_HH_FILES par_utils_std.hh)
endif()

if(USE_OPENMP)
    list(APPEND PAR_HH_FILES par_utils_omp.hh)
endif()

add_library(par_utils SHARED)
target_sources(par_utils PUBLIC ${PAR_HH_FILES} )
set_target_properties(par_utils PROPERTIES INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib" LINKER_LANGUAGE CXX)

if(USE_STD_PARALLEL)
	target_link_libraries(par_utils PRIVATE TBB::tbb)
endif()

if(USE_OPENMP)
	target_link_libraries(par_utils PRIVATE OpenMP::OpenMP_CXX)
endif()

target_include_directories(par_utils PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
