#pragma once

#include <GL/glew.h>
#include "GT_Shader.h"
#include "glm/glm.hpp"

class GT_Terrain
{
public:

    GT_Terrain();

    void drawTerrain();



public:
    GLuint VAO_;

    GT_Shader*  terrainShader_;

    glm::vec3 position;

private:

    void initValues();
    void createTexture();
    void createVAO();
    void defineShader();

    GLfloat vertices_[20];
    GLuint indices_[6];
    GLfloat texCoords_[8];

    GLuint terrainTexture_;
    GLuint VBO_;
    GLuint TBO_;
    GLuint EBO_;



};
