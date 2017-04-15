#pragma once

#include "GT_Primitive.h"

class GT_Ocean : public GT_Primitive
{
public:
    GT_Ocean();

    void initValues();
    void defineTexture();
    void defineVAO();
    void defineShader();

    void draw();
};
