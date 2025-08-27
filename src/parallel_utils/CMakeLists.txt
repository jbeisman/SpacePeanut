cmake_minimum_required(VERSION 3.23 FATAL_ERROR)

# List of public header files
set(PAR_PUBLIC_HH_FILES
	parallel_utils.hh
)


add_library(par_utils INTERFACE)

target_sources(par_utils PUBLIC FILE_SET HEADERS FILES ${PAR_PUBLIC_HH_FILES})

if(USE_STD_PARALLEL)
	target_link_libraries(par_utils INTERFACE TBB::tbb)
endif()

if(USE_OPENMP)
	target_link_libraries(par_utils INTERFACE OpenMP::OpenMP_CXX)
endif()

install(TARGETS par_utils FILE_SET HEADERS)
