cmake_minimum_required(VERSION 3.23 FATAL_ERROR)

# List of source files
set(GRID_CC_FILES
	grid_utils.cc
)

# List of public header files
set(GRID_PUBLIC_HH_FILES
	grid_utils.hh
)

add_library(grid_utils SHARED)

target_sources(grid_utils PRIVATE ${GRID_CC_FILES})
target_sources(grid_utils PUBLIC FILE_SET HEADERS FILES ${GRID_PUBLIC_HH_FILES})

install(TARGETS grid_utils FILE_SET HEADERS)
