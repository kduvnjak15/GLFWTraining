#include "GT_Utils.h"

#include "GT_Primitive.h"
#include "GT_Locator.h"




void GT_Utils::drawImage(GT_Image& imgRef)
{
    imgRef.Draw();

  //  DISP("introscene")
}

void GT_Utils::getWindowWidthAndHeight(GLuint &win_width, GLuint &win_height)
{
    win_width  = GT_Locator::getGameCamera()->getWindowWidth();
    win_height = GT_Locator::getGameCamera()->getWindowHeight();
}
