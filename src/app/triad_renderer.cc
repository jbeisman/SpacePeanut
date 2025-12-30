
#include "renderer.hh"

void Renderer::generate_triad(float GMAX) {

	// Generate and store view matrix for triad viewport
  this->triad_view_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -0.2f * GMAX));

  // Pack triad vertex position and color together
  // x is red, y is green, z is blue
	std::vector<float> triad_data = {
	0.f, 0.f, 0.f,         1.f, 0.f, 0.f,
	-0.1f*GMAX, 0.f, 0.f,  1.f, 0.f, 0.f,

	0.f, 0.f, 0.f,         0.f, 1.f, 0.f,
	0.f, 0.1f*GMAX, 0.f,   0.f, 1.f, 0.f,

	0.f, 0.f, 0.f,         0.f, 0.f, 1.f,
	0.f, 0.f, -0.1f*GMAX,  0.f, 0.f, 1.f
	};

  glGenVertexArrays(1, &this->triad_VAO);
  glGenBuffers(1, &this->triad_VBO);

  glBindVertexArray(this->triad_VAO);
  glBindBuffer(GL_ARRAY_BUFFER, this->triad_VBO);
  glBufferData(GL_ARRAY_BUFFER,
               triad_data.size()*sizeof(float),
               triad_data.data(),
               GL_STATIC_DRAW);

  // Position
  glVertexAttribPointer(
  	0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void*>(0));
  glEnableVertexAttribArray(0);

  // Color
  glVertexAttribPointer(
  	1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // Unbind vertex array
  glBindVertexArray(0);
}


void Renderer::display_triad() {
  
  // Small viewport in corner
  glViewport(0, 0, 100, 100);

  // Use triad shader
  glUseProgram(shader_program_triad);

  // Draw triad over the simulation data
  glClear(GL_DEPTH_BUFFER_BIT);

  // Get view matrix with camera rotation data
  glm::mat4x4 triad_view_rotation_matrix = triad_view_matrix * this->camera.get_rotation_matrix();
  
  // Update UBO data
  glBindBuffer(GL_UNIFORM_BUFFER, this->UBO);
  glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4),
    glm::value_ptr(triad_view_rotation_matrix));
  glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4),
    glm::value_ptr(this->camera.get_projection_matrix(1.0f)));
  glBindBuffer(GL_UNIFORM_BUFFER, 0);

  // Draw triad
  glBindVertexArray(this->triad_VAO);
  glLineWidth(3.0f);
  glDrawArrays(GL_LINES, 0, 6);
  glBindVertexArray(0);
}

