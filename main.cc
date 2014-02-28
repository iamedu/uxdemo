#include <cstdlib>
#include <cstdio>
#include <cmath>

#include <error.h>

#include <ux/util.h>

#define VERTEX_SHADER " \
  #version 130\n \
  in vec4 position; \
  in vec4 color; \
  smooth out vec4 vColor; \
  void main() { \
  gl_Position = position; \
    vColor = color; \
  }"

#define FRAGMENT_SHADER " \
  #version 130\n \
  smooth in vec4 vColor; \
  void main() { \
    gl_FragColor = vColor; \
  }"

const GLfloat sValues[] = {
  0.0f, 0.5f, 0.0f, 1.0f,
  -0.5f, -0.5f, 0.0f, 1.0f,
  0.5f, -0.5f, 0.0f, 1.0f,
  1.0f, 0.0f, 0.0f, 1.0f,
  0.0f, 1.0f, 0.0f, 1.0f,
  0.0f, 0.0f, 1.0f, 1.0f,
};

static GLuint sProgram;
static GLuint sLocPosition;
static GLuint sLocColor;
static GLuint sValuesBuffer;

GLuint CreateShader(GLenum shaderType, const char* shaderSource) {
  GLuint shader = glCreateShader(shaderType);
  glShaderSource(shader, 1, (const GLchar **)&shaderSource, NULL);
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

void Init() {
    GLuint vertexShader = CreateShader(GL_VERTEX_SHADER, VERTEX_SHADER);
    GLuint fragmentShader = CreateShader(GL_FRAGMENT_SHADER, FRAGMENT_SHADER);
    sProgram = CreateProgram(vertexShader, fragmentShader);

    sLocPosition = glGetAttribLocation(sProgram, "position");
    sLocColor = glGetAttribLocation(sProgram, "color");

    glGenBuffers(1, &sValuesBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, sValuesBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(sValues), sValues, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void GLUT_display() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(sProgram);

    glBindBuffer(GL_ARRAY_BUFFER, sValuesBuffer);
    glEnableVertexAttribArray(sLocPosition);
    glVertexAttribPointer(sLocPosition, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(sLocColor);
    glVertexAttribPointer(sLocColor, 4, GL_FLOAT, GL_FALSE, 0, (void *)48);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    for (GLenum err = glGetError(); err != GL_NO_ERROR; err = glGetError()) {
        fprintf(stderr, "%d: %s\n", err, gluErrorString(err));
    }

    glDisableVertexAttribArray(sLocPosition);
    glDisableVertexAttribArray(sLocColor);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glUseProgram(0);

    glutSwapBuffers();
}

void GLUT_reshape(int w, int h) {
    glViewport(0, 0, w, h);
}


int main(int argc, char *argv[]) {

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH | GLUT_STENCIL);
    //glutInitWindowSize(800, 2560);
    //glutInitWindowPosition(200, 100);
    glutCreateWindow(argv[0]);
    glutFullScreen();

    glutDisplayFunc(GLUT_display);
    glutReshapeFunc(GLUT_reshape);

    Init();
    glutMainLoop();

    return 0;
}

