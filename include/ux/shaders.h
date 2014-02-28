#pragma once

#include <string>
#include <ux/gl.h>

GLuint CreateShader(GLenum shaderType, std::string shaderSource);
GLuint CreateProgram(GLuint vertexShader, GLuint fragmentShader);

class ShaderProgram {
    protected:
        GLuint program;
    public:
        ShaderProgram(std::string vshPath, std::string fshPath);
        void useProgram();
};

class ColorShaderProgram : public ShaderProgram {
    private:
        GLuint aPositionLocation;
        GLuint aColorLocation;

    public:
        ColorShaderProgram(std::string vshPath, std::string fshPath) : ShaderProgram(vshPath, fshPath) {
            aPositionLocation = glGetAttribLocation(program, "position");
            aColorLocation = glGetAttribLocation(program, "color");
        }

        GLuint getPositionAttributeLocation() {
            return aPositionLocation;
        }

        GLuint getColorAttributeLocation() {
            return aColorLocation;
        }
};

