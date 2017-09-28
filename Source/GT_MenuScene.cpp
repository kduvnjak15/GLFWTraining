#include <iostream>

#include "GT_MenuScene.h"


GT_MenuScene::GT_MenuScene()
    :
      GT_Scene(menuScene),
      currButton_(0),
      camTimer_(0.0f),
      skybox_(GT_Locator::getSkybox()),
      ocean_(GT_Locator::getOcean()),
      menuCamFront_(glm::vec3(0.0f, 0.0f, 1.0f)),
      startAnimateGameplay_(false),
      stopAnimateGameplay_(false)
{
    buttons_.push_back("PLAY GAME");
    buttons_.push_back("CREDITS");
    buttons_.push_back("QUIT GAME");

    sound_.setBuffer(*(GT_Locator::getAudio()->getSoundBuffMap(OCEAN)));
    sound_.setVolume(40);
    sound_.setLoop(true);

    soundJet_.setBuffer(*(GT_Locator::getAudio()->getSoundBuffMap(ENGINE)));
    soundJet_.setVolume(0);

    carrier_ = GT_Locator::getUSSCarrier();
    fighter_ = GT_Locator::getFighter();

    soundJetPos_ = fighter_->getPosition().x;

    std::cout << " GT_MenuScene initialized " << std::endl;
}

void GT_MenuScene::checkKeyboardInput()
{
    if (keys_ != nullptr)
    {
        if (keys_[GLFW_KEY_Q])
            camTimer_-=3.0f;
        if (keys_[GLFW_KEY_E])
            camTimer_+=1.0f;
    }
}

void GT_MenuScene::sceneKeyboardHandler(bool* keys, int key, int scancode, int action, int mode)
{

    keys_ = keys;

    if (keys[GLFW_KEY_LEFT] || keys[GLFW_KEY_UP]  && action == GLFW_PRESS)
    {
        if (currButton_ == 0)
            currButton_ = buttons_.size() - 1;
        else
        {
            currButton_ = --currButton_ % buttons_.size();
        }
    }

    if (keys[GLFW_KEY_RIGHT] || keys[GLFW_KEY_DOWN] && action == GLFW_PRESS)
    {
        currButton_ = ++currButton_ % buttons_.size();
    }

    if (keys[GLFW_KEY_ENTER] && action == GLFW_PRESS)
    {
        if (currButton_ == 0)
        {
            startAnimateGameplay_ = true;
            soundJetPos_ = sceneCamera_->getCameraPos().x;
            if (stopAnimateGameplay_)
            {
                GT_Locator::getSceneManager()->activateScene( gameplay );
            }
        }

        if (currButton_ == 1)
            GT_Locator::getSceneManager()->activateScene( credits );
        if (currButton_ == 2)
            GT_Locator::getSceneManager()->activateScene( exitGame );
    }
}


void GT_MenuScene::initGameplayScene()
{
    sound_.setLoop(false);
    dirtySound_ = false;
    sound_.stop();

    dirtySoundJet_ = false;
    soundJet_.stop();

    GT_Locator::getSceneManager()->activateScene( gameplay );
    GT_Locator::getGameCamera()->setCameraPos(fighter_->getPosition());
    GT_Locator::getGameCamera()->setCameraFront(fighter_->getFront());
    GT_Locator::getGameCamera()->setCameraRight(fighter_->getRight());
    GT_Locator::getGameCamera()->setCameraUp(fighter_->getUp());
}

void GT_MenuScene::animateCam()
{
    sceneCamera_->setCameraPos(fighter_->getPosition() +  glm::vec3(sin(camTimer_*SPINFACTOR_), 0.0f, cos(camTimer_*SPINFACTOR_))*10.0f);
    sceneCamera_->setCameraFront(glm::normalize(fighter_->getPosition() - sceneCamera_->getCameraPos()));
}

void GT_MenuScene::animateGameplay()
{
    fighter_->setPosition(fighter_->getPosition() + fighter_->getFront()*5.2f);

    soundJet_.setVolume( (sceneCamera_->getCameraPos().x - soundJetPos_)/234.0f * (150.0f/6));

    if (dirtySoundJet_)
    {
        soundJet_.play();
        dirtySoundJet_ = false;
    }


    if (fighter_->getPosition().x > 400.0f)
    {
        startAnimateGameplay_ = false;
        stopAnimateGameplay_ = true;
        initGameplayScene();
    }
}

void GT_MenuScene::integrateScene(GLfloat deltaTime)
{
    //deltaTime *= SPINFACTOR_;
    camTimer_ += 1.0f;
    animateCam();
    if (startAnimateGameplay_)
        animateGameplay();
}

void GT_MenuScene::renderScene()
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


    for (int i = 0; i < buttons_.size(); i++)
    {
        GLfloat fontSize;
        if (i == currButton_)
            fontSize = 1.0f;
        else
            fontSize = 0.4f;

        font_->PrintLine(buttons_[i], 0.45f, 0.50f-i*0.050f, fontSize, glm::vec3(0.0, 0.3f, 0.2f));
    }


//    font_->PrintLine("Time: ", 25.0f, 25.0f, .50f, glm::vec3(1.0, 0.1f, 0.1f));
//    font_->PrintLine( std::to_string(glfwGetTime()), 120.0f, 25.0f, .50f, glm::vec3(0.5, 0.8f, 0.2f));
//    font_->PrintLine("FPS: ", 25.0f, 50.0f, .50f, glm::vec3(1.0, 0.1f, 0.1f));
//    font_->PrintLine( std::to_string(1/deltaTime_), 120.0f, 50.0f, .50f, glm::vec3(0.5, 0.8f, 0.2f));
//    font_->PrintLine("Speed: ", 25.0f, 75.0f, .50f, glm::vec3(1.0, 0.1f, 0.1f));
//    font_->PrintLine( std::to_string(sceneCamera_->getSpeed()), 120.0f, 75.0f, .50f, glm::vec3(0.5, 0.8f, 0.2f));
//    font_->PrintLine("Altitude: ", 25.0f, 100.0f, .50f, glm::vec3(1.0, 0.1f, 0.1f));
//    font_->PrintLine( std::to_string(sceneCamera_->getCameraPos().y), 120.0f, 100.0f, .50f, glm::vec3(0.5, 0.8f, 0.2f));

    ////////////////////////   rendering phase   /////////////////////////////

    skybox_->Draw(sceneCamera_);
    ocean_->draw(sceneCamera_);

    carrier_->Draw(sceneCamera_);
    fighter_->Draw(sceneCamera_);

    if (dirtySound_)
    {
        sound_.play();
        dirtySound_ = false;
    }


    ///////////////////////////////////////////////////////////////////////
}
