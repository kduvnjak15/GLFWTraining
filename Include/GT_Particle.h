#pragma once

#include "GL/glew.h"
#include "glm/glm.hpp"

#include "GT_Primitive.h"

class GT_Particle : public GT_Primitive
{
public:

    GT_Particle();

    void initValues();
    void defineTexture();
    void defineVAO();
    void defineShader();

    void draw();

    ~GT_Particle();
};

