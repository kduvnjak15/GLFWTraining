#pragma once

#include "GL/glew.h"
#include "GT_Shader.h"

class GT_Primitive
{

public:
    GT_Primitive();

    virtual void initValues() = 0;
    virtual void defineTexture() = 0 ;
    virtual void defineVAO() = 0;
    virtual void defineShader() = 0;

    virtual void draw() = 0 ;

    GT_Shader* primitiveShader_;

protected:
    GLuint VAO_;

    GLuint VBO_;
    GLuint EBO_;
    GLuint TBO_;
    GLuint primitiveTexture_;

    GLfloat vertices_[20];
    GLuint indices_[6];
    GLfloat texCoords_[8];





};
