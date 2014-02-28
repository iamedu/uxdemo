#pragma once

#include <ux/gl.h>

class VertexArray {
    private:
        GLuint valuesBuffer;
    public:
        VertexArray(const GLfloat* values, int totalComponents);
        void setVertexAttribPointer(void *dataOffset, int attributeLocation, int componentCount, int stride);
        void bindBuffer();
        void unbindBuffer();
};
