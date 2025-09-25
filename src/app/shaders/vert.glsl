// vertex_shader.glsl
#version 330 core

layout(location = 0) in vec3 aPos;

layout(std140) uniform UBO {
    mat4 view;
    mat4 projection;
};

out vec3 FragPos;

void main() {

    gl_PointSize = 2.0;
    gl_Position = projection * view * vec4(aPos, 1.0);
    FragPos = aPos;
}
