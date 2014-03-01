#include <cstdlib>
#include <cstdio>
#include <cmath>

#include <error.h>

#include <ux/gl.h>
#include <ux/util.h>
#include <ux/shaders.h>
#include <ux/shapes.h>
#include <ux/texture.h>

#include <ft2build.h>
#include FT_FREETYPE_H

static ColorShaderProgram *colorProgram;
static TextureShaderProgram *textureProgram;
static TextShaderProgram *textProgram;
static ColorQuad *colorQuad;
static TextureQuad *textureQuad;
static int textureCache[100];
static int whiteTexture;
static int blackTexture;
static int textureWidth;
static int textureHeight;
static int frame = 1;
static int direction = -1;
static double lastTime;
static float backgroundScale = 3.0f;
static float translation = -5.0f;
static float alpha = 1.0f;
static glm::mat4 projection;
//TEXT
static FT_Face face;
static GLuint tex;
static GLuint vbo;

void start_text();
void render_text(const char *text, float x, float y, float sx, float sy);

void init() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    colorProgram = new ColorShaderProgram("shaders/color.vsh", "shaders/color.fsh");
    textureProgram = new TextureShaderProgram("shaders/texture.vsh", "shaders/texture.fsh");
    textProgram = new TextShaderProgram("shaders/text.vsh", "shaders/text.fsh");

    colorQuad = new ColorQuad();
    textureQuad = new TextureQuad();

    /*
     *
     * Text gets up to here
     *
     *
     *
     */

    FT_Library ft;

    if(FT_Init_FreeType(&ft)) {
        fprintf(stderr, "Could not init freetype library\n");
        exit(1);
    }

    if(FT_New_Face(ft, "fonts/Replica-Regular.ttf", 0, &face)) {
        fprintf(stderr, "Could not open font\n");
        exit(1);
    }

    FT_Set_Pixel_Sizes(face, 0, 48);

    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glGenBuffers(1, &vbo);


    /*
     *
     * Text gets up to here
     *
     *
     *
     */

    whiteTexture = loadTexture("textures/lunario10_whitebg01.png", &textureWidth, &textureHeight);
    blackTexture = loadTexture("textures/lunario10_blackbg01.png", &textureWidth, &textureHeight);

    glEnable( GL_LINE_SMOOTH );
    glEnable( GL_POLYGON_SMOOTH );
    glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
    glHint( GL_POLYGON_SMOOTH_HINT, GL_NICEST );

    // Enable alpha blending.
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glfwSetTime(0);
}


void draw1(float alpha) {
    glm::mat4 scaleBackground = glm::scale( glm::mat4 (1.0f), glm::vec3(1.0f, (float)textureHeight / (float)textureWidth, 1.0f));
    glm::mat4 scale = glm::scale( glm::mat4 (1.0f), glm::vec3(backgroundScale));
    glm::mat4 translate;
    glm::mat4 transformed = projection * scale * scaleBackground;


    textureProgram->useProgram();
    textureProgram->setUniforms(transformed, whiteTexture, alpha);
    textureQuad->bindData(textureProgram);
    textureQuad->draw();

    scale = glm::scale( glm::mat4 (1.0f), glm::vec3(3.0f, 0.7f, 1.0f));
    translate = glm::translate( glm::mat4(1.0f), glm::vec3(translation, -0.5f, 0.0f)); 
    transformed = projection * translate * scale;

    glm::vec4 color = glm::vec4(0.0, 0.0, 0.0, 0.8);
    colorProgram->useProgram();
    colorProgram->setUniforms(transformed, color, alpha);
    colorQuad->bindData(colorProgram);
    colorQuad->draw();


    float sx = 2.0 / 1024;
    float sy = 2.0 / 768;
    color = glm::vec4(1.0, 1.0, 1.0, 1.0);
    translate = glm::translate( glm::mat4(2.0f), glm::vec3(0.45f + translation, -0.1f, 0.0f)); 
    textProgram->useProgram();
    textProgram->setUniforms(tex, translate, color, alpha);

    start_text();
    FT_Set_Pixel_Sizes(face, 0, 32);
    render_text("Celebren con nosotros!! ",
            -1 + 8 * sx,   1 - 480 * sy,   sx, sy);

    color = glm::vec4(0.93, 0.25, 0.21, 1.0);
    textProgram->setUniforms(tex, translate, color, alpha);
    FT_Set_Pixel_Sizes(face, 0, 48);
    render_text("#LUNARIO10 ",
            -1 + 240 * sx,   1 - 540 * sy,   sx, sy);

}

