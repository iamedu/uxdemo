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
static ColorQuad *background;
static ColorQuad *colorQuad;
static TextureQuad *textureQuad;
static int texture;
static int textureWidth;
static int textureHeight;
static float backgroundScale = 3.0f;
glm::mat4 projection;

void init() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    colorProgram = new ColorShaderProgram("shaders/color.vsh", "shaders/color.fsh");
    textureProgram = new TextureShaderProgram("shaders/texture.vsh", "shaders/texture.fsh");

    background = new ColorQuad();
    colorQuad = new ColorQuad();
    textureQuad = new TextureQuad();

    // texture = loadTexture("textures/OpenGL_Tutorial_Texture.jpg", &w, &h);
    texture = loadTexture("textures/lunario10_whitebg01.png", &textureWidth, &textureHeight);

    glEnable( GL_LINE_SMOOTH );
    glEnable( GL_POLYGON_SMOOTH );
    glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
    glHint( GL_POLYGON_SMOOTH_HINT, GL_NICEST );

    // Enable alpha blending.
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void draw() {
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    glm::vec4 color = glm::vec4(0.0, 0.0, 0.0, 0.9);
    colorProgram->useProgram();
    colorProgram->setUniforms(projection, color);
    colorQuad->bindData(colorProgram);
    colorQuad->draw();

    glm::mat4 scaleBackground = glm::scale( glm::mat4 (1.0f), glm::vec3(1.0f, (float)textureHeight / (float)textureWidth, 1.0f));
    glm::mat4 scale = glm::scale( glm::mat4 (1.0f), glm::vec3(backgroundScale));
    glm::mat4 transformed = projection * scale * scaleBackground;


    textureProgram->useProgram();
    textureProgram->setUniforms(transformed, texture);
    textureQuad->bindData(textureProgram);
    textureQuad->draw();

    // glutSwapBuffers();
}

void resize(int w, int h) {
    glViewport(0, 0, w, h);

    float aspectRatio = (float)w / (float)h; 

    projection = glm::ortho( -aspectRatio, aspectRatio, -1.0f, 1.0f, -1.0f, 1.0f );
}

void animate() {
    backgroundScale += 0.0008f;

    // glutPostRedisplay();
}


int main(int argc, char *argv[]) {

    // glutInit(&argc, argv);
    // glutInitDisplayMode(GLUT_DOUBLE | GLUT_ALPHA | GLUT_STENCIL | GLUT_DEPTH | GLUT_RGBA);
    // glutInitWindowSize(800, 2560);
    // glutInitWindowPosition(200, 100);
    // glutCreateWindow(argv[0]);
    // glutFullScreen();
    //
    // glutIdleFunc( animate );
    // glutDisplayFunc(draw);
    // glutReshapeFunc(resize);
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    const GLFWvidmode *mode = glfwGetVideoMode( glfwGetPrimaryMonitor() );
    window = glfwCreateWindow(mode->width, mode->height, "Hello World", glfwGetPrimaryMonitor(), NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);


    init();
    resize(mode->width, mode->height);
    // glutMainLoop();

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        draw();
        animate();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}

