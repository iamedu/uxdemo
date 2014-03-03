#version 120

uniform vec4 color;
uniform float alpha;

void main() {
    gl_FragColor = color * vec4(1, 1, 1, alpha);
}

