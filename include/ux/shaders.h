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
        GLuint uProjectionMatrixLocation;
        GLuint uColorLocation;

    public:
        ColorShaderProgram(std::string vshPath, std::string fshPath) : ShaderProgram(vshPath, fshPath) {
            aPositionLocation = glGetAttribLocation(program, "position");
            uColorLocation = glGetUniformLocation(program, "color");
            uProjectionMatrixLocation = glGetUniformLocation(program, "projectionMatrix");
        }

        void setUniforms(glm::mat4 projection, glm::vec4 color) {
            glUniformMatrix4fv( uProjectionMatrixLocation, 1, GL_FALSE, glm::value_ptr( projection ) );
            glUniform4fv( uColorLocation, 1, glm::value_ptr( color ) );
        }

        GLuint getPositionAttributeLocation() {
            return aPositionLocation;
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

class TextShaderProgram : public ShaderProgram {
    private:
        GLuint aCoordLocation;
        GLuint uProjectionMatrixLocation;
        GLuint uTextureUnitLocation;
        GLuint uColorLocation;

    public:
        TextShaderProgram(std::string vshPath, std::string fshPath) : ShaderProgram(vshPath, fshPath) {
            aCoordLocation = glGetAttribLocation(program, "coord");
            uColorLocation = glGetUniformLocation(program, "color");
            uProjectionMatrixLocation = glGetUniformLocation(program, "projection");
            uTextureUnitLocation = glGetUniformLocation(program, "tex");
        }

        void setUniforms(GLuint textureId, glm::mat4 projection, glm::vec4 color) {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, textureId);
            glUniform1i(uTextureUnitLocation, 0);
            glUniform4fv( uColorLocation, 1, glm::value_ptr( color ) );
            glUniformMatrix4fv( uProjectionMatrixLocation, 1, GL_FALSE, glm::value_ptr( projection ) );
        }

        GLuint getCoordAttributeLocation() {
            return aCoordLocation;
        }
};
