#include <ux/texture.h>
#include <SOIL/SOIL.h>
#include <iostream>

GLuint loadTexture(GLenum texture, std::string path) {
    glActiveTexture(GL_TEXTURE0);

    GLuint texture_id = SOIL_load_OGL_texture
        (
         path.data(),
         SOIL_LOAD_AUTO,
         SOIL_CREATE_NEW_ID,
         SOIL_FLAG_INVERT_Y
        );

    if(texture_id == 0) {
        std::cerr << "SOIL loading error: '" << SOIL_last_result() << "' (" << path << ")" << std::endl;
    }

    return texture_id;
}

GLuint loadTexture(GLenum texture, std::string path, int *width, int *height) {
    GLuint texture_id;
    glActiveTexture(GL_TEXTURE0);

    unsigned char* img = SOIL_load_image(path.data(), width, height, NULL, 0);
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, *width, *height, 0, GL_RGB, GL_UNSIGNED_BYTE, img);

    return texture_id; 
}

