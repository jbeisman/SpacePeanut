cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

project(SpacePeanut CXX)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)



## Add user options
option(USE_OPENMP "Enable OpenMP parallelization" ON)
option(USE_STD_PARALLEL "Enable C++ std::execution parallelization" OFF)



# Ensure that only one of USE_OPENMP or USE_STD_PARALLEL is set to true
# IMPORTANT -- this needs to stay toward the top of CMake tree
if (USE_OPENMP AND NOT USE_STD_PARALLEL)
    message(STATUS "USE_OPENMP is true, building with OpenMP parallel support")
elseif (USE_STD_PARALLEL AND NOT USE_OPENMP)
    message(STATUS "USE_STD_PARALLEL is true, building with std::execution parallel support")
else()
    message(FATAL_ERROR "ERROR: Incorrect configuration -- Please set only one of USE_STD_PARALLEL or USE_OPENMP to true")
endif()



# Set compiler definitions
if (USE_OPENMP)
  add_compile_definitions (USE_OPENMP)
endif()

if(USE_STD_PARALLEL)
  add_compile_definitions (USE_STD_PARALLEL)
endif()

## Global compiler flags
add_compile_options(-ffast-math)



# Location of user-provided CMake modules
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Find external libraries
find_package(FFTWF REQUIRED)
find_package(OpenMP REQUIRED)
find_package(TBB REQUIRED)



# Add FFTW support for threading backend 
if(USE_OPENMP)
    list(APPEND FFTWF_LIBRARIES ${FFTWF_OMP_LIB})
endif()

if(USE_STD_PARALLEL)
    list(APPEND FFTWF_LIBRARIES ${FFTWF_THREADS_LIB})
endif()


# go into subdirectory
add_subdirectory(src)



add_executable(spacepeanut src/main.cc)

if(USE_STD_PARALLEL)
  target_link_libraries(spacepeanut PUBLIC simulator TBB::tbb ${FFTWF_LIBRARIES})
endif()

if(USE_OPENMP)
target_link_libraries(spacepeanut PUBLIC simulator OpenMP::OpenMP_CXX ${FFTWF_LIBRARIES})
endif()

target_include_directories(spacepeanut PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${FFTWF_INCLUDE_DIR} )

install(TARGETS simulator spacepeanut)
