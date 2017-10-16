#pragma once

#include "ft2build.h"
#include FT_FREETYPE_H

#include <map>

#include "GT_Shader.h"

#include <GL/glew.h>

#include "glm/glm.hpp"

#include "GT_Camera.h"

#include <sstream>

template <typename T>
std::string to_string(const T& value) {
    std::stringstream ss;
    ss << value;
    return ss.str();
}

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
    GT_Alphabet(GT_Camera* tempCam);

    GLuint fontVAO;

    void PrintLine(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);
    void PrintLine(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec4 color);

private:

    void RenderText(GT_Shader &shader, std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec4 color);

    void createTexQuads();

    GLuint VBO;

    std::map<GLchar, Character> Alphabet_;

    GT_Shader* fontShader_;
    GT_Camera* tempCam_;
    glm::mat4 projMatrix_;
};
