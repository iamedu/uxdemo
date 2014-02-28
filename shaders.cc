#include <ux/gl.h>
#include <ux/shaders.h>
#include <ux/util.h>

GLuint CreateShader(GLenum shaderType, std::string shaderSource) {
    GLuint shader = glCreateShader(shaderType);
    const char * source = shaderSource.data();
    glShaderSource(shader, 1, (const GLchar **)&source, NULL);

    glCompileShader(shader);

    CHECK_GL_STATUS(Shader, shader, GL_COMPILE_STATUS);

    return shader;
}

GLuint CreateProgram(GLuint vertexShader, GLuint fragmentShader) {
    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    CHECK_GL_STATUS(Program, program, GL_LINK_STATUS);

    return program;
}

ShaderProgram::ShaderProgram(std::string vshPath, std::string fshPath) {
    std::string vshSource = readFile(vshPath);
    std::string fshSource = readFile(fshPath);

    GLuint vshShader = CreateShader(GL_VERTEX_SHADER, vshSource);
    GLuint fshShader = CreateShader(GL_FRAGMENT_SHADER, fshSource);

    program = CreateProgram(vshShader, fshShader);
}

void ShaderProgram::useProgram() {
    glUseProgram(program);
}

