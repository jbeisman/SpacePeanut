cmake_minimum_required(VERSION 3.23 FATAL_ERROR)

add_executable(sdl_peanut SDL_test.cc camera.cc renderer.cc)
target_link_libraries(sdl_peanut PRIVATE SDL3::SDL3 GLEW glfw OpenGL imgui simulator)

install(TARGETS sdl_peanut)

# Assuming your shaders are in a 'shaders' subdirectory within your source directory
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/shaders/"
        DESTINATION "bin/shaders"  # Or "bin/shaders", "lib/myproject/shaders", etc.
        FILES_MATCHING PATTERN "*.glsl"        # Optional: Only copy files matching this pattern
)