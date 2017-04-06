#pragma once

#include "GL/glew.h"
#include "glm/glm.hpp"

#include "GT_Shader.h"

class GT_Particle
{
public:

    GT_Particle();


    void drawParticle();

    ~GT_Particle();

public:
    GLuint VAO_;

    GT_Shader* particleShader_;

    glm::vec3 position;


private:

    void initValues();
    void createParticle();
    void createVAO();
    void defineShader();

    GLuint particleTexture_;
    GLuint particleVAO_;
    GLuint particleVBO_;
    GLuint particleEBO_;
    GLuint particleTBO_;

    GLfloat vertices_[20];
    GLuint indices_[6];
    GLfloat texCoords_[8];

    GLuint terrainTexture_;
    GLuint VBO_;
    GLuint TBO_;
    GLuint EBO_;


    GLfloat birthday_;
    GLfloat life_;
};
