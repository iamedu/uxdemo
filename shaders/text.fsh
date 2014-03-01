#version 120

varying vec2 texcoord;
uniform sampler2D tex;
uniform vec4 color;
uniform float alpha;

void main(void) {
    gl_FragColor = vec4(1, 1, 1, texture2D(tex, texcoord).a) * color * vec4(1, 1, 1, alpha);
}
