
#pragma once

#include "color_palette.hh"
#include <fstream>
#include <memory>
#include <vector>

#include "camera.hh"
#include "simulator.hh"

#include <GL/glew.h>

class Renderer {
public:
  Renderer();
  ~Renderer();
  void init(float RSHIFT, int NSTEPS, int NBODS, int NGRID, float GMAX);
  void change_color(Color::ColorType color);
  void update();
  void display(float aspect_ratio, float mass_clip_factor);
  void reset_simulator();
  std::unique_ptr<ParticleMeshSimulator> simulator;
  std::unique_ptr<Camera> camera;
private:
  GLuint compileShader(GLenum type, const char *path);
  GLuint createShaderProgram(const char *vertexPath, const char *fragmentPath);
  int NUMBODS;
  int NUMGRID;
  float GRIDLEN;
  float mass_min;
  float mass_max;
  std::vector<glm::vec3> colorMap;
  GLuint shaderProgram;
  GLuint texture3D;
  GLuint textureColor;
  GLuint VAO;
  GLuint VBO;
  GLuint UBO;
  GLuint grid_origin_loc;
  GLuint grid_size_loc;
  GLuint density_min_loc;
  GLuint density_max_loc;
  GLuint densityTexture_loc;
  GLuint colorMapTexture_loc;
};
