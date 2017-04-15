#pragma once

#include "GT_Primitive.h"

class GT_HUD /*: public GT_Primitive*/
{
public:
    GT_HUD();

    void initValues();
    void defineTexture();
    void defineVAO();
    void defineShader();

    void draw();

private:

};

