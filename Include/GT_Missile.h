#pragma once

#include "GT_Model.h"
#include "GT_Camera.h"
#include "GT_Actor.h"

#include <glm/glm.hpp>


class GT_Missile : public GT_Actor
{
public:

    GT_Missile(GT_Model* missileModel, void* ownerPtr);
    ~GT_Missile();

    void Draw(GT_Camera* tempCam);
    void Integrate(GLfloat DX_);

protected:

    void* ownerPtr_;

};
