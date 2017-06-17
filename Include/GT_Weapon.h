#pragma once

#include "GT_Missile.h"
#include "GT_Aircraft.h"

class GT_Weapon
{

public:

    GT_Weapon(GLuint numOfMissiles, GT_Aircraft* ownerPtr);

    void Draw(GT_Camera* tempCam);
    void Integrate(GLfloat DT_);

    GT_Aircraft* target_;

    void lock(GT_Aircraft* locked) { target_ = locked; }
    bool targetLocked() { return (bool)target_; }

    virtual void fireMissile();

protected:

    GLuint numOfMissiles_;

    std::vector<GT_Missile* > missiles_;

    glm::vec3 targetPos_;

    GLboolean intelligent_;
};
