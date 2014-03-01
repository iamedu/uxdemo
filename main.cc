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
static int textureWidth;
static int textureHeight;
static float backgroundScale = 3.0f;
static float translation = -5.0f;
glm::mat4 projection;

void init() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    colorProgram = new ColorShaderProgram("shaders/color.vsh", "shaders/color.fsh");
    textureProgram = new TextureShaderProgram("shaders/texture.vsh", "shaders/texture.fsh");

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


    glm::mat4 scaleBackground = glm::scale( glm::mat4 (1.0f), glm::vec3(1.0f, (float)textureHeight / (float)textureWidth, 1.0f));
    glm::mat4 scale = glm::scale( glm::mat4 (1.0f), glm::vec3(backgroundScale));
    glm::mat4 translate;
    glm::mat4 transformed = projection * scale * scaleBackground;


    textureProgram->useProgram();
    textureProgram->setUniforms(transformed, texture);
    textureQuad->bindData(textureProgram);
    textureQuad->draw();

    scale = glm::scale( glm::mat4 (1.0f), glm::vec3(3.0f, 0.7f, 1.0f));
    translate = glm::translate( glm::mat4(1.0f), glm::vec3(translation, -0.5f, 0.0f)); 
    transformed = projection * translate * scale;

    glm::vec4 color = glm::vec4(0.0, 0.0, 0.0, 0.8);
    colorProgram->useProgram();
    colorProgram->setUniforms(transformed, color);
    colorQuad->bindData(colorProgram);
    colorQuad->draw();

}

void resize(int w, int h) {
    glViewport(0, 0, w, h);

    float aspectRatio = (float)w / (float)h; 

    projection = glm::ortho( -aspectRatio, aspectRatio, -1.0f, 1.0f, -1.0f, 1.0f );
}

void animate() {
    backgroundScale += 0.0008f;

    if(translation < -0.5f) {
        translation += 0.1f;
    }

}


int main(int argc, char *argv[]) {
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

