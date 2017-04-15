#pragma once

#include "GT_Enemy.h"

class GT_Raptor : public GT_Enemy
{
public:

    GT_Raptor(const char* modelPath);

    void Draw(GT_Camera* tempCam);

private:


};
