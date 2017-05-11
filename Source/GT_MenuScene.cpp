#include <iostream>
#include "GT_MenuScene.h"

GT_MenuScene::GT_MenuScene(GT_Camera* tempCam)
    :
      GT_Scene(tempCam, introScene),
      currButton_(0)
{
    buttons_.push_back("PLAY GAME");
    buttons_.push_back("QUIT GAME");

    std::cout << " GT_MenuScene initialized " << std::endl;
}


void GT_MenuScene::sceneKeyboardHandler(int key, int scancode, int action, int mode)
{

}

void GT_MenuScene::renderScene()
{

}
