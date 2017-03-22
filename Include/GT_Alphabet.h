#pragma once

#include "ft2build.h"
#include FT_FREETYPE_H

#include <map>

#include "GT_Shader.h"

#include <GL/glew.h>

#include "glm/glm.hpp"



struct Character
{
    GLuint TextureID;
    glm::ivec2 Size;
    glm::ivec2 Bearing;
    GLuint Advance;

};


class GT_Alphabet
{
public:
    GT_Alphabet();

    GLuint fontVAO;

    void RenderText(GT_Shader &shader, std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);

private:

    void createTexQuads();

    GLuint VBO;

    std::map<GLchar, Character> Alphabet_;
};
