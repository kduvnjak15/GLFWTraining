#pragma once

#include "GT_Primitive.h"

class GT_Ocean : public GT_Primitive
{
public:
    GT_Ocean();
    GT_Ocean(const char *textureImage);

    void draw(GT_Camera *tempCam);

private:

    GLuint modelLoc_;
    GLuint viewLoc_;
    GLuint projLoc_;


};
