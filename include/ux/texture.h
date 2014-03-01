#pragma once

#include <ux/gl.h>
#include <string>

GLuint loadTexture(std::string path);
GLuint loadTexture(std::string path, int *width, int *height);
