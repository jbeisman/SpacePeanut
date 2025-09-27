
#pragma once

#include "color_palette.hh"
#include "camera.hh"
#include "simulator.hh"

#include <fstream>
#include <memory>
#include <vector>

#include <GL/glew.h>

class Renderer {
public:
  Renderer();
  ~Renderer();
  void init(float RSHIFT, int NSTEPS, int NBODS, int NGRID, float GMAX);
  void change_color(Color::ColorType color);
  void update();
  void display(float aspect_ratio, float mass_clip_factor, bool log_scale) const;
  void reset_simulator();
  std::unique_ptr<ParticleMeshSimulator> simulator;
  Camera camera;
private:
  GLuint compile_shader(GLenum type, const char *path);
  GLuint create_shader_program(const char *vertexPath, const char *fragmentPath);
  int numbods;
  int numgrid;
  float gridlen;
  float mass_min;
  float mass_max;
  std::vector<glm::vec3> color_map;
  GLuint shader_program_log;
  GLuint shader_program_lin;
  GLuint texture_3D;
  GLuint texture_color;
  GLuint VAO;
  GLuint VBO;
  GLuint UBO;
  GLuint grid_origin_loc;
  GLuint grid_size_loc;
  GLuint density_min_loc;
  GLuint density_max_loc;
  GLuint density_texture_loc;
  GLuint color_texture_loc;
};
