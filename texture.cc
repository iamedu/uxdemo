#include <ux/texture.h>
#include <SOIL/SOIL.h>
#include <iostream>


GLuint loadTexture(std::string path) {
    glActiveTexture(GL_TEXTURE0);

    GLuint texture_id = SOIL_load_OGL_texture
        (
         path.data(),
         SOIL_LOAD_AUTO,
         SOIL_CREATE_NEW_ID,
         0//SOIL_FLAG_INVERT_Y
        );

    if(texture_id == 0) {
        std::cerr << "SOIL loading error: '" << SOIL_last_result() << "' (" << path << ")" << std::endl;
    }

    // glBindTexture(GL_TEXTURE_2D, texture_id);
    // glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    // glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    // glBindTexture(GL_TEXTURE_2D, 0);

    return texture_id;
}

GLuint loadTexture(std::string path, int *width, int *height) {
    unsigned char* img = SOIL_load_image(path.data(), width, height, NULL, 0);
    SOIL_free_image_data(img);

    GLuint texture_id = SOIL_load_OGL_texture
        (
         path.data(),
         SOIL_LOAD_AUTO,
         SOIL_CREATE_NEW_ID,
         0//SOIL_FLAG_INVERT_Y
        );

    if(texture_id == 0) {
        std::cerr << "SOIL loading error: '" << SOIL_last_result() << "' (" << path << ")" << std::endl;
    }

    // glBindTexture(GL_TEXTURE_2D, texture_id);
    // glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    // glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    // glBindTexture(GL_TEXTURE_2D, 0);

    return texture_id; 
}

