#pragma once

#include "GT_Primitive.h"
#include "GT_Radar.h"

#include <vector>

class GT_HUD : public GT_Primitive
{
public:
    friend GT_Radar;

    GT_HUD();

    void defineShader();

    void definePanels();

    void installRadar();


    void draw(GT_Camera *tempCam);

    typedef struct
    {
        glm::vec3 position_;
        glm::vec3 volume_;
    }panel;

private:

    std::vector<panel> panelSettings_;

    GT_Radar* radar_;
};


