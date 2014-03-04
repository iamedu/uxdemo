#include <cstdlib>
#include <cstdio>
#include <cmath>

#include <ux/gl.h>
#include <ux/util.h>
#include <ux/shaders.h>
#include <ux/shapes.h>
#include <ux/texture.h>
#include <ux/script.h>
#include <ux/download.h>
#include <ux/data.h>

#include <fstream>

#include <ft2build.h>
#include "utf8.h"
#include FT_FREETYPE_H

#define TIMEOUT 180
static vector<Twitt *> tweets;
static vector<Instagram *> instagrams;

static const GLFWvidmode *mode;
static ColorShaderProgram *colorProgram;
static TextureShaderProgram *textureProgram;
static TextShaderProgram *textProgram;
static ColorQuad *colorQuad;
static TextureQuad *textureQuad;
static int textureCache[100];
static int whiteTexture;
static int twitterTexture;
static int whiteBoxTexture;
static int blackTexture;
static int backgroundTexture = -1;
static int textureWidth;
static int textureHeight;
static int frame = 1;
static int direction = -1;
static float backgroundScale = 3.0f;
static float translation = -5.0f;
static float alpha = 1.0f;
static double lastTime;
static glm::mat4 projection;
//TEXT
static FT_Face face;
static GLuint tex;
static GLuint vbo;

void start_text();
float text_width(std::string s, float sx, float sy);
void render_text(std::string, float, float, float, float, int, int);
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

    if(FT_New_Face(ft, "fonts/FuturaStd-Light.ttf", 0, &face)) {
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

    whiteBoxTexture = loadTexture("textures/white-box.png", &textureWidth, &textureHeight);
    twitterTexture = loadTexture("textures/twitter.png", &textureWidth, &textureHeight);

    glEnable( GL_LINE_SMOOTH );
    glEnable( GL_POLYGON_SMOOTH );
    glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
    glHint( GL_POLYGON_SMOOTH_HINT, GL_NICEST );

    // Enable alpha blending.
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glfwSetTime(0);
}

std::string translateFile(std::string filename) {
    std::stringstream home;

    home << getenv("HOME") << "/.uxdemo/" << filename;

    return home.str();
}

int file_exists(std::string filename) {
    std::ifstream infile(translateFile(filename).data());
    return infile.good();
}

static Twitt *twittData = NULL;
static int currentTweet = 0;
static int userPicture;
static int tweetPicture;

void load_tweet() {
    if(tweets.size() == 0) {
        load_data(&tweets, &instagrams);
        return;
    }

    Twitt *t = tweets[currentTweet];

    int textureWidth;
    int textureHeight;

    if(t) {
        stringstream ss;
        ss << "twitter/" << t->twitter_id << "/slug";

        userPicture = loadTexture(translateFile(ss.str()), &textureWidth, &textureHeight);

        if(t->picture_url.size() > 0) {
            ss.str("");
            ss << "twitter/" << t->twitter_id << "/picture_url";
            tweetPicture = loadTexture(translateFile(ss.str()), &textureWidth, &textureHeight);
        }
        twittData = t;
    }

}

