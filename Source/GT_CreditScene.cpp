#include "GT_CreditScene.h"


GT_CreditScene::GT_CreditScene()
    :
      GT_Scene(credits),
      skybox_(GT_Locator::getSkybox()),
      ocean_(GT_Locator::getOcean()),
      camTimer_(0),
      menuCamFront_(glm::vec3(0.0f, 0.0f, 1.0f))

{
    carrier_ = GT_Locator::getUSSCarrier();
    fighter_ = GT_Locator::getFighter();

    canvas_ = new GT_Image(0.0f, 0.0f, 0.0f, 0.5f);
    canvas_->defineImageScreenCoordinates(-0.7, -0.7, 0.7, 0.7);

    nextScene_ = credits;

    GT_Locator::getGameCamera()->setCameraPos(carrier_->getPosition()+ glm::vec3(170.0f, 58.0f, -32.0f));
    GT_Locator::getGameCamera()->setCameraFront(carrier_->getFront());
    GT_Locator::getGameCamera()->setCameraRight(carrier_->getRight());
    GT_Locator::getGameCamera()->setCameraUp(carrier_->getUp());

    oceanSound_.setBuffer(*(GT_Locator::getAudio()->getSoundBuffMap(OCEAN)));
    oceanSound_.setLoop(true);


    nextScene_ = credits;
}

void GT_CreditScene::checkKeyboardInput()
{
    if (keys_ != nullptr)
    {
        if (keys_[GLFW_KEY_Q])
            camTimer_-=3.0f;
        if (keys_[GLFW_KEY_E])
            camTimer_+=1.0f;
    }
}

void GT_CreditScene::sceneKeyboardHandler(bool *keys, int key, int scancode, int action, int mode)
{
    keys_ = keys;

    if (keys[GLFW_KEY_ESCAPE] && action == GLFW_PRESS)
    {
        nextScene_ = menuScene;
    }
}

void GT_CreditScene::printMSG()
{
    font_->PrintLine("Made by Kresimir Duvnjak: ", 55.0f, 150.0f, .50f, glm::vec3(1.0, 0.1f, 0.1f));

}


void GT_CreditScene::renderScene()
{
    ////////////////////////// timer //////////////////////////////////////
    updateClock();
    ///////////////////////////////////////////////////////////////////////

    checkKeyboardInput();

    //////////////////////    integration   ///////////////////////////////

    integrateScene(deltaTime_);

    ///////////////////////////////////////////////////////////////////////

    /////////////////////   reset color     //////////////////////////////
    glClearColor(0.0, 0.15f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    ///////////////////////////////////////////////////////////////////////

    ////////////////////////   rendering phase   /////////////////////////////


    skybox_->Draw(sceneCamera_);
    ocean_->draw(sceneCamera_);


    carrier_->Draw(sceneCamera_);
    fighter_->Draw(sceneCamera_);


    canvas_->Draw();

    printMSG();

    if (dirtySound_)
    {
        oceanSound_.play();
        dirtySound_ = false;
    }

}



void GT_CreditScene::integrateScene(GLfloat deltaTime_)
{
    camTimer_ += 1.0f;
    sceneCamera_->setCameraPos(GT_Locator::getFighter()->getPosition() +  glm::vec3(sin(camTimer_*SPINFACTOR_), 0.0f, cos(camTimer_*SPINFACTOR_))*10.0f);
    sceneCamera_->setCameraFront(glm::normalize(GT_Locator::getFighter()->getPosition() - sceneCamera_->getCameraPos()));
}
