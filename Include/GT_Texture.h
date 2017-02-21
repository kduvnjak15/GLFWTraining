#pragma once

#include <GL/glew.h>

class GT_Texture
{

public:
    GT_Texture(const char *textureImagePath);

    void Bind();

private:
    GLuint texture_;
};
