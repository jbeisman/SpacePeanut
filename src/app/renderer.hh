
#pragma once

#include <fstream>
#include <memory>

#include "simulator.hh"
#include "camera.hh"

#include <GL/glew.h>


class Renderer {
private:
    int NUMBODS;
    GLuint compileShader(GLenum type, const char* path);
    GLuint createShaderProgram(const char* vertexPath, const char* fragmentPath);
public:
    std::unique_ptr<ParticleMeshSimulator> simulator;
    std::unique_ptr<Camera> camera;
    GLuint shaderProgram, VAO, VBO, UBO;
    Renderer();
    void init(float RSHIFT, int NSTEPS, int NBODS, int NGRID, float GMAX);
    void run_and_display(bool run, float aspect_ratio);
};

