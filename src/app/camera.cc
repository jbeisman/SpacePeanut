
#include "camera.hh"
#include <glm/gtc/matrix_transform.hpp>

template <typename T> int sgn(T val) { return (T(0) < val) - (val < T(0)); }

Camera::Camera(glm::vec3 camera_position, glm::vec3 look_at, glm::vec3 up_vec)
    : cam_pos(std::move(camera_position)), look_at(std::move(look_at)),
      up_vector(std::move(up_vec)) {
  update_view_matrix();
}

void Camera::start_drag(const glm::vec2 &mouse_pos) {
  is_drag = true;
  prev_mouse_pos = mouse_pos;
}

void Camera::end_drag() { is_drag = false; }

void Camera::update_drag(const glm::vec2 &mouse_pos,
                         const glm::vec2 &view_size) {
  if (!is_drag) {
    return;
  }

  glm::vec2 delta;
  delta.x = (2 * M_PI / view_size.x);
  delta.y = (M_PI / view_size.y);

  glm::vec2 angle;
  angle.x = (prev_mouse_pos.x - mouse_pos.x) * delta.x;
  angle.y = (prev_mouse_pos.y - mouse_pos.y) * delta.y;

  float cosine_angle = glm::dot(get_view_direction(), up_vector);
  if (cosine_angle * sgn(delta.y) > 0.99f) {
    delta.y = 0;
  }

  glm::mat4x4 rotation_matrix_x(1.0f);
  glm::mat4x4 rotation_matrix_y(1.0f);

  rotation_matrix_x = glm::rotate(rotation_matrix_x, angle.x, up_vector);
  rotation_matrix_y = glm::rotate(rotation_matrix_y, angle.y, get_right_vec());

  glm::vec4 position(get_cam_pos().x, get_cam_pos().y, get_cam_pos().z, 1);
  glm::vec4 pivot(get_LookAt().x, get_LookAt().y, get_LookAt().z, 1);

  position = (rotation_matrix_x * (position - pivot)) + pivot;
  glm::vec3 new_position = (rotation_matrix_y * (position - pivot)) + pivot;

  set_camera_view(new_position);

  prev_mouse_pos.x = mouse_pos.x;
  prev_mouse_pos.y = mouse_pos.y;
}

void Camera::set_camera_view(glm::vec3 eye) {
  cam_pos = std::move(eye);
  update_view_matrix();
}

void Camera::update_view_matrix() {
  view_matrix = glm::lookAt(cam_pos, look_at, up_vector);
}

glm::mat4x4 Camera::get_view_matrix() const { return view_matrix; }
glm::mat4x4 Camera::get_projection_matrix(float aspect_ratio) const {
  return glm::perspective(glm::radians(60.0f), aspect_ratio, 0.1f, 1000.0f);
}
glm::mat4x4 Camera::get_model_matrix() const { return glm::mat4x4(1.0f); }
glm::vec3 Camera::get_cam_pos() const { return cam_pos; }
glm::vec3 Camera::get_up_vec() const { return up_vector; }
glm::vec3 Camera::get_LookAt() const { return look_at; }
glm::vec3 Camera::get_view_direction() const {
  return -glm::transpose(view_matrix)[2];
}
glm::vec3 Camera::get_right_vec() const {
  return glm::transpose(view_matrix)[0];
}