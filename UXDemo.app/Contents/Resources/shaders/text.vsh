#version 120

uniform mat4 projection;
attribute vec4 coord;
varying vec2 texcoord;

void main(void) {
    gl_Position = projection * vec4(coord.xy, 0, 1);
    texcoord = coord.zw;
}

