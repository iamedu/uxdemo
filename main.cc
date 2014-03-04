#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <cstring>

#include <ux/gl.h>
#include <ux/util.h>
#include <ux/shaders.h>
#include <ux/shapes.h>
#include <ux/texture.h>
#include <ux/script.h>
#include <ux/download.h>
#include <ux/data.h>

#include <algorithm>

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
static int instagramTexture;
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
    instagramTexture = loadTexture("textures/instagram.png", &textureWidth, &textureHeight);

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
    std::string s = translateFile(filename).data();
    //cout << "Preguntando por " << s << endl;
    std::ifstream infile(s.data());
    return infile.good();
}

static Twitt *twittData = NULL;
static int currentTweet = 0;
static int userPicture;
static int tweetPicture;

static Instagram *instagramData = NULL;
static int currentInstagram = 0;
static int instagramPicture;


void load_tweet() {
    if(tweets.size() == 0) {
        load_data(&tweets, &instagrams);
        return;
    }
    
    if(currentTweet >= tweets.size()) {
    }

    Twitt *t = tweets[currentTweet++];

    int textureWidth;
    int textureHeight;

    if(t) {
        std::stringstream ss;
        ss << "twitter/" << t->twitter_id << "/slug";

        userPicture = loadTexture(translateFile(ss.str()), &textureWidth, &textureHeight);

        if(t->picture_url.size() > 0) {
            ss.str("");
            ss << "twitter/" << t->twitter_id << "/picture_url";
            tweetPicture = loadTexture(translateFile(ss.str()), &textureWidth, &textureHeight);
        }
        twittData = t;
        instagramData = NULL;
    }


}

