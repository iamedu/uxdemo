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
        GLuint uProjectionMatrixLocation;

    public:
        ColorShaderProgram(std::string vshPath, std::string fshPath) : ShaderProgram(vshPath, fshPath) {
            aPositionLocation = glGetAttribLocation(program, "position");
            aColorLocation = glGetAttribLocation(program, "color");
            uProjectionMatrixLocation = glGetUniformLocation(program, "projectionMatrix");
        }

        void setUniforms(glm::mat4 projection) {
            glUniformMatrix4fv( uProjectionMatrixLocation, 1, GL_FALSE, glm::value_ptr( projection ) );
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
        GLuint uProjectionMatrixLocation;

    public:
        TextureShaderProgram(std::string vshPath, std::string fshPath) : ShaderProgram(vshPath, fshPath) {
            aPositionLocation = glGetAttribLocation(program, "position");
            aTextureCoordinatesLocation = glGetAttribLocation(program, "textureCoordinates");
            uTextureUnitLocation = glGetUniformLocation(program, "textureUnit");
            uProjectionMatrixLocation = glGetUniformLocation(program, "projectionMatrix");
        }

        void setUniforms(glm::mat4 projection, GLuint textureId) {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, textureId);
            glUniform1i(uTextureUnitLocation, 0);
            glUniformMatrix4fv( uProjectionMatrixLocation, 1, GL_FALSE, glm::value_ptr( projection ) );
        }

        GLuint getPositionAttributeLocation() {
            return aPositionLocation;
        }

        GLuint getTextureCoordinatesAttributeLocation() {
            return aTextureCoordinatesLocation;
        }
};

