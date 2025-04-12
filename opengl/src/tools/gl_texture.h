// Class to wrap OpenGL texture creation and use.
// Adapted from code in DeVries' coordinate systems
// example from www.learnopengl.com.

#ifndef GL_TEXTURE_H
#define GL_TEXTURE_H

#include "glad/glad.h"

#include <stb_image.h>

#include <learnopengl/filesystem.h>

#include <string>

class GLTexture {
public:
  explicit  GLTexture(const std::string& filename, GLenum format) {
    glGenTextures(1, &mGlHandle);
    glBindTexture(GL_TEXTURE_2D, mGlHandle);

    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    const std::string filepath = FileSystem::getPath(filename);
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    unsigned char *data = stbi_load(filepath.c_str(),
                                    &width, &height, &nrChannels, 0);

    if (data) {
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, format, GL_UNSIGNED_BYTE, data);
      glGenerateMipmap(GL_TEXTURE_2D);
      mIsLoaded = true;
    }

    stbi_image_free(data);
  }

  [[nodiscard]] bool isLoaded() const {
    return mIsLoaded;
  }

  void bind(GLenum textureUnit) const {
    glActiveTexture(textureUnit);
    glBindTexture(GL_TEXTURE_2D, mGlHandle);
  }

private:
  unsigned int mGlHandle = 0;

  bool mIsLoaded = false;
};

#endif //GL_TEXTURE_H
