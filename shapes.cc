#include <memory>
#include <cstdio>
#include <iostream>

#include <ux/shapes.h>

ColorQuad::ColorQuad() {
    std::unique_ptr<VertexArray> tmp(new VertexArray((const GLfloat *)quadVertices, sizeof(quadVertices)));
    vertexArray = std::move(tmp);
}

void ColorQuad::bindData(ColorShaderProgram *shaderProgram) {
    vertexArray->bindBuffer();

    vertexArray->setVertexAttribPointer(
            0,
            shaderProgram->getPositionAttributeLocation(),
            4,
            0);

    vertexArray->setVertexAttribPointer(
            (void *)64,
            shaderProgram->getColorAttributeLocation(),
            4,
            0);
}

void ColorQuad::draw() {
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    
    for (GLenum err = glGetError(); err != GL_NO_ERROR; err = glGetError()) {
        fprintf(stderr, "%d: %s\n", err, gluErrorString(err));
    }

    vertexArray->unbindBuffer();
    glUseProgram(0);
}

TextureQuad::TextureQuad() {
    std::unique_ptr<VertexArray> tmp(new VertexArray((const GLfloat *)textureQuadVertices, sizeof(textureQuadVertices)));
    vertexArray = std::move(tmp);
}

void TextureQuad::bindData(TextureShaderProgram *shaderProgram) {
    vertexArray->bindBuffer();

    int positionComponentCount = 2;
    int textureComponentCount = 2;
    int stride = (positionComponentCount + textureComponentCount) * 4;


    vertexArray->setVertexAttribPointer(
            0,
            shaderProgram->getPositionAttributeLocation(),
            2,
            stride);

    vertexArray->setVertexAttribPointer(
            (void *)2, //positionComponentCount
            shaderProgram->getTextureCoordinatesAttributeLocation(),
            textureComponentCount,
            stride);
}

void TextureQuad::draw() {
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    
    for (GLenum err = glGetError(); err != GL_NO_ERROR; err = glGetError()) {
        fprintf(stderr, "%d: %s\n", err, gluErrorString(err));
    }

    vertexArray->unbindBuffer();
    glUseProgram(0);
}

