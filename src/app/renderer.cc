
#include "renderer.hh"
#include "parallel_utils.hh"

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <SDL3/SDL.h>

#include <filesystem>
#include <stdexcept>

#include <vector>

GLuint Renderer::compileShader(GLenum type, const char *path) {
  std::ifstream file(path);
  std::stringstream buffer;
  buffer << file.rdbuf();
  std::string srcStr = buffer.str();
  const char *src = srcStr.c_str();

  GLuint shader = glCreateShader(type);
  glShaderSource(shader, 1, &src, NULL);
  glCompileShader(shader);

  // Error checking
  GLint success;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    char info[512];
    glGetShaderInfoLog(shader, 512, nullptr, info);
    std::cerr << "Shader compile error: " << info << std::endl;
  }

  return shader;
}

GLuint Renderer::createShaderProgram(const char *vertexPath,
                                     const char *fragmentPath) {
  GLuint vs = compileShader(GL_VERTEX_SHADER, vertexPath);
  GLuint fs = compileShader(GL_FRAGMENT_SHADER, fragmentPath);

  GLuint program = glCreateProgram();
  glAttachShader(program, vs);
  glAttachShader(program, fs);
  glLinkProgram(program);

  // Error checking
  GLint success;
  glGetProgramiv(program, GL_LINK_STATUS, &success);
  if (!success) {
    char info[512];
    glGetProgramInfoLog(program, 512, nullptr, info);
    std::cerr << "Shader link error: " << info << std::endl;
  }

  glDeleteShader(vs);
  glDeleteShader(fs);
  return program;
}

Renderer::Renderer() {
  this->simulator = std::make_unique<ParticleMeshSimulator>();
  this->camera = nullptr;

  glEnable(GL_PROGRAM_POINT_SIZE);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void Renderer::init(float RSHIFT, int NSTEPS, int NBODS, int NGRID,
                    float GMAX) {
  this->NUMBODS = NBODS;
  this->NUMGRID = NGRID;
  this->GRIDLEN = GMAX;
  this->camera = std::make_unique<Camera>(
      glm::vec3(GMAX / 2, GMAX / 2, -1.5 * GMAX),
      glm::vec3(GMAX / 2, GMAX / 2, GMAX / 2), glm::vec3(0.0f, 1.0f, 0.0f));

  this->simulator->initialize_simulation(RSHIFT, NSTEPS, NBODS, NGRID, GMAX);

  auto [mass_minimum, mass_maximum] =
      minmax_vec_elems(this->simulator->get_mass_density_ref());
  this->mass_min = mass_minimum;
  this->mass_max = mass_maximum;

  auto *vertices = this->simulator->get_positions();

  // Generate buffers
  glGenVertexArrays(1, &this->VAO);
  glGenBuffers(1, &this->VBO);
  glGenBuffers(1, &this->UBO);

  // Bind vertex array
  glBindVertexArray(this->VAO);
  glEnableVertexAttribArray(0);

  // Bind vertex buffer and setup data
  glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * NBODS * 3, vertices,
               GL_DYNAMIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat),
                        (void *)0);

  // Generate texture for mass density
  glGenTextures(1, &this->texture3D);
  glBindTexture(GL_TEXTURE_3D, this->texture3D);

  // Set texture parameters
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // Upload data to the texture.
  auto *density = this->simulator->get_mass_density();
  glTexImage3D(GL_TEXTURE_3D, 0, GL_R32F, this->NUMGRID, this->NUMGRID,
               this->NUMGRID, 0, GL_RED, GL_FLOAT, density);

  // Color texture
  glGenTextures(1, &this->textureColor);
  glBindTexture(GL_TEXTURE_1D, this->textureColor);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  // glTexImage1D(GL_TEXTURE_1D, 0, GL_RGB, this->colorMap.size(), 0, GL_RGB,
  // GL_FLOAT, this->colorMap.data());

  // Setup uniform buffer
  glBindBuffer(GL_UNIFORM_BUFFER, this->UBO);
  glBufferData(GL_UNIFORM_BUFFER, 3 * sizeof(glm::mat4), nullptr,
               GL_DYNAMIC_DRAW); // Allocate space only
  glBindBuffer(GL_UNIFORM_BUFFER, 0);

  // Bind UBO to binding point 0
  glBindBufferBase(GL_UNIFORM_BUFFER, 0, this->UBO);

  // Unbind
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  // Load and compile shaders
  std::filesystem::path base_path = SDL_GetBasePath();
  std::filesystem::path vs_name = base_path / "shaders/vert.glsl";
  std::filesystem::path fs_name = base_path / "shaders/frag.glsl";

  this->shaderProgram = createShaderProgram(vs_name.c_str(), fs_name.c_str());
  glUseProgram(this->shaderProgram);

  // Add uniform block to shader program
  GLuint blockIndex = glGetUniformBlockIndex(this->shaderProgram, "UBO");
  glUniformBlockBinding(this->shaderProgram, blockIndex,
                        0); // 0 = binding point
}