void draw() {
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    glClearColor(alpha, alpha, alpha, 1.0f);

    int textureWidth, textureHeight;
    if(file_exists("background") && backgroundTexture == -1) {
        backgroundTexture = loadTexture(translateFile("background"), &textureWidth, &textureHeight);
        if(backgroundTexture == 0) {
            backgroundTexture = -1;
        }
    }

    float w = (float)mode->width / (float)mode->height;

    glm::mat4 scaleBackground = glm::scale( glm::mat4 (1.0f), glm::vec3(2.0f * w, 2.0f, 1.0f));
    glm::mat4 scale = glm::scale( glm::mat4 (1.0f), glm::vec3(backgroundScale));
    glm::mat4 translate;
    glm::mat4 transformed = projection * scaleBackground;


    textureProgram->useProgram();
    textureProgram->setUniforms(transformed, backgroundTexture, alpha);
    textureQuad->bindData(textureProgram);
    textureQuad->draw();

    if(twittData == NULL) {
        load_tweet();
    } else {
        //User
        scale = glm::scale( glm::mat4 (1.0f), glm::vec3(0.3));
        translate = glm::translate( glm::mat4(1.0f), glm::vec3(w * 0.3, -2.5f, 0.0f));
        transformed = projection * scale * translate;
        textureProgram->useProgram();
        textureProgram->setUniforms(transformed, userPicture, alpha);
        textureQuad->bindData(textureProgram);
        textureQuad->draw();

        //Box
        textureProgram->useProgram();
        scale = glm::scale( glm::mat4 (1.0f), glm::vec3(1.5f, 0.5f, 0.3f));
        translate = glm::translate( glm::mat4(1.0f), glm::vec3(w * 0.3, -0.52f, 0.0f));
        transformed = projection * scale * translate;
        textureProgram->setUniforms(transformed, whiteBoxTexture, 0.2);
        textureQuad->bindData(textureProgram);
        textureQuad->draw();

        //Twitter
        textureProgram->useProgram();
        scale = glm::scale( glm::mat4 (1.0f), glm::vec3(0.20f, 0.16f, 0.16f));
        translate = glm::translate( glm::mat4(1.0f), glm::vec3(w * 0.8, 2.4f, 0.0f));
        transformed = projection * scale * translate;
        textureProgram->setUniforms(transformed, twitterTexture, 1.0);
        textureQuad->bindData(textureProgram);
        textureQuad->draw();


        float sx = 2.0 / 768;
        float sy = 2.0 / 768;
        glm::vec4 color = glm::vec4(1.0, 1.0, 1.0, 1.0);
        translate = glm::translate( glm::mat4(2.0f), glm::vec3(w * 0.64, 0.2f, 0.0f)); 
        textProgram->useProgram();
        textProgram->setUniforms(tex, translate, color, alpha);

        start_text();
        FT_Set_Pixel_Sizes(face, 0, 18);
        std::cout << text_width(twittData->status, sx, sy) << std::endl;
        render_text(twittData->status,
                -1 + 8 * sx,  1 - 520 * sy,   sx, sy, 40, 30);
    }

}


void start_text() {
    glEnableVertexAttribArray(textProgram->getCoordAttributeLocation());
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(textProgram->getCoordAttributeLocation(), 4, GL_FLOAT, GL_FALSE, 0, 0);
}

float text_width(std::string s, float sx, float sy) {
    const char *p = s.data();
    int len = s.size();
    float result = 0;

    FT_GlyphSlot g = face->glyph;

    for(int i = 0; i < len; i++) {
        int cp = utf8::next(p, p + (len - i));

        if(FT_Load_Char(face, cp, FT_LOAD_RENDER))
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

        float w = g->bitmap.width * sx;

        result += w;
    }

    return result;
}

void render_text(std::string s, float x, float y, float sx, float sy, int max_chars, int step) {
    int count_chars = 0;

    float startx = x;
    float starty = y;

    FT_GlyphSlot g = face->glyph;

    string token;
    while(token != s){
        token = s.substr(0,s.find_first_of(" "));
        s = s.substr(s.find_first_of(" ") + 1);
        const char *p = token.data();
        int len = utf8::distance(p, p + token.size());
        cout << token << " " << len << " " << count_chars << " " << max_chars << endl;

        count_chars += len + 1;

        if(max_chars > 0 && count_chars > max_chars) {
            count_chars = len;
            x = startx;
            y -= step * sy;
        }

        for(int i = 0; i < len; i++) {
            int cp = utf8::next(p, p + (len - i));

            if(FT_Load_Char(face, cp, FT_LOAD_RENDER))
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
        int cp = ' ';

        if(FT_Load_Char(face, cp, FT_LOAD_RENDER))
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
}


int main(int argc, char *argv[]) {
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    mode = glfwGetVideoMode( glfwGetPrimaryMonitor() );
    window = glfwCreateWindow(mode->width, mode->height, "UX Demo", glfwGetPrimaryMonitor(), NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    char temp[1024];
    double currentTime;

    init();
    resize(mode->width, mode->height);

    lastTime = glfwGetTime();
    init_download();

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        draw();
        animate();

        currentTime = glfwGetTime();

        if((currentTime - lastTime) > TIMEOUT) {
            download();
            lastTime = currentTime;
        }

        /* Swap front and back buffers */
        glfwSwapBuffers(window);
        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}

