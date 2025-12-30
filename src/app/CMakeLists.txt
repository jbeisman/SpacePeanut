cmake_minimum_required(VERSION 3.23 FATAL_ERROR)

add_executable(spacepeanut app.cc app_state.cc render_ui.cc camera.cc renderer.cc triad_renderer.cc color_palette.cc)
target_link_libraries(spacepeanut PRIVATE SDL3::SDL3 GLEW OpenGL imgui simulator)

install(TARGETS spacepeanut)

# Install shaders into the run directory
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/shaders/"
        DESTINATION "bin/shaders"
        FILES_MATCHING PATTERN "*.glsl"
)