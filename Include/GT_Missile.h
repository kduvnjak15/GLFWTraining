#pragma once

#include "GT_Actor.h"

#include <glm/glm.hpp>

const GLfloat MISSILE_SPEED = 50.0f;

class GT_Missile : public GT_Actor
{
public:

    GT_Missile(GT_Model* missileModel, void *ownerPtr, GLuint missileIndex);
    ~GT_Missile();

    void Draw(GT_Camera* tempCam);
    void Integrate(GT_Camera* tempCam, GLfloat DX_);

    void FIRE();
    bool isFired() { return fired_; }

protected:

    void* ownerPtr_;
    GLuint missileIndex_;
    glm::vec3 missileOffset_;

private:

    bool fired_;

};