void draw2(float alpha) {
    glm::mat4 scaleBackground = glm::scale( glm::mat4 (1.0f), glm::vec3(1.0f, (float)textureHeight / (float)textureWidth, 1.0f));
    glm::mat4 scale = glm::scale( glm::mat4 (1.0f), glm::vec3(backgroundScale));
    glm::mat4 translate;
    glm::mat4 transformed = projection * scale * scaleBackground;


    textureProgram->useProgram();
    textureProgram->setUniforms(transformed, blackTexture, alpha);
    textureQuad->bindData(textureProgram);
    textureQuad->draw();

    scale = glm::scale( glm::mat4 (1.0f), glm::vec3(3.0f, 0.7f, 1.0f));
    translate = glm::translate( glm::mat4(1.0f), glm::vec3(translation, -0.5f, 0.0f)); 
    transformed = projection * translate * scale;

    glm::vec4 color = glm::vec4(1.0, 1.0, 1.0, 0.8);
    colorProgram->useProgram();
    colorProgram->setUniforms(transformed, color, alpha);
    colorQuad->bindData(colorProgram);
    colorQuad->draw();


    float sx = 2.0 / 1024;
    float sy = 2.0 / 768;
    color = glm::vec4(0.0, 0.0, 0.0, 1.0);
    translate = glm::translate( glm::mat4(1.0f), glm::vec3(0.45f + translation, -0.1f, 0.0f)); 
    textProgram->useProgram();
    textProgram->setUniforms(tex, translate, color, alpha);

    start_text();
    FT_Set_Pixel_Sizes(face, 0, 32);
    render_text("Celebren con nosotros!! ",
            -1 + 8 * sx,   1 - 480 * sy,   sx, sy);

    color = glm::vec4(0.93, 0.25, 0.21, 1.0);
    textProgram->setUniforms(tex, translate, color, alpha);
    FT_Set_Pixel_Sizes(face, 0, 48);
    render_text("#LUNARIO10 ",
            -1 + 240 * sx,   1 - 540 * sy,   sx, sy);

}

