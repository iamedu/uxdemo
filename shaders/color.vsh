#version 130

in vec4 position;
in vec4 color;

smooth out vec4 vColor;

void main() {
    gl_Position = position;
    vColor = color;
}
