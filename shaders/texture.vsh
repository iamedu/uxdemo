uniform mat4 projectionMatrix;

attribute vec4 position;  
attribute vec2 textureCoordinates;

varying vec2 v_TextureCoordinates;

void main()     
{    
    v_TextureCoordinates = textureCoordinates;//vec2(position[0], position[1]);    
    gl_Position = projectionMatrix * position;    
}

