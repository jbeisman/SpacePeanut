
#pragma once

#include "color_palette.hh"
#include <fstream>
#include <memory>
#include <vector>

#include "camera.hh"
#include "simulator.hh"

#include <GL/glew.h>

class Renderer {
private:
  int NUMBODS, NUMGRID;
  float GRIDLEN;
  GLuint compileShader(GLenum type, const char *path);
  GLuint createShaderProgram(const char *vertexPath, const char *fragmentPath);

public:
  std::unique_ptr<ParticleMeshSimulator> simulator;
  std::unique_ptr<Camera> camera;
  GLuint shaderProgram, texture3D, VAO, VBO, UBO, textureColor;
  std::vector<glm::vec3> colorMap;
  Renderer();
  void init(float RSHIFT, int NSTEPS, int NBODS, int NGRID, float GMAX);
  void run_and_display(bool run, float aspect_ratio, Color::ColorType color,
                       bool change_color);
  void reset_simulator();
};
