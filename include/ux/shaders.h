#pragma once

#include <string>
#include <iostream>
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

class TextureShaderProgram : public ShaderProgram {
    private:
        GLuint aPositionLocation;
        GLuint aTextureCoordinatesLocation;
        GLuint uTextureUnitLocation;

    public:
        TextureShaderProgram(std::string vshPath, std::string fshPath) : ShaderProgram(vshPath, fshPath) {
            aPositionLocation = glGetAttribLocation(program, "position");
            aTextureCoordinatesLocation = glGetAttribLocation(program, "textureCoordinates");
            uTextureUnitLocation = glGetUniformLocation(program, "textureUnit");
        }

        void setUniforms(GLuint textureId) {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, textureId);
            glUniform1i(uTextureUnitLocation, 0);
        }

        GLuint getPositionAttributeLocation() {
            return aPositionLocation;
        }

        GLuint getTextureCoordinatesAttributeLocation() {
            return aTextureCoordinatesLocation;
        }
};

