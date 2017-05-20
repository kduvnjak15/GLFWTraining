#pragma once

#include "GT_Actor.h"

#include <glm/glm.hpp>


class GT_Missile : public GT_Actor
{
public:

    GT_Missile(GT_Model* missileModel, void *ownerPtr, GLuint missileIndex);
    ~GT_Missile();

    void Draw(GT_Camera* tempCam);
    void Integrate(GLfloat DX_);

protected:

    void* ownerPtr_;
    GLuint missileIndex_;
};
