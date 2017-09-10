#pragma once

#include "GL/glew.h"
#include "GT_Shader.h"

class GT_Image
{

public:

    GT_Image(GLfloat r_ = 1.0f, GLfloat g_ = 1.0f, GLfloat b_ = 1.0f, GLfloat alpha = 1.0f );
    GT_Image(const char *texturePath);

    void Draw();

    void defineImageCoordinates(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2);
    void defineImageScreenCoordinates(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2);
    void setTransparency(GLfloat alpha);
    void makeTransparent(bool make) { transparent_ = make ;}


private:

    const char* texturePath_;
    GLuint textureHandle_;

    GT_Shader* imageShader_;

    bool hasTexture_ = false;
    bool transparent_ = false;

    GLfloat imageCoords_[8];
    GLuint imageIndices_[6];
    GLfloat textureCoords_[8];
    GLfloat RGBA_[4];

    GLuint VAO_;
    GLuint VBO_;
    GLuint TBO_;
    GLuint EBO_;
    GLuint CBO_;

    void initScreenCoords();
    void defineVAO();
    void defineShader();
    void defineTexture();


};