void video(float alpha) {
    glm::mat4 scale;
    glm::mat4 translate;
    glm::mat4 transformed;
    
    std::stringstream path;

    double currentTime = glfwGetTime();

    if(currentTime - lastTime >= 1.0) {
        frame += (int)(lastTime - currentTime);
    }

    lastTime = glfwGetTime();

    char num[4];

    sprintf(num, "%03d", frame);

    path << "videos/f65e6f845a2711e3ba591293cad68979_101/image-" << num << ".jpeg";

    std::ifstream f(path.str());

    if(!f.good()) {
        frame = 1;
        path.str("");
        path << "videos/f65e6f845a2711e3ba591293cad68979_101/image-001.jpeg";
        glfwSetTime(0);
    }

    int w, h;
    int videoTexture = loadTexture(path.str(), &w, &h);

    scale = glm::scale( glm::mat4 (1.0f), glm::vec3(2.0f, 2.0f, 1.0f));
    translate = glm::translate( glm::mat4(1.0f), glm::vec3(-0.4, 0.0f, 0.0f)); 
    transformed = projection * scale * translate;

    textureProgram->useProgram();
    textureProgram->setUniforms(transformed, videoTexture, alpha);
    textureQuad->bindData(textureProgram);
    textureQuad->draw();

    frame += 1;

    glDeleteTextures(1, (const GLuint*)&videoTexture);

    scale = glm::scale( glm::mat4 (1.0f), glm::vec3(2.0f, 3.0f, 1.0f));
    translate = glm::translate( glm::mat4(1.0f), glm::vec3(1.2f, 0.0f, 0.0f)); 
    transformed = projection * translate * scale;

    glm::vec4 color = glm::vec4(0.93, 0.25, 0.21, 1.0);

    colorProgram->useProgram();
    colorProgram->setUniforms(transformed, color, alpha);
    colorQuad->bindData(colorProgram);
    colorQuad->draw();

    scale = glm::scale( glm::mat4 (1.0f), glm::vec3(4.0f, 4.0f, 1.0f));
    translate = glm::translate( glm::mat4(1.0f), glm::vec3(-1.5f, -1.0f, 0.0f)); 
    transformed = projection * translate * scale;
    colorProgram->useProgram();
    colorProgram->setUniforms(transformed, color, 0.3);
    colorQuad->bindData(colorProgram);
    colorQuad->draw();

    float sx = 2.0 / 1024;
    float sy = 2.0 / 768;
    color = glm::vec4(1.0, 1.0, 1.0, 1.0);
    translate = glm::translate( glm::mat4(1.0f), glm::vec3(1.2f, 1.0f, 0.0f)); 
    textProgram->useProgram();
    textProgram->setUniforms(tex, translate, color, alpha);

    start_text();
    FT_Set_Pixel_Sizes(face, 0, 32);
    render_text("Celebren con nosotros!! ",
            -1 + 8 * sx,   1 - 480 * sy,   sx, sy);

}

void draw() {
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    glClearColor(alpha, alpha, alpha, 1.0f);
    // video(alpha);
    draw1(alpha);
    draw2(1 - alpha);
}


void start_text() {
    glEnableVertexAttribArray(textProgram->getCoordAttributeLocation());
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(textProgram->getCoordAttributeLocation(), 4, GL_FLOAT, GL_FALSE, 0, 0);
}

void render_text(const char *text, float x, float y, float sx, float sy) {
    const char *p;

    FT_GlyphSlot g = face->glyph;

    for(p = text; *p; p++) {
        if(FT_Load_Char(face, *p, FT_LOAD_RENDER))
            continue;

        glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_ALPHA,
                g->bitmap.width,
                g->bitmap.rows,
                0,
                GL_ALPHA,
                GL_UNSIGNED_BYTE,
                g->bitmap.buffer
                );

        float x2 = x + g->bitmap_left * sx;
        float y2 = -y - g->bitmap_top * sy;
        float w = g->bitmap.width * sx;
        float h = g->bitmap.rows * sy;

        GLfloat box[4][4] = {
            {x2,     -y2    , 0, 0},
            {x2 + w, -y2    , 1, 0},
            {x2,     -y2 - h, 0, 1},
            {x2 + w, -y2 - h, 1, 1},
        };

        glBufferData(GL_ARRAY_BUFFER, sizeof box, box, GL_DYNAMIC_DRAW);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        x += (g->advance.x >> 6) * sx;
        y += (g->advance.y >> 6) * sy;
    }
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

    if(direction == -1) {
        if(alpha > 0.0f) {
            alpha -= 0.001f;
            if(alpha <= 0.0f) {
                alpha = 0.0f;
                direction = 1;
            }
        } else {
            direction = 1;
        }
    } else {
        if(alpha < 1.0f) {
            alpha += 0.001f;
            if(alpha >= 1.0f) {
                alpha = 1.0f;
                direction = -1;
            }
        } else {
            direction = -1;
        }
    }

}


int main(int argc, char *argv[]) {
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    const GLFWvidmode *mode = glfwGetVideoMode( glfwGetPrimaryMonitor() );
    window = glfwCreateWindow(mode->width, mode->height, "UX Demo", glfwGetPrimaryMonitor(), NULL);
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

