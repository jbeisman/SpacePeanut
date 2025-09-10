
#include "renderer.hh"
#include <iostream>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <SDL3/SDL.h>

#include <filesystem>
#include <stdexcept>

GLuint Renderer::compileShader(GLenum type, const char* path) {
    std::ifstream file(path);
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string srcStr = buffer.str();
    const char* src = srcStr.c_str();

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

GLuint Renderer::createShaderProgram(const char* vertexPath, const char* fragmentPath) {
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

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void Renderer::init(float RSHIFT, int NSTEPS, int NBODS, int NGRID, float GMAX) {
    this->NUMBODS = NBODS;
    this->camera = std::make_unique<Camera>(glm::vec3(GMAX/2,GMAX/2,-1*GMAX),
                                            glm::vec3(GMAX/2,GMAX/2,GMAX/2),
                                            glm::vec3(0.0f,1.0f,0.0f));
    
    this->simulator->initialize_simulation(RSHIFT, NSTEPS, NBODS, NGRID, GMAX);
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
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * NBODS * 3, vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void *)0);

    // Setup uniform buffer
    glBindBuffer(GL_UNIFORM_BUFFER, this->UBO);
    glBufferData(GL_UNIFORM_BUFFER, 3 * sizeof(glm::mat4), nullptr, GL_DYNAMIC_DRAW); // Allocate space only
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
    glUniformBlockBinding(this->shaderProgram, blockIndex, 0); // 0 = binding point
}


void Renderer::run_and_display(bool run, float aspect_ratio) {

    if (run) {
        this->simulator->advance_single_timestep();
    }

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
    glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), sizeof(glm::mat4),
        glm::value_ptr(this->camera->get_projection_matrix(aspect_ratio)));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    auto *vertices = simulator->get_positions();
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * this->NUMBODS * 3, vertices);

    glBindVertexArray(this->VAO);
    glDrawArrays(GL_POINTS, 0, this->NUMBODS);
    glBindVertexArray(0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Reset to normal mode
}
