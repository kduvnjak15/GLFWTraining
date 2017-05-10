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


void GT_MenuScene::KeyboardHandler(keyPressed keyInput)
{
    if (keyInput == keyPressed::LEFT)
    {
        int temp = currButton_;
        temp--;
        currButton_ = temp % buttons_.size();
        std::cout << currButton_ << std::endl;
    }
    else if (keyInput == keyPressed::RIGHT)
    {
        int temp = currButton_;
        temp++;
        currButton_ = temp % buttons_.size();
        std::cout << currButton_ << std::endl;
    }
    else if (keyInput == keyPressed::UP)
    {

    }
    else if (keyInput == keyPressed::DOWN)
    {

    }
    else if (keyInput == keyPressed::ENTER)
    {
        if (buttons_[currButton_] == "QUIT GAME")
        {

        }
        else if (buttons_[currButton_] == "PLAY GAME")
        {

        }
    }
    else if (keyInput == keyPressed::ESC)
    {

    }
}

