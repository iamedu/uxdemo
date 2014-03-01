#pragma once

#include <ux/gl.h>
#include <string>

GLuint loadTexture(GLenum texture, std::string path);
GLuint loadTexture(GLenum texture, std::string path, int *width, int *height);
