#include <cstdlib>
#include <cstdio>
#include <cmath>

#include <error.h>

#include <ux/gl.h>
#include <ux/util.h>
#include <ux/shaders.h>
#include <ux/shapes.h>

static ColorShaderProgram *colorProgram;
static ColorQuad *colorQuad;

void init() {
    colorProgram = new ColorShaderProgram("shaders/color.vsh", "shaders/color.fsh");

    colorQuad = new ColorQuad();

    glEnable( GL_LINE_SMOOTH );
    glEnable( GL_POLYGON_SMOOTH );
    glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
    glHint( GL_POLYGON_SMOOTH_HINT, GL_NICEST );
}

void GLUT_display() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    colorProgram->useProgram();

    colorQuad->bindData(colorProgram);
    colorQuad->draw();

    glutSwapBuffers();
}

void GLUT_reshape(int w, int h) {
    glViewport(0, 0, w, h);
}


int main(int argc, char *argv[]) {

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH | GLUT_STENCIL);
    // glutInitWindowSize(800, 2560);
    // glutInitWindowPosition(200, 100);
    glutCreateWindow(argv[0]);
    glutFullScreen();

    glutDisplayFunc(GLUT_display);
    glutReshapeFunc(GLUT_reshape);

    init();
    glutMainLoop();

    return 0;
}

