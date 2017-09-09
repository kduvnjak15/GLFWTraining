#pragma once

#define DISP(e) std::cout<< e << std::endl;

#include "GT_Image.h"

namespace GT_Utils
{

    void drawImage(GT_Image &imgRef);
    void getWindowWidthAndHeight(GLuint& win_width, GLuint& win_height);

}


