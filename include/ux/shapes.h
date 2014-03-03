#pragma once

#include <memory>

#include <ux/gl.h>
#include <ux/shaders.h>
#include <ux/vertex.h>

const GLfloat quadVertices[] = {
    -0.5f, 0.5f, 0.0f, 1.0f,
    -0.5f, -0.5f, 0.0f, 1.0f,
    0.5f, 0.5f, 0.0f, 1.0f,
    0.5f, -0.5f, 0.0f, 1.0f,
};

const GLfloat textureQuadVertices[] = {
    -0.5f, 0.5f, 0.0f, 0.0f,
    -0.5f, -0.5f, 0.0f, 1.0f,
    0.5f, 0.5f, 1.0f, 0.0f,
    0.5f, -0.5f, 1.0f, 1.0f
};

class ColorQuad {
    private:
        VertexArray *vertexArray;
    public:
        ColorQuad();
        void bindData(ColorShaderProgram *shaderProgram);
        void draw();
};

class TextureQuad {
    private:
        VertexArray *vertexArray;
    public:
        TextureQuad();
        void bindData(TextureShaderProgram *shaderProgram);
        void draw();
};

