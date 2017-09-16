#pragma once

#include "GT_Aircraft.h"
#include "GT_Weapon.h"

class GT_Fighter : public GT_Aircraft ,  public GT_Weapon
{
public:

    GT_Fighter();

    void Draw(GT_Camera *tempCam);

    void Integrate(GT_Camera* tempCam, GLfloat DT_);

    bool isTarget() { return isTarget_ > 0; }
    void isTarget(bool flag);

    bool evade() { return evade_ > 0; }
    void evade(bool flag);

    void unlockFreeMode() { freeMode_ = true; }
    bool isFreeMode() { return freeMode_; }

private:

    GLint isTarget_;
    GLint evade_;

    bool freeMode_ = false;

};
