#pragma once 

#include "GT_Primitive.h"

class GT_Radar : public GT_Primitive
{
public:

    GT_Radar();

    void draw(GT_Camera* tempCam);

    void setScreenPos(glm::vec3 radarPos) { screenPos_ = radarPos ;}

private:

    glm::vec3 screenPos_;
};
