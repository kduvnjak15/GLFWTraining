#pragma once

#include "GL/glew.h"
#include "GT_Shader.h"

class GT_Image
{

public:

    GT_Image(const char *texturePath);

    void Draw();

private:

    const char* texturePath_;
    GLuint textureHandle_;
    GT_Shader* imageShader_;

    bool transparent_ = false;

    GLfloat imageCoords_[8];
    GLuint imageIndices_[6];
    GLfloat textureCoords_[8];

    GLuint VAO_;
    GLuint VBO_;
    GLuint TBO_;
    GLuint EBO_;

    void initScreenCoords();
    void defineVAO();
    void defineShader();
    void defineTexture();

  unsigned int VBO, VAO, EBO;
    float vertices[12] = {
             0.5f,  0.5f, 0.0f,  // top right
             0.5f, -0.5f, 0.0f,  // bottom right
            -0.5f, -0.5f, 0.0f,  // bottom left
            -0.5f,  0.5f, 0.0f   // top left
        };
        unsigned int indices[6] = {  // note that we start from 0!
            0, 1, 3,  // first Triangle
            1, 2, 3   // second Triangle
        };
};
