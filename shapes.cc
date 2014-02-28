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

    GLuint sLocPosition = shaderProgram->getPositionAttributeLocation();
    GLuint sLocColor = shaderProgram->getColorAttributeLocation();

    glEnableVertexAttribArray(sLocPosition);
    glVertexAttribPointer(sLocPosition, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(sLocColor);
    glVertexAttribPointer(sLocColor, 4, GL_FLOAT, GL_FALSE, 0, (void *)64);

    // vertexArray->setVertexAttribPointer(
    //         0,
    //         shaderProgram->getPositionAttributeLocation(),
    //         4,
    //         0);
    //
    // vertexArray->setVertexAttribPointer(
    //         (void *)64,
    //         shaderProgram->getColorAttributeLocation(),
    //         4,
    //         0);
}

void ColorQuad::draw() {
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    
    for (GLenum err = glGetError(); err != GL_NO_ERROR; err = glGetError()) {
        fprintf(stderr, "%d: %s\n", err, gluErrorString(err));
    }

    vertexArray->unbindBuffer();
    glUseProgram(0);
}

