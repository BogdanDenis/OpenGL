#version 400 core
layout (location = 1) in vec3 position;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float radius;

void main() {
    gl_Position = projection * view * model * vec4 (normalize (position) * radius, 1.0f);
}