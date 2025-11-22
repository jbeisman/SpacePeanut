cmake_minimum_required(VERSION 3.23 FATAL_ERROR)

# List of source files
set(SIM_CC_FILES
	get_input.cc
	simulator.cc
	space_time.cc
)

# List of public header files
set(SIM_PUBLIC_HH_FILES
	simulator.hh
)

add_library(simulator SHARED)

target_sources(simulator PRIVATE ${SIM_CC_FILES})
target_sources(simulator PUBLIC FILE_SET HEADERS FILES ${SIM_PUBLIC_HH_FILES})

target_link_libraries(simulator PUBLIC pm_system)

install(TARGETS simulator FILE_SET HEADERS)