void load_instagram() {
    if(instagrams.size() == 0) {
        load_data(&tweets, &instagrams);
        return;
    }

    if(currentInstagram >= instagrams.size()) {
        currentInstagram = 0;
    }

    Instagram *t = instagrams[currentInstagram++];

    while(t && t->video && currentInstagram <= instagrams.size()) {
        t = instagrams[++currentInstagram];
    }


    if(currentInstagram > currentInstagram) {
        t = NULL;
    }

    int textureWidth;
    int textureHeight;

    if(t) {
        std::stringstream ss;
        string part = t->link.substr(24, 9);
        ss << "instagram/" << part << "/profile_url";

        userPicture = loadTexture(translateFile(ss.str()), &textureWidth, &textureHeight);

        ss.str("");
        ss << "instagram/" << part << "/standard_resolution";
        instagramPicture = loadTexture(translateFile(ss.str()), &textureWidth, &textureHeight);

        instagramData = t;
        twittData = NULL;
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
    textureProgram->setUniforms(transformed, backgroundTexture, 1.0f);
    textureQuad->bindData(textureProgram);
    textureQuad->draw();

    // load_instag\ram();
    if(instagramData != NULL) {
        float down = 0.0f;
        down = 0.15f;
        scale = glm::scale( glm::mat4 (1.0f), glm::vec3(0.5, 0.125, 0.0));
        translate = glm::translate( glm::mat4(1.0f), glm::vec3(w * 0.4, 3.0f, 0.0f));
        transformed = projection * scale * translate;
        textureProgram->useProgram();
        textureProgram->setUniforms(transformed, instagramTexture, alpha);
        textureQuad->bindData(textureProgram);
        textureQuad->draw();

        scale = glm::scale( glm::mat4 (1.0f), glm::vec3(0.8));
        translate = glm::translate( glm::mat4(1.0f), glm::vec3(w * 0.80, 0.25f, 0.0f));
        transformed = projection * scale * translate;
        textureProgram->useProgram();
        textureProgram->setUniforms(transformed, instagramPicture, alpha);
        textureQuad->bindData(textureProgram);
        textureQuad->draw();


        //User
        scale = glm::scale( glm::mat4 (1.0f), glm::vec3(0.3));
        translate = glm::translate( glm::mat4(1.0f), glm::vec3(w * 0.3, -2.6f, 0.0f));
        transformed = projection * scale * translate;
        textureProgram->useProgram();
        textureProgram->setUniforms(transformed, userPicture, alpha);
        textureQuad->bindData(textureProgram);
        textureQuad->draw();

        //Box
        textureProgram->useProgram();
        scale = glm::scale( glm::mat4 (1.0f), glm::vec3(1.5f, 0.5f, 0.3f));
        translate = glm::translate( glm::mat4(1.0f), glm::vec3(w * 0.3, -0.52f - down * 2.0, 0.0f));
        transformed = projection * scale * translate;
        float transparentAlpha = 0.2 * alpha;
        //if(alpha < transparentAlpha) transparentAlpha = alpha;
        textureProgram->setUniforms(transformed, whiteBoxTexture, transparentAlpha);
        textureQuad->bindData(textureProgram);
        textureQuad->draw();

        float sx = 2.0 / 768;
        float sy = 2.0 / 768;
        glm::vec4 color = glm::vec4(1.0, 1.0, 1.0, 1.0);
        translate = glm::translate( glm::mat4(2.0f), glm::vec3(w * 0.64, 0.2f - down, 0.0f)); 
        textProgram->useProgram();
        textProgram->setUniforms(tex, translate, color, alpha);

        start_text();
        FT_Set_Pixel_Sizes(face, 0, 18);
        render_text(instagramData->tags,
                -1 + 8 * sx,  1 - 520 * sy,   sx, sy, 40, 30);

        color = glm::vec4(1.0, 1.0, 1.0, 1.0);
        translate = glm::translate( glm::mat4(2.0f), glm::vec3(w * 0.75, -0.4f, 0.0f)); 
        textProgram->useProgram();
        textProgram->setUniforms(tex, translate, color, alpha);

        std::stringstream ss;
        ss << "@" << instagramData->name;

        FT_Set_Pixel_Sizes(face, 0, 20);
        render_text(ss.str(),
                -1 + 8 * sx,  1 - 520 * sy,   sx, sy, 0, 0);

    }
    
    if(twittData != NULL) {
        float down = 0.0f;
        if(twittData->picture_url.size() > 0) {
            down = 0.15f;
            scale = glm::scale( glm::mat4 (1.0f), glm::vec3(0.8));
            translate = glm::translate( glm::mat4(1.0f), glm::vec3(w * 0.32, 0.25f, 0.0f));
            transformed = projection * scale * translate;
            textureProgram->useProgram();
            textureProgram->setUniforms(transformed, tweetPicture, alpha);
            textureQuad->bindData(textureProgram);
            textureQuad->draw();
        }


        //User
        scale = glm::scale( glm::mat4 (1.0f), glm::vec3(0.3));
        translate = glm::translate( glm::mat4(1.0f), glm::vec3(w * 0.3, -2.6f, 0.0f));
        transformed = projection * scale * translate;
        textureProgram->useProgram();
        textureProgram->setUniforms(transformed, userPicture, alpha);
        textureQuad->bindData(textureProgram);
        textureQuad->draw();

        //Box
        textureProgram->useProgram();
        scale = glm::scale( glm::mat4 (1.0f), glm::vec3(1.5f, 0.5f, 0.3f));
        translate = glm::translate( glm::mat4(1.0f), glm::vec3(w * 0.3, -0.52f - down * 2.0, 0.0f));
        transformed = projection * scale * translate;
        float transparentAlpha = 0.2f * alpha;
        //if(alpha < transparentAlpha) transparentAlpha = alpha;
        textureProgram->setUniforms(transformed, whiteBoxTexture, transparentAlpha);
        textureQuad->bindData(textureProgram);
        textureQuad->draw();

        //Twitter
        textureProgram->useProgram();
        if(down > 0) {
            scale = glm::scale( glm::mat4 (1.0f), glm::vec3(0.10f, 0.08f, 0.08f));
            translate = glm::translate( glm::mat4(1.0f), glm::vec3(w * 5.6, 2.4f, 0.0f));
        } else {
            scale = glm::scale( glm::mat4 (1.0f), glm::vec3(0.20f, 0.16f, 0.16f));
            translate = glm::translate( glm::mat4(1.0f), glm::vec3(w * 0.8, 2.4f, 0.0f));
        }
        transformed = projection * scale * translate;
        textureProgram->setUniforms(transformed, twitterTexture, alpha);
        textureQuad->bindData(textureProgram);
        textureQuad->draw();


        float sx = 2.0 / 768;
        float sy = 2.0 / 768;
        glm::vec4 color = glm::vec4(1.0, 1.0, 1.0, 1.0);
        translate = glm::translate( glm::mat4(2.0f), glm::vec3(w * 0.64, 0.2f - down, 0.0f)); 
        textProgram->useProgram();
        textProgram->setUniforms(tex, translate, color, alpha);

        start_text();
        FT_Set_Pixel_Sizes(face, 0, 18);
        render_text(twittData->status,
                -1 + 8 * sx,  1 - 520 * sy,   sx, sy, 40, 30);

        color = glm::vec4(1.0, 1.0, 1.0, 1.0);
        translate = glm::translate( glm::mat4(2.0f), glm::vec3(w * 0.75, -0.4f, 0.0f)); 
        textProgram->useProgram();
        textProgram->setUniforms(tex, translate, color, alpha);
        std::stringstream ss;
        ss << "@" << twittData->slug;

        FT_Set_Pixel_Sizes(face, 0, 20);
        render_text(ss.str(),
                -1 + 8 * sx,  1 - 520 * sy,   sx, sy, 0, 0);

        color = glm::vec4(1.0, 1.0, 1.0, 1.0);
        if(down > 0) {
            scale = glm::scale( glm::mat4 (1.0f), glm::vec3(0.8f));
            translate = glm::translate( glm::mat4(2.0f), glm::vec3(w * 1.1, 0.45f, 0.0f)); 
            transformed = projection * translate * scale;
        } else {
            translate = glm::translate( glm::mat4(2.0f), glm::vec3(w * 0.76, 0.74f, 0.0f)); 
            transformed = translate;
        }
        textProgram->useProgram();
        textProgram->setUniforms(tex, transformed, color, alpha);

        FT_Set_Pixel_Sizes(face, 0, 32);
        render_text("@LunarioMx",
                -1 + 8 * sx,  1 - 520 * sy,   sx, sy, 0, 0);
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
        int cp;
        try {
            cp = utf8::next(p, p + (len - i));
        } catch(const utf8::exception& utfcpp_ex) { 
            std::cerr << utfcpp_ex.what() << endl;
            cp = ' ';
        }

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

    glm::vec4 orange = glm::vec4(0.93, 0.25, 0.21, 1.0);
    glm::vec4 white = glm::vec4(1.0, 1.0, 1.0, 1.0);

    FT_GlyphSlot g = face->glyph;

    std::replace( s.begin(), s.end(), '\n', ' ');

    string token;
    while(token != s){
        int found = s.find_first_of(" ");
        token = s.substr(0, s.find_first_of(" "));
        s = s.substr(s.find_first_of(" ") + 1);
        const char *p = token.data();
        int len = utf8::distance(p, p + token.size());

        if(strcasecmp(token.data(), "#lunario10") == 0) {
            token = "#LUNARIO10";
            textProgram->updateColor(orange);
        } else {
            textProgram->updateColor(white);
        }

        count_chars += len + 1;

        if(max_chars > 0 && count_chars > max_chars) {
            count_chars = len;
            x = startx;
            y -= step * sy;
        }

        for(int i = 0; i < len; i++) {
            int cp;
            try {
                cp = utf8::next(p, p + (len - i));
            } catch(const utf8::exception& utfcpp_ex) { 
                std::cerr << utfcpp_ex.what() << endl;
                cp = ' ';
            }

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

        if(found == -1) {
            break;
        }
    }

}

void resize(int w, int h) {
    glViewport(0, 0, w, h);

    float aspectRatio = (float)w / (float)h; 

    projection = glm::ortho( -aspectRatio, aspectRatio, -1.0f, 1.0f, -1.0f, 1.0f );
}

static int stage;
static float counter;
static float startTime;

void animate() {
    // cout << counte\r << endl;
    counter = counter + 0.01f;
    static double nowTime = counter;
    if(stage == 0) {
        if(counter >= 3) {
            stage = 1;
            startTime = counter;
        }
    } else if(stage == 1) {
        alpha -= 0.01f;
        if(alpha <= 0) {
            alpha = 0.0f;
            stage = 2;
            int r = random() % 10;
            if(r < 6) {
                load_tweet();
            } else {
                load_instagram();
            }
        }
    } else if(stage == 2) {
        alpha += 0.01f;
        if(alpha >= 1.0f) {
            alpha = 1.0f;
            stage = 0;
            counter = 0.0f;
            startTime = counter;
        }
    }
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

    stage =0;
    counter = 0.0f;
    startTime = 0.0f;

    //load_tweet();
    int r = random() % 10;
    if(r < 6) {
        load_tweet();
    } else {
        load_instagram();
    }

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

