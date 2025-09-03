cmake_minimum_required(VERSION 3.23 FATAL_ERROR)

add_executable(sdl_peanut SDL_test.cc)

target_link_libraries(sdl_peanut PRIVATE SDL3::SDL3 OpenGL imgui simulator)
#target_compile_options(sdl_peanut PRIVATE -fsanitize=address -Wall -Wextra)
#target_link_options(sdl_peanut PRIVATE -fsanitize=address)

install(TARGETS sdl_peanut)