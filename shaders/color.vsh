#version 130

uniform mat4 projectionMatrix;
in vec4 position;

void main() {
    gl_Position = projectionMatrix * position;
}
