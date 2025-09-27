
#include "renderer.hh"
#include "parallel_utils.hh"

#include <iostream>
#include <filesystem>
#include <stdexcept>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SDL3/SDL.h>


Renderer::Renderer()
  : simulator(std::make_unique<ParticleMeshSimulator>()),
    camera(),
    numbods(0),
    numgrid(0),
    gridlen(0.0f),
    mass_min(0.0f),
    mass_max(0.0f),
    shader_program_log(0),
    shader_program_lin(0),
    texture_3D(0),
    texture_color(0),
    VAO(0),
    VBO(0),
    UBO(0)
{
  glEnable(GL_PROGRAM_POINT_SIZE);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
}

void Renderer::init(float RSHIFT, int NSTEPS, int NBODS, int NGRID,
                    float GMAX) {
  this->numbods = NBODS;
  this->numgrid = NGRID;
  this->gridlen = GMAX;
  this->camera = Camera(glm::vec3(GMAX / 2, GMAX / 2, -1.5 * GMAX),
                        glm::vec3(GMAX / 2, GMAX / 2, GMAX / 2),
                        glm::vec3(0.0f, 1.0f, 0.0f));

  this->simulator->initialize_simulation(RSHIFT, NSTEPS, NBODS, NGRID, GMAX);

  auto [mass_minimum, mass_maximum] =
      minmax_vec_elems(this->simulator->get_mass_density_ref());
  this->mass_min = mass_minimum;
  this->mass_max = mass_maximum;

  // Generate buffers
  glGenVertexArrays(1, &this->VAO);
  glGenBuffers(1, &this->VBO);
  glGenBuffers(1, &this->UBO);

  // Bind vertex array
  glBindVertexArray(this->VAO);
  glEnableVertexAttribArray(0);

  // Bind vertex buffer and setup data
  glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * NBODS * 3,
    this->simulator->get_positions(), GL_DYNAMIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat),
                        (void *)0);

  // Generate texture for mass density
  glGenTextures(1, &this->texture_3D);
  glBindTexture(GL_TEXTURE_3D, this->texture_3D);

  // Set texture parameters
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // Upload data to the texture
  glTexImage3D(GL_TEXTURE_3D, 0, GL_R32F, this->numgrid, this->numgrid,
               this->numgrid, 0, GL_RED, GL_FLOAT,
               this->simulator->get_mass_density());

  // Color texture
  glGenTextures(1, &this->texture_color);
  glBindTexture(GL_TEXTURE_1D, this->texture_color);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);

  // Setup uniform buffer
  glBindBuffer(GL_UNIFORM_BUFFER, this->UBO);
  glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), nullptr, GL_DYNAMIC_DRAW);
  glBindBuffer(GL_UNIFORM_BUFFER, 0);

  // Bind UBO to binding point 0
  glBindBufferBase(GL_UNIFORM_BUFFER, 0, this->UBO);

  // Unbind
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  // Load and compile shaders
  std::filesystem::path base_path = SDL_GetBasePath();
  std::filesystem::path vs_name = base_path / "shaders/vert.glsl";
  std::filesystem::path fs_lin_name = base_path / "shaders/frag_linear.glsl";
  std::filesystem::path fs_log_name = base_path / "shaders/frag_log.glsl";

  this->shader_program_lin = create_shader_program(vs_name.c_str(), fs_lin_name.c_str());
  glUseProgram(this->shader_program_lin);

  // Add uniform block to linear shader program
  GLuint blockIndexLin = glGetUniformBlockIndex(this->shader_program_lin, "UBO");
  glUniformBlockBinding(this->shader_program_lin, blockIndexLin,
                        0);

  this->shader_program_log = create_shader_program(vs_name.c_str(), fs_log_name.c_str());
  glUseProgram(this->shader_program_log);

  // Add uniform block to log shader program
  GLuint blockIndexLog = glGetUniformBlockIndex(this->shader_program_log, "UBO");
  glUniformBlockBinding(this->shader_program_log, blockIndexLog,
                        0);

  // Get uniform locations and store them for later
  this->grid_origin_loc = glGetUniformLocation(this->shader_program_log, "grid_origin");
  this->grid_size_loc = glGetUniformLocation(this->shader_program_log, "grid_size");
  this->density_min_loc = glGetUniformLocation(this->shader_program_log, "density_min");
  this->density_max_loc = glGetUniformLocation(this->shader_program_log, "density_max");
  this->density_texture_loc = glGetUniformLocation(this->shader_program_log, "densityTexture");
  this->color_texture_loc = glGetUniformLocation(this->shader_program_log, "colorMapTexture");

}

