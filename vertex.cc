#include <ux/vertex.h>

VertexArray::VertexArray(const GLfloat* values, int totalComponents) {
    glGenBuffers(1, &valuesBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, valuesBuffer);
    glBufferData(GL_ARRAY_BUFFER, totalComponents, values, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexArray::bindBuffer() {
    glBindBuffer(GL_ARRAY_BUFFER, valuesBuffer);
}

void VertexArray::setVertexAttribPointer(void *dataOffset, int attributeLocation, int componentCount, int stride) {
    glEnableVertexAttribArray(attributeLocation);
    glVertexAttribPointer(attributeLocation, componentCount, GL_FLOAT, GL_FALSE, stride, dataOffset);
}

void VertexArray::unbindBuffer() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

