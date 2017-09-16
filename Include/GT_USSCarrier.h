#pragma once

#include "GT_Aircraft.h"


class GT_USSCarrier : public GT_Aircraft
{
public:
    GT_USSCarrier();

    void Draw(GT_Camera *tempCam);

    virtual void Integrate(GT_Camera* tempCam,  GLfloat DT_);

private:


};
