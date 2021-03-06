#include "GT_Primitive.h"

#include "SOIL/src/SOIL.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"


#include <iostream>

GT_Primitive::GT_Primitive()
    : texturePath_(""),
       sideA_(1.0f),
       sideB_(1.0f),
       hasTexture_(false),
       transparent_(false)
{
    std::cout<<"Textureless primitive"<<this<<std::endl;
}

GT_Primitive::GT_Primitive(const char* textureImage)
    : texturePath_(textureImage),
      sideA_(1.0f),
      sideB_(1.0f),
      hasTexture_(true)

{

    initValues();
    defineTexture();
    defineVAO();
    defineShader();

    std::cout<< "GT_Primitive ctor "<<this<<" "<<textureImage<<std::endl;

}

void GT_Primitive::initValues()
{
    GLfloat h = 0.0f;
    // left up
    vertices_[0] = -sideA_;
    vertices_[1] =  h;
    vertices_[2] = -sideB_;

    vertices_[3] =  0.0f;
    vertices_[4] =  1.0f;

    // left down
    vertices_[5] = -sideA_;
    vertices_[6] =  h;
    vertices_[7] =  sideB_;

    vertices_[8] =  0.0f;
    vertices_[9] =  0.0f;

    // right near
    vertices_[10] =  sideA_;
    vertices_[11] =  h;
    vertices_[12] =  sideB_;

    vertices_[13] =  1.0f;
    vertices_[14] =  0.0f;

    // top right
    vertices_[15]  =  sideA_;
    vertices_[16] =  h;
    vertices_[17] = -sideB_;

    vertices_[18] =  1.0f;
    vertices_[19] =  1.0f;


    /****************************/

    indices_[0] = 0;
    indices_[1] = 1;
    indices_[2] = 2;

    indices_[3] = 2;
    indices_[4] = 3;
    indices_[5] = 0;

    /****************************/

    texCoords_[0] = 1.0f;
    texCoords_[1] = 1.0f;

    texCoords_[2] = 1.0f;
    texCoords_[3] = 0.0f;

    texCoords_[4] = 0.0f;
    texCoords_[5] = 0.0f;

    texCoords_[6] = 0.0f;
    texCoords_[7] = 1.0f;
}

void GT_Primitive::defineTexture()
{
    int width, height;
    GLenum alphaChannel, soilAlpha;
    if (transparent_)
    {
        alphaChannel = GL_RGBA;
        soilAlpha    = SOIL_LOAD_RGBA;
    }
    else
    {
        alphaChannel = GL_RGB;
        soilAlpha    = SOIL_LOAD_RGB;
    }
    unsigned char* image = SOIL_load_image(texturePath_, &width, &height, 0, soilAlpha);


    glGenTextures(1, &primitiveTexture_);
    glBindTexture(GL_TEXTURE_2D, primitiveTexture_);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    //Set texture filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, alphaChannel, width, height, 0, alphaChannel, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);

    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void GT_Primitive::defineVAO()
{
    glGenVertexArrays(1, &VAO_);
    glGenBuffers(1, &VBO_);
    glGenBuffers(1, &TBO_);
    glGenBuffers(1, &EBO_);

    glBindVertexArray(VAO_);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_), vertices_, GL_STATIC_DRAW);
    // Position attribute

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3* sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_), indices_, GL_STATIC_DRAW);

    glBindVertexArray(0);
}


void GT_Primitive::defineShader()
{
    primitiveShader_ = new GT_Shader(particleShader, "../Shaders/primitiveShader.vs", "../Shaders/primitiveShader.fs");
    modelLoc_    = glGetUniformLocation(primitiveShader_->shaderProgram_, "model");
    viewLoc_     = glGetUniformLocation(primitiveShader_->shaderProgram_, "view");
    projLoc_     = glGetUniformLocation(primitiveShader_->shaderProgram_, "projection");
}


void GT_Primitive::draw(GT_Camera *tempCam)
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, primitiveTexture_);

    this->primitiveShader_->Use();
    glm::mat4 model      = glm::mat4(1.0f);
    glm::mat4 view       = tempCam->GetViewMatrix();
    glm::mat4 projection = tempCam->GetProjectionMatrix();

    glUniformMatrix4fv(modelLoc_, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc_, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(projLoc_, 1, GL_FALSE, glm::value_ptr(model));

    glBindVertexArray(VAO_);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (const GLvoid*)0);

    glBindVertexArray(0);
}

GT_Primitive::~GT_Primitive()
{
    delete primitiveShader_;
    std::cout << "GT_Primitive dtor" << std::endl;

}

