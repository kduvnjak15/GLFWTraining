#pragma once

#include "GT_Enemy.h"


class GT_Fighter : public GT_Aircraft
{
public:

    GT_Fighter();

    void Draw(GT_Camera *tempCam);

    void lock(GT_Enemy* locked) { target_ = locked; }
    bool isLocked() { return (bool)target_; }

private:

    GT_Enemy* target_;
};
