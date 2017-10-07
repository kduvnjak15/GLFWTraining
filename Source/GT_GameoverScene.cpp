#include "GT_GameoverScene.h"
#include "GT_MenuScene.h"
#include "GT_GameplayScene.h"


GT_GameoverScene::GT_GameoverScene()
    :
      GT_Scene(credits),
      skybox_(GT_Locator::getSkybox()),
      ocean_(GT_Locator::getOcean()),
      camTimer_(0),
      menuCamFront_(glm::vec3(0.0f, 0.0f, 1.0f))
{

    carrier_ = GT_Locator::getUSSCarrier();
    fighter_ = GT_Locator::getFighter();

    canvas_ = new GT_Image(0.0f, 0.0f, 0.0f, 0.3f);
    canvas_->defineImageScreenCoordinates(-0.7, -0.7, 0.7, 0.7);

    GT_Locator::getGameCamera()->setCameraPos(carrier_->getPosition()+ glm::vec3(170.0f, 58.0f, -32.0f));
    GT_Locator::getGameCamera()->setCameraFront(carrier_->getFront());
    GT_Locator::getGameCamera()->setCameraRight(carrier_->getRight());
    GT_Locator::getGameCamera()->setCameraUp(carrier_->getUp());

    oceanSound_.setBuffer(*(GT_Locator::getAudio()->getSoundBuffMap(OCEAN)));
    oceanSound_.setLoop(true);

}

void GT_GameoverScene::checkKeyboardInput()
{
//    if (keys_ != nullptr)
//    {
//        if (keys_[GLFW_KEY_Q])
//            camTimer_-=3.0f;
//        if (keys_[GLFW_KEY_E])
//            camTimer_+=1.0f;
//    }


}

void GT_GameoverScene::sceneKeyboardHandler(bool *keys, int key, int scancode, int action, int mode)
{
    keys_ = keys;
    if (keys_[GLFW_KEY_ESCAPE] && action == GLFW_PRESS)
    {
        GT_Locator::getSceneManager()->activateScene( gameplay );
        GT_GameplayScene* gameplayScenePtr =dynamic_cast<GT_GameplayScene*>(GT_Locator::getSceneManager()->getActiveScene());
        if (!fighter_->isFreeMode() && sceneEnum_ != WIN)
        {
            gameplayScenePtr->resetGameplay();

            GT_Locator::getSceneManager()->activateScene( menuScene );
            GT_MenuScene* menuScenePtr =dynamic_cast<GT_MenuScene*>(GT_Locator::getSceneManager()->getActiveScene());
            menuScenePtr->resetMenuGameplay();
        }
    }

}

void GT_GameoverScene::printWIN()
{
    font_->PrintLine("MISSION ACCOMPLISHED!", 0.37, 0.6f, 1.00f, glm::vec3(1.0, 1.0f, 1.0f));

    font_->PrintLine("EPIC WIN!", 0.420f, 0.70f, 1.00f, glm::vec3(1.0, 1.0f, 1.0f));

    font_->PrintLine("You have passed all levels and unlocked free mode!", 0.3f, 0.35f, .50f, glm::vec3(1.0, 1.0f, 1.0f));

    font_->PrintLine("GOD SPEED, ACE!!!", 0.42f, 0.20f, .50f, glm::vec3(1.0, 1.0f, 1.0f));

}

void GT_GameoverScene::printLOSE()
{

    if (carrier_->explode_ > 0 )
    {
        font_->PrintLine("YOU WORTHLESS PIECE OF SHIT!!!",  0.35f, 0.80f, .70f, glm::vec3(1.0, 1.0f, 1.0f));

        font_->PrintLine("LET'S HOPE YOU DON'T LOOK LIKE YOU PLAY!",  0.38f, 0.20f, .50f, glm::vec3(1.0, 1.0f, 1.0f));
    }
    else if (fighter_->explode_ > 0)
    {
        font_->PrintLine("YOU SUCK!",  0.44f, 0.80f, 1.0f, glm::vec3(1.0, 1.0f, 1.0f));

        font_->PrintLine("BUY YOURSELF A BICYCLE FIRST!", 0.40f, 0.20f, .50f, glm::vec3(1.0, 1.0f, 1.0f));
    }
}

void GT_GameoverScene::printDROWN()
{
    font_->PrintLine("YOU FLY LOW LIKE YOUR MOMMA's IQ! ", 0.38f, 0.80f, .50f, glm::vec3(1.0, 1.0f, 1.0f));

    font_->PrintLine("Don't hold your breath!", 0.44f, 0.20f, .50f, glm::vec3(1.0, 1.0f, 1.0f));
}

void GT_GameoverScene::renderScene()
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

    if (sceneEnum_ == WIN)
        printWIN();
    else if (sceneEnum_ == LOSE)
        printLOSE();
    else if (sceneEnum_ == DROWN)
        printDROWN();

    if (dirtySound_)
    {
        oceanSound_.play();
        dirtySound_ = false;
    }

}


void GT_GameoverScene::integrateScene(GLfloat deltaTime_)
{
    camTimer_ += 1.0f;
    if (GT_Locator::getUSSCarrier()->explode_)
    {
        std::cout << GT_Locator::getUSSCarrier()->getPosition().y << std::endl;
        if (GT_Locator::getUSSCarrier()->getPosition().y < 14)
        {
            GT_Locator::getSceneManager()->activateScene( menuScene );
//            GT_Scene* menusceneHolder = GT_Locator::getSceneManager()->getActiveScene();
//            GT_MenuScene* menuscenePtr = dynamic_cast<GT_MenuScene*>(menusceneHolder);
//            menuscenePtr->initializeMenu();
        }

        GT_Locator::getUSSCarrier()->Integrate(sceneCamera_, deltaTime_);

        sceneCamera_->setCameraPos(GT_Locator::getUSSCarrier()->getPosition() +  glm::vec3(sin(camTimer_*SPINFACTOR_), 0.0f, cos(camTimer_*SPINFACTOR_))*200.0f);
        sceneCamera_->setCameraFront(glm::normalize(GT_Locator::getUSSCarrier()->getPosition() - (sceneCamera_->getCameraPos()+ glm::vec3(0.0f, 40.0f, 0.0f))));
    }
    else
    {
        sceneCamera_->setCameraPos(GT_Locator::getFighter()->getPosition() +  glm::vec3(sin(camTimer_*SPINFACTOR_), 0.0f, cos(camTimer_*SPINFACTOR_))*10.0f);
        sceneCamera_->setCameraFront(glm::normalize(GT_Locator::getFighter()->getPosition() - sceneCamera_->getCameraPos()));
    }

    if (sceneCamera_->getCameraPos().y < 15)
        sceneEnum_ = DROWN;
    else if (fighter_->explode_ > 0 || carrier_->explode_ > 0)
        sceneEnum_ = LOSE;
    else if (fighter_->explode_ <= 0)
    {
        sceneEnum_ = WIN;
        fighter_->unlockFreeMode();
    }



}
