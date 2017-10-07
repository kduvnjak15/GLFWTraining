#pragma once

#include "GL/glew.h"
#include "glm/glm.hpp"

#include "GT_Primitive.h"

class GT_Particle : public GT_Primitive
{
public:

    GT_Particle();

    void defineTexture();

    void draw();

    ~GT_Particle() = default;
};

