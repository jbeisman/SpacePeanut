cmake_minimum_required(VERSION 3.23 FATAL_ERROR)

add_subdirectory(SDL)
install(TARGETS SDL3-shared)

add_subdirectory(imgui)
