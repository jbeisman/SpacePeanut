// fragment shader for point data with log-linear color scheme
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
    float logDensity = log(max(density, 0.0001));
    float logDensityMin = log(max(density_min, 0.0001));
    float logDensityMax = log(max(density_max, 0.0001));
    float normalizedDensity = (logDensity - logDensityMin) / (logDensityMax - logDensityMin);
    normalizedDensity = clamp(normalizedDensity, 0.0, 1.0);
    vec3 color = texture(colorMapTexture, normalizedDensity).rgb;
    FragColor = vec4(color, 1.0);
}
