cmake_minimum_required(VERSION 3.23 FATAL_ERROR)

project(SpacePeanut CXX)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

## Add user options
## These are provided here as defaults, but should be set in CMakePresets.json
option(USE_OPENMP "Enable OpenMP parallelization" OFF)
option(USE_STD_PARALLEL "Enable C++ std::execution parallelization" OFF)

# IMPORTANT -- this needs to stay toward the top of CMake tree
# Ensure that only one of USE_OPENMP or USE_STD_PARALLEL is set to true
if (USE_OPENMP AND NOT USE_STD_PARALLEL)
    message(STATUS "USE_OPENMP is true, building with OpenMP parallel support")
elseif (USE_STD_PARALLEL AND NOT USE_OPENMP)
    message(STATUS "USE_STD_PARALLEL is true, building with std::execution parallel support")
else()
    message(FATAL_ERROR "ERROR: Incorrect configuration -- Please set only one of USE_STD_PARALLEL or USE_OPENMP to true")
endif()

# Set compiler definitions
if (USE_STD_PARALLEL)
  add_compile_definitions (USE_STD_PARALLEL)
endif()

if (USE_OPENMP)
  add_compile_definitions (USE_OPENMP)
endif()

# Set RPATH
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
list(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES "${CMAKE_INSTALL_PREFIX}/lib" isSystemDir)
if("${isSystemDir}" STREQUAL "-1")
    set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
endif("${isSystemDir}" STREQUAL "-1")

# Location of user-provided CMake modules
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Find external libraries
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)
find_package(FFTWF REQUIRED)
find_package(OpenMP REQUIRED)
find_package(TBB REQUIRED)

# Add FFTW libraries to support threading backend 
if (USE_STD_PARALLEL)
    list(APPEND FFTWF_LIBRARIES ${FFTWF_THREADS_LIB})
endif()

if (USE_OPENMP)
    list(APPEND FFTWF_LIBRARIES ${FFTWF_OMP_LIB})
endif()

# Build the project
add_subdirectory(third_party)
add_subdirectory(src)
