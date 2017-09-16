#include <iostream>

#include "GT_Alphabet.h"


GT_Alphabet::GT_Alphabet(GT_Camera *tempCam)
{

    FT_Library ft;
    if (FT_Init_FreeType(&ft))
    {
        std::cout<< "ERROR::FREETYPE: Could not initi FreeType Library" <<std::endl;
    }

    FT_Face face;
    if (FT_New_Face(ft, "/usr/share/fonts/truetype/freefont/FreeSans.ttf", 0, &face))
    {
        std::cout << "ERRROR::FREETYPE Failed to load font " << std::endl;
    }

    FT_Set_Pixel_Sizes(face, 0, 48);

    if (FT_Load_Char(face, 'X', FT_LOAD_RENDER))
    {
        std::cout<< "ERROR::FREETYPE Failed to load Glyph"<<std::endl;
    }

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for (GLubyte c = 0; c < 128; c++)
    {
        if(FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            std::cout<< "ERROR::FREETYPE Failed to load Glyph"<<std::endl;
            continue;
        }

        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D( GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        Character letter =  {
                texture,
                glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                face->glyph->advance.x
            };

        Alphabet_.insert(std::pair<GLchar, Character>(c, letter));
    }

    createTexQuads();

    FT_Done_Face(face);
    FT_Done_FreeType(ft);



    fontShader_ = new GT_Shader(fontShader, "../Shaders/fontShader.vs", "../Shaders/fontShader.fs");
    this->tempCam_  = tempCam;

}

void GT_Alphabet::PrintLine(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color)
{
    RenderText(*fontShader_, text, x, y, scale, glm::vec4(color, 1.0f) );
}


void GT_Alphabet::PrintLine(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec4 color)
{
    RenderText(*fontShader_, text, x, y, scale, color );
}

void GT_Alphabet::RenderText(GT_Shader &shader, std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec4 color)
{
    shader.Use();
    glUniform4f(glGetUniformLocation(shader.shaderProgram_, "textColor"), color.x, color.y, color.z, color.w);
    glUniform1i(glGetUniformLocation(shader.shaderProgram_, "text"), 1);

    glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(window_width), 0.0f, static_cast<GLfloat>(window_height));
    glUniformMatrix4fv(glGetUniformLocation(fontShader_->shaderProgram_, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    glActiveTexture(GL_TEXTURE1);
    glBindVertexArray(fontVAO);

    glBlendFunc(GL_SRC0_ALPHA, GL_ONE);

    std::string::const_iterator c;
    for (c = text.begin(); c!=text.end(); c++)
    {
        Character ch = Alphabet_[*c];

        GLfloat xpos = x + ch.Bearing.x * scale;
        GLfloat ypos = y - (ch.Size.y - ch.Bearing.y)*scale;

        GLfloat w = ch.Size.x * scale;
        GLfloat h = ch.Size.y * scale;

        // Update VBO for each charachter
        GLfloat vertices[6][4] = {
            {xpos,     ypos + h, 0.0f, 0.0},
            {xpos,     ypos,     0.0f, 1.0},
            {xpos + w, ypos,     1.0f, 1.0},

            {xpos,     ypos + h, 0.0f, 0.0},
            {xpos + w, ypos,     1.0f, 1.0},
            {xpos + w, ypos + h, 1.0f, 0.0}
        };

        glBindTexture(GL_TEXTURE_2D, ch.TextureID);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // Now advance cursors for next glyph
        x += (ch.Advance >> 6) * scale;
    }

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);

    glBlendFunc(GL_SRC0_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

void GT_Alphabet::createTexQuads()
{
    glGenVertexArrays(1, &fontVAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(fontVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*6*4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4*sizeof(GLfloat), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

}
