// fragment shader for point data with linear color scheme
#version 330 core

in vec3 vPos;

uniform sampler3D densityTexture;
uniform sampler1D colorMapTexture;

uniform vec3 grid_origin;
uniform vec3 grid_size;
uniform float density_min;
uniform float density_max;

out vec4 FragColor;

void main() {

    vec3 texCoord = (vPos - grid_origin) / grid_size;
    float density = texture(densityTexture, texCoord).r;
    float normalizedDensity = (density - density_min) / (density_max - density_min);
    normalizedDensity = clamp(normalizedDensity, 0.0, 1.0);
    vec3 color = texture(colorMapTexture, normalizedDensity).rgb;
    FragColor = vec4(color, 1.0);
}
