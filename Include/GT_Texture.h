#pragma once

#include <GL/glew.h>

class GT_Texture
{

public:
    GT_Texture(const char *textureImagePath);

    void Bind(GLuint textureUnit);

private:
    GLuint texture_;
};
