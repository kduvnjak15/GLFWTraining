#pragma once

#include "GT_Aircraft.h"
#include "GT_Weapon.h"

class GT_Fighter : public GT_Aircraft ,  public GT_Weapon
{
public:

    GT_Fighter();

    void Draw(GT_Camera *tempCam);

    void Integrate(GT_Camera* tempCam, GLfloat DT_);


    bool isTarget() { return isTarget_; }

    void isTarget(bool flag) { isTarget_ = flag; }

private:

    bool isTarget_;

};
