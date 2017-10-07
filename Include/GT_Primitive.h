#pragma once

#include "GL/glew.h"
#include "GT_Shader.h"
#include "GT_Camera.h"

class GT_Primitive
{

public:
    GT_Primitive();
    GT_Primitive(const char* textureImage);

    virtual void initValues();
    virtual void defineTexture();
    virtual void defineVAO();
    virtual void defineShader();

    virtual void draw(GT_Camera* tempCam);

    GT_Shader* primitiveShader_;
    GLuint modelLoc_;
    GLuint viewLoc_;
    GLuint projLoc_;

    ~GT_Primitive();

protected:

    GLuint VAO_;
    GLuint VBO_;
    GLuint EBO_;
    GLuint TBO_;
    GLuint primitiveTexture_;


    GLfloat vertices_[20];
    GLuint indices_[6];
    GLfloat texCoords_[8];


    GLfloat sideA_;
    GLfloat sideB_;

    const char* texturePath_;
    bool hasTexture_;
    bool transparent_;



};

