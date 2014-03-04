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
        GLuint uAlphaLocation;

    public:
        ColorShaderProgram(std::string vshPath, std::string fshPath) : ShaderProgram(vshPath, fshPath) {
            aPositionLocation = glGetAttribLocation(program, "position");
            uColorLocation = glGetUniformLocation(program, "color");
            uAlphaLocation = glGetUniformLocation(program, "alpha");
            uProjectionMatrixLocation = glGetUniformLocation(program, "projectionMatrix");
        }

        void setUniforms(glm::mat4 projection, glm::vec4 color, float alpha) {
            glUniformMatrix4fv( uProjectionMatrixLocation, 1, GL_FALSE, glm::value_ptr( projection ) );
            glUniform4fv( uColorLocation, 1, glm::value_ptr( color ) );
            glUniform1f(uAlphaLocation, alpha);
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
        GLuint uAlphaLocation;

    public:
        TextureShaderProgram(std::string vshPath, std::string fshPath) : ShaderProgram(vshPath, fshPath) {
            aPositionLocation = glGetAttribLocation(program, "position");
            aTextureCoordinatesLocation = glGetAttribLocation(program, "textureCoordinates");
            uAlphaLocation = glGetUniformLocation(program, "alpha");
            uTextureUnitLocation = glGetUniformLocation(program, "textureUnit");
            uProjectionMatrixLocation = glGetUniformLocation(program, "projectionMatrix");
        }

        void setUniforms(glm::mat4 projection, GLuint textureId, float alpha) {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, textureId);
            glUniform1i(uTextureUnitLocation, 0);
            glUniform1f(uAlphaLocation, alpha);
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
        GLuint uAlphaLocation;

    public:
        TextShaderProgram(std::string vshPath, std::string fshPath) : ShaderProgram(vshPath, fshPath) {
            aCoordLocation = glGetAttribLocation(program, "coord");
            uColorLocation = glGetUniformLocation(program, "color");
            uProjectionMatrixLocation = glGetUniformLocation(program, "projection");
            uAlphaLocation = glGetUniformLocation(program, "alpha");
            uTextureUnitLocation = glGetUniformLocation(program, "tex");
        }

        void setUniforms(GLuint textureId, glm::mat4 projection, glm::vec4 color, float alpha) {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, textureId);
            glUniform1i(uTextureUnitLocation, 0);
            glUniform4fv( uColorLocation, 1, glm::value_ptr( color ) );
            glUniform1f(uAlphaLocation, alpha);
            glUniformMatrix4fv( uProjectionMatrixLocation, 1, GL_FALSE, glm::value_ptr( projection ) );
        }

        void updateColor(glm::vec4 color) {
            glUniform4fv( uColorLocation, 1, glm::value_ptr( color ) );
        }

        GLuint getCoordAttributeLocation() {
            return aCoordLocation;
        }
};
