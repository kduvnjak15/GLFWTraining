#pragma once

#include <GL/glew.h>
#include "GT_Shader.h"
#include "glm/glm.hpp"
#include "GT_Primitive.h"

class GT_Terrain : public GT_Primitive
{
public:

    GT_Terrain();

    void initValues();
    void defineTexture();
    void defineVAO();
    void defineShader();

    void draw();



public:
    GLuint VAO_;

    GT_Shader*  primitiveShader_;

    glm::vec3 position;

private:



    GLfloat vertices_[20];
    GLuint indices_[6];
    GLfloat texCoords_[8];

    GLuint terrainTexture_;
    GLuint VBO_;
    GLuint TBO_;
    GLuint EBO_;



};
