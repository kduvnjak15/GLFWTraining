#include <iostream>
#include "GT_MenuScene.h"

GT_MenuScene::GT_MenuScene(GT_Camera* tempCam)
    :
      GT_Scene(tempCam, introScene),
      currButton_(0),
      skybox_(new GT_Skybox()),
      Ocean_(new GT_Ocean())
{
    buttons_.push_back("PLAY GAME");
    buttons_.push_back("QUIT GAME");

     skybox_ = new GT_Skybox();
     Ocean_  = new GT_Ocean();

    std::cout << " GT_MenuScene initialized " << std::endl;
}


void GT_MenuScene::sceneKeyboardHandler(int key, int scancode, int action, int mode)
{

}

void GT_MenuScene::renderScene()
{
    std::cout << "oko kere " << skybox_ <<  std::endl;
    skybox_->Draw(sceneCamera_);
    std::cout << "oko kere " << Ocean_  << "flush" << std::endl;
    Ocean_->draw(sceneCamera_);
}
