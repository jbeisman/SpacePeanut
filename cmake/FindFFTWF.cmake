# - Find the FFTWF libraries
#
# Usage:
#   find_package(FFTWF [REQUIRED] [QUIET] )
#     
# It sets the following variables:
#   FFTWF_FOUND               ... true if fftwf is found on the system
#   FFTWF_LIBRARIES           ... full path to fftwf library
#   FFTWF_INCLUDES            ... fftwf include directory
#
# The following variables will be checked by the function
#   FFTWF_USE_STATIC_LIBS    ... if true, only static libraries are found
#   FFTWF_ROOT               ... if set, the libraries are exclusively searched
#                               under this path
#   FFTWF_LIB                ... fftwf library to use
#   FFTWF_THREADS_LIB        ... fftwf threads library to use
#   FFTWF_OMP_LIB            ... fftwf omp library to use
#   FFTWF_INCLUDE_DIR        ... fftwf include directory
#

#If environment variable FFTWDIR is specified, it has same effect as FFTWF_ROOT
if( NOT FFTWF_ROOT AND ENV{FFTWDIR} )
  set( FFTWF_ROOT $ENV{FFTWDIR} )
endif()

# Check if we can use PkgConfig
find_package(PkgConfig)

#Determine from PKG
if( PKG_CONFIG_FOUND AND NOT FFTWF_ROOT )
  pkg_check_modules( PKG_FFTWF QUIET "fftw3" )
endif()

#Check whether to search static or dynamic libs
set( CMAKE_FIND_LIBRARY_SUFFIXES_SAV ${CMAKE_FIND_LIBRARY_SUFFIXES} )

if( ${FFTWF_USE_STATIC_LIBS} )
  set( CMAKE_FIND_LIBRARY_SUFFIXES ${CMAKE_STATIC_LIBRARY_SUFFIX} )
else()
  set( CMAKE_FIND_LIBRARY_SUFFIXES ${CMAKE_SHARED_LIBRARY_SUFFIX} )
endif()

if( FFTWF_ROOT )

  find_library(
    FFTWF_LIB
    NAMES "fftw3f"
    PATHS ${FFTWF_ROOT}
    PATH_SUFFIXES "lib" "lib64"
    NO_DEFAULT_PATH
  )

  find_library(
    FFTWF_THREADS_LIB
    NAMES "fftw3f_threads"
    PATHS ${FFTWF_ROOT}
    PATH_SUFFIXES "lib" "lib64"
    NO_DEFAULT_PATH
  )

  find_library(
    FFTWF_OMP_LIB
    NAMES "fftw3f_omp"
    PATHS ${FFTWF_ROOT}
    PATH_SUFFIXES "lib" "lib64"
    NO_DEFAULT_PATH
  )

  #find includes
  find_path(
    FFTWF_INCLUDES
    NAMES "fftw3.h"
    PATHS ${FFTWF_ROOT}
    PATH_SUFFIXES "include"
    NO_DEFAULT_PATH
  )

else()

  find_library(
    FFTWF_LIB
    NAMES "fftw3f"
    PATHS ${PKG_FFTWF_LIBRARY_DIRS} ${LIB_INSTALL_DIR}
  )

  find_library(
    FFTWF_THREADS_LIB
    NAMES "fftw3f_threads"
    PATHS ${PKG_FFTWF_LIBRARY_DIRS} ${LIB_INSTALL_DIR}
  )


  find_library(
    FFTWF_OMP_LIB
    NAMES "fftw3f_omp"
    PATHS ${PKG_FFTWF_LIBRARY_DIRS} ${LIB_INSTALL_DIR}
  )

  find_path(
    FFTWF_INCLUDES
    NAMES "fftw3.h"
    PATHS ${PKG_FFTWF_INCLUDE_DIRS} ${INCLUDE_INSTALL_DIR}
  )

endif( FFTWF_ROOT )

set(FFTWF_LIBRARIES ${FFTWF_LIB})

set(CMAKE_FIND_LIBRARY_SUFFIXES ${CMAKE_FIND_LIBRARY_SUFFIXES_SAV})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(FFTWF DEFAULT_MSG
                                  FFTWF_INCLUDES FFTWF_LIBRARIES)

mark_as_advanced(FFTWF_INCLUDES FFTWF_LIBRARIES FFTWF_LIB FFTWF_THREADS_LIB FFTWF_OMP_LIB)