void Renderer::update() {
  if (!simulator->sim_is_paused()) {

    // Run a timestep if ready
    this->simulator->advance_single_timestep();

    // Get density min and max
    auto [mass_minimum, mass_maximum] =
      minmax_vec_elems(this->simulator->get_mass_density_ref());
    this->mass_min = mass_minimum;
    this->mass_max = mass_maximum;

    // Bind new density data to texture buffer
    glBindTexture(GL_TEXTURE_3D, this->texture_3D);
    glTexSubImage3D(GL_TEXTURE_3D, 0, 0, 0, 0, this->numgrid, this->numgrid,
                    this->numgrid, GL_RED, GL_FLOAT,
                    this->simulator->get_mass_density());

    // Bind new position data to VBO
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * this->numbods * 3,
                    this->simulator->get_positions());
  }
}

void Renderer::change_color(Color::ColorType color) {
  this->color_map = getColormap(color);
  glTexImage1D(GL_TEXTURE_1D, 0, GL_RGB, this->color_map.size(), 0, GL_RGB,
                 GL_FLOAT, this->color_map.data());
}

void Renderer::display(float aspect_ratio, float mass_clip_factor, bool log_scale) const {

  if (log_scale) {
    glUseProgram(this->shader_program_log);
  } else {
    glUseProgram(this->shader_program_lin);
  }

  // Clipping factor
  float clipped_mass_max = this->mass_max * mass_clip_factor;

  // Update UBO data
  glBindBuffer(GL_UNIFORM_BUFFER, this->UBO);
  glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4),
                  glm::value_ptr(this->camera.get_view_matrix()));
  glBufferSubData(
      GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4),
      glm::value_ptr(this->camera.get_projection_matrix(aspect_ratio)));
  glBindBuffer(GL_UNIFORM_BUFFER, 0);

  // Grid parameters
  glm::vec3 grid_origin(0.0f, 0.0f, 0.0f);
  glm::vec3 grid_size(this->gridlen, this->gridlen, this->gridlen);
  glUniform3fv(this->grid_origin_loc, 1,glm::value_ptr(grid_origin));
  glUniform3fv(this->grid_size_loc, 1, glm::value_ptr(grid_size));
  glUniform1f(this->density_min_loc, this->mass_min);
  glUniform1f(this->density_max_loc, clipped_mass_max);

  // Bind textures
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_3D, this->texture_3D);
  glUniform1i(this->density_texture_loc, 0);

  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_1D, this->texture_color);
  glUniform1i(this->color_texture_loc, 1);

  // Bind VAO and draw
  glBindVertexArray(this->VAO);
  glDrawArrays(GL_POINTS, 0, this->numbods);
  glBindVertexArray(0);
}

void Renderer::reset_simulator() {
  this->simulator.reset();
  this->simulator = std::make_unique<ParticleMeshSimulator>();
}

Renderer::~Renderer() {
  if (glIsVertexArray(this->VAO)) {
    glBindVertexArray(this->VAO);
    glDisableVertexAttribArray(0);
    glDeleteVertexArrays(1, &this->VAO);
  }
  if (glIsTexture(this->texture_3D)) {
    glBindTexture(GL_TEXTURE_3D, 0);
    glDeleteTextures(1, &this->texture_3D);
  }
  if (glIsTexture(this->texture_color)) {
    glBindTexture(GL_TEXTURE_1D, 0);
    glDeleteTextures(1, &this->texture_color);
  }
  if (glIsProgram(this->shader_program_lin)) {
    glUseProgram(0);
    glDeleteProgram(this->shader_program_lin);
  }
  if (glIsProgram(this->shader_program_log)) {
    glUseProgram(0);
    glDeleteProgram(this->shader_program_log);
  }
  if (glIsBuffer(this->VBO)) glDeleteBuffers(1, &this->VBO);
  if (glIsBuffer(this->UBO)) glDeleteBuffers(1, &this->UBO);
}

GLuint Renderer::compile_shader(GLenum type, const char *path) {
  std::ifstream file(path);
  if (!file.is_open()) {
    throw std::runtime_error("Failed to open shader file: " + std::string(path));
  }
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
    constexpr size_t LOG_SIZE = 512;
    char info[LOG_SIZE];
    glGetShaderInfoLog(shader, LOG_SIZE, nullptr, info);
    std::cerr << "Shader compile error: " << info << std::endl;
    throw std::runtime_error("");
  }

  return shader;
}

GLuint Renderer::create_shader_program(const char *vertexPath,
                                     const char *fragmentPath) {
  GLuint vs = compile_shader(GL_VERTEX_SHADER, vertexPath);
  GLuint fs = compile_shader(GL_FRAGMENT_SHADER, fragmentPath);

  GLuint program = glCreateProgram();
  glAttachShader(program, vs);
  glAttachShader(program, fs);
  glLinkProgram(program);

  // Error checking
  GLint success;
  glGetProgramiv(program, GL_LINK_STATUS, &success);
  if (!success) {
    constexpr size_t LOG_SIZE = 512;
    char info[LOG_SIZE];
    glGetProgramInfoLog(program, LOG_SIZE, nullptr, info);
    std::cerr << "Shader link error: " << info << std::endl;
    throw std::runtime_error("");
  }

  glDeleteShader(vs);
  glDeleteShader(fs);
  return program;
}

