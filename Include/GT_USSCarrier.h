#pragma once

#include "GT_Enemy.h"

class GT_USSCarrier : public GT_Enemy
{
public:
    GT_USSCarrier(const char *modelPath);

    void Draw(GT_Camera *tempCam);

private:

};
