#include <cstdlib>
#include <cstdio>
#include <cmath>

#include <error.h>

#include <ux/gl.h>
#include <ux/util.h>
#include <ux/shaders.h>
#include <ux/shapes.h>
#include <ux/texture.h>

static ColorShaderProgram *colorProgram;
static TextureShaderProgram *textureProgram;
static ColorQuad *colorQuad;
static TextureQuad *textureQuad;
static int texture;

void init() {
    colorProgram = new ColorShaderProgram("shaders/color.vsh", "shaders/color.fsh");
    textureProgram = new TextureShaderProgram("shaders/texture.vsh", "shaders/texture.fsh");

    colorQuad = new ColorQuad();
    textureQuad = new TextureQuad();

    int w, h;

    // texture = loadTexture("textures/OpenGL_Tutorial_Texture.jpg");
    texture = loadTexture("textures/lunario10_whitebg01.png");

    glEnable( GL_LINE_SMOOTH );
    glEnable( GL_POLYGON_SMOOTH );
    glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
    glHint( GL_POLYGON_SMOOTH_HINT, GL_NICEST );

    // Enable alpha blending.
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void GLUT_display() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // colorProgram->useProgram();
    //
    // colorQuad->bindData(colorProgram);
    // colorQuad->draw();
    
    textureProgram->useProgram();
    textureProgram->setUniforms(texture);
    textureQuad->bindData(textureProgram);
    textureQuad->draw();

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