void Renderer::run_and_display(float aspect_ratio, Color::ColorType color,
                              bool change_color, float mass_clip_factor) {

  if (!simulator->sim_is_paused()) {

    // Run a timestep if ready
    this->simulator->advance_single_timestep();

    // Get density min and max
    auto [mass_minimum, mass_maximum] =
      minmax_vec_elems(this->simulator->get_mass_density_ref());
    this->mass_min = mass_minimum;
    this->mass_max = mass_maximum;

    // Bind new density data to texture buffer
    glBindTexture(GL_TEXTURE_3D, this->texture3D);
    auto *density = this->simulator->get_mass_density();
    glTexSubImage3D(GL_TEXTURE_3D, 0, 0, 0, 0, this->NUMGRID, this->NUMGRID,
                    this->NUMGRID, GL_RED, GL_FLOAT, density);

    // Bind new position data to VBO
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    auto *vertices = this->simulator->get_positions();
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * this->NUMBODS * 3,
                    vertices);
  }

  // Generate new color texture
  if (change_color) {
    this->colorMap = getColormap(color);
    glTexImage1D(GL_TEXTURE_1D, 0, GL_RGB, this->colorMap.size(), 0, GL_RGB,
                 GL_FLOAT, this->colorMap.data());
  }

  // Clipping factor
  float clipped_mass_max = this->mass_max * mass_clip_factor;

  // Enable depth test
  glEnable(GL_DEPTH_TEST);
  // Accept fragment if it is closer to the camera than the former one
  glDepthFunc(GL_LESS);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glUseProgram(this->shaderProgram);

  // Update UBO data
  glBindBuffer(GL_UNIFORM_BUFFER, this->UBO);
  glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4),
                  glm::value_ptr(this->camera->get_model_matrix()));
  glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4),
                  glm::value_ptr(this->camera->get_view_matrix()));
  glBufferSubData(
      GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), sizeof(glm::mat4),
      glm::value_ptr(this->camera->get_projection_matrix(aspect_ratio)));
  glBindBuffer(GL_UNIFORM_BUFFER, 0);

  // Grid parameters
  glm::vec3 grid_origin(0.0f, 0.0f, 0.0f);
  glm::vec3 grid_size(this->GRIDLEN, this->GRIDLEN, this->GRIDLEN);
  glUniform3fv(glGetUniformLocation(shaderProgram, "grid_origin"), 1,
               glm::value_ptr(grid_origin));
  glUniform3fv(glGetUniformLocation(shaderProgram, "grid_size"), 1,
               glm::value_ptr(grid_size));
  glUniform1f(glGetUniformLocation(shaderProgram, "density_min"), this->mass_min);
  glUniform1f(glGetUniformLocation(shaderProgram, "density_max"), clipped_mass_max);

  // Bind textures
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_3D, this->texture3D);
  glUniform1i(glGetUniformLocation(shaderProgram, "densityTexture"), 0);

  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_1D, this->textureColor);
  glUniform1i(glGetUniformLocation(shaderProgram, "colorMapTexture"), 1);

  // Bind VAO and draw
  glBindVertexArray(this->VAO);
  glDrawArrays(GL_POINTS, 0, this->NUMBODS);
  glBindVertexArray(0);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Reset to normal mode
}


void Renderer::reset_simulator() {
  this->simulator.reset();
  this->simulator = std::make_unique<ParticleMeshSimulator>();
}

Renderer::~Renderer() {
  glDisableVertexAttribArray(0);
  glDeleteBuffers(1, &this->VBO);
  glDeleteBuffers(1, &this->UBO);
  glBindTexture(GL_TEXTURE_3D, 0);
  glDeleteTextures(1, &this->texture3D);
  glBindTexture(GL_TEXTURE_1D, 0);
  glDeleteTextures(1, &this->textureColor);
  glUseProgram(0);
  glDeleteProgram(this->shaderProgram);
  glDeleteVertexArrays(1, &this->VAO);
}
