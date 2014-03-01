uniform sampler2D textureUnit;    
varying vec2 v_TextureCoordinates;    

void main()     
{    
    gl_FragColor = texture2D(textureUnit, v_TextureCoordinates);
}

