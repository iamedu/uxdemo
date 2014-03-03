uniform sampler2D textureUnit;    
varying vec2 v_TextureCoordinates;    
uniform float alpha;

void main()     
{    
    gl_FragColor = texture2D(textureUnit, v_TextureCoordinates) * vec4(1, 1, 1, alpha);
}

