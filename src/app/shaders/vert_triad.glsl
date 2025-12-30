// vertex shader for rendering in-scene triad
#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;

layout(std140) uniform UBO {
    mat4 view;
    mat4 projection;
};

out vec3 vColor;

void main() {

    vColor = aColor;
    gl_Position = projection * view * vec4(aPos, 1.0);
}
