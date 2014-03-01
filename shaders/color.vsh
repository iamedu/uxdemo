#version 130

uniform mat4 projectionMatrix;
in vec4 position;
in vec4 color;

smooth out vec4 vColor;

void main() {
    gl_Position = projectionMatrix * position;
    vColor = color;
}
