#include "GT_GameplayScene.h"

#include <set>

GT_GameplayScene::GT_GameplayScene()
    :
      GT_Scene(gameplay),
      skybox_(GT_Locator::getSkybox()),
      ocean_(GT_Locator::getOcean()),
      level_(0)
{

    enemies_.push_back(new GT_Enemy());
    enemies_.push_back(new GT_Enemy());
    enemies_.push_back(new GT_Enemy());
    enemies_.push_back(new GT_Enemy());
    enemies_.push_back(new GT_Enemy());
    enemies_.push_back(new GT_Enemy());

    fighter_= GT_Locator::getFighter();
    ussCarrier_ = GT_Locator::getUSSCarrier();

    nextScene_ = gameplay;
}

void GT_GameplayScene::checkKeyboardInput()
{
    if (keys_ != nullptr)
    {

        sceneCamera_->keyboardHandler(keysEnable_, deltaTime_);

        if (keys_[GLFW_KEY_LEFT_CONTROL])
            sceneCamera_->keyboardHandler(ACCELERATE, deltaTime_);
        if (keys_[GLFW_KEY_LEFT_SHIFT])
            sceneCamera_->keyboardHandler(DECELERATE, deltaTime_);
    }
}

void GT_GameplayScene::renderScene()
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

    font_->PrintLine("Time: ", 25.0f, 25.0f, .50f, glm::vec3(1.0, 0.1f, 0.1f));
    font_->PrintLine( std::to_string(glfwGetTime()), 120.0f, 25.0f, .50f, glm::vec3(0.5, 0.8f, 0.2f));
    font_->PrintLine("FPS: ", 25.0f, 50.0f, .50f, glm::vec3(1.0, 0.1f, 0.1f));
    font_->PrintLine( std::to_string(60.0f - 1/deltaTime_), 120.0f, 50.0f, .50f, glm::vec3(0.5, 0.8f, 0.2f));
    font_->PrintLine("Speed: ", 25.0f, 75.0f, .50f, glm::vec3(1.0, 0.1f, 0.1f));
    font_->PrintLine( std::to_string(sceneCamera_->getSpeed()), 120.0f, 75.0f, .50f, glm::vec3(0.5, 0.8f, 0.2f));
    font_->PrintLine("Altitude: ", 25.0f, 100.0f, .50f, glm::vec3(1.0, 0.1f, 0.1f));
    font_->PrintLine( std::to_string(sceneCamera_->getCameraPos().y), 120.0f, 100.0f, .50f, glm::vec3(0.5, 0.8f, 0.2f));

    font_->PrintLine("Pos", 120.0f, 380.0f, .50f, glm::vec3(1.0, 0.1f, 0.1f));
    font_->PrintLine( std::to_string(sceneCamera_->getCameraPos().x), 20.0f, 460.0f, .40f, glm::vec3(0.5, 0.8f, 0.2f));
    font_->PrintLine( std::to_string(sceneCamera_->getCameraPos().y), 20.0f, 430.0f, .40f, glm::vec3(0.5, 0.8f, 0.2f));
    font_->PrintLine( std::to_string(sceneCamera_->getCameraPos().z), 20.0f, 400.0f, .40f, glm::vec3(0.5, 0.8f, 0.2f));


    font_->PrintLine("Front ", 120.0f, 380.0f, .50f, glm::vec3(1.0, 0.1f, 0.1f));
    font_->PrintLine( std::to_string(sceneCamera_->getCameraFront().x), 120.0f, 460.0f, .40f, glm::vec3(0.5, 0.8f, 0.2f));
    font_->PrintLine( std::to_string(sceneCamera_->getCameraFront().y), 120.0f, 430.0f, .40f, glm::vec3(0.5, 0.8f, 0.2f));
    font_->PrintLine( std::to_string(sceneCamera_->getCameraFront().z), 120.0f, 400.0f, .40f, glm::vec3(0.5, 0.8f, 0.2f));

    font_->PrintLine("Right ", 250.0f, 380.0f, .50f, glm::vec3(1.0, 0.1f, 0.1f));
    font_->PrintLine( std::to_string(sceneCamera_->getCameraRight().x), 250.0f, 460.0f, .40f, glm::vec3(0.5, 0.8f, 0.2f));
    font_->PrintLine( std::to_string(sceneCamera_->getCameraRight().y), 250.0f, 430.0f, .40f, glm::vec3(0.5, 0.8f, 0.2f));
    font_->PrintLine( std::to_string(sceneCamera_->getCameraRight().z), 250.0f, 400.0f, .40f, glm::vec3(0.5, 0.8f, 0.2f));

//    font_->PrintLine("GAMEPLAY", 600.0f, 400.0f, 1.0f, glm::vec3(1.0, 0.1f, 0.1f));

    ////////////////////////   rendering phase   /////////////////////////////

    skybox_->Draw(sceneCamera_);
    ocean_->draw(sceneCamera_);

    renderAircrafts();

    ///////////////////////////////////////////////////////////////////////
}

void GT_GameplayScene::sceneKeyboardHandler(bool* keys, int key, int scancode, int action, int mode)
{

    keys_ = keys;

    if (keys[GLFW_KEY_ESCAPE] && action == GLFW_PRESS)
    {
        nextScene_ = pauseScene;
    }

    if (keys[GLFW_KEY_ENTER] && action == GLFW_PRESS)
    {
        missileFIRE();
    }

    if (action == GLFW_PRESS)
    {
        keysEnable_.insert(key);
    }
    else if (action == GLFW_RELEASE)
    {
        auto it = keysEnable_.find(key);
        if (it != keysEnable_.end())
            keysEnable_.erase( keysEnable_.find(key) );
    }

    if (keysEnable_.size()> 0)
        keyPressed_ = true;
    else
        keyPressed_ = false;

}

void GT_GameplayScene::checkCrosshair()
{
    GT_Enemy* locked_ = nullptr;
    glm::vec3 dir;
    for (auto mit = enemies_.begin(); mit != enemies_.end(); mit++)
    {
        dir = glm::normalize((*mit)->getPosition() - fighter_->getPosition()) ;

        if ( glm::dot(dir, sceneCamera_->getCameraFront()) > 0.995 )
               locked_ = *mit;
        break;
    }

    fighter_->lock(locked_);
}

void GT_GameplayScene::integrateScene(GLfloat deltaTime)
{
    // Fly movement
    sceneCamera_->keyboardHandler(FORWARD, deltaTime);

    integrateAircrafts(deltaTime);
    checkCrosshair();
}

void GT_GameplayScene::integrateAircrafts(GLfloat deltaTime)
{
    fighter_->setPosition(sceneCamera_->getCameraPos() + sceneCamera_->getCameraUp()*-3.0f);
    fighter_->Integrate(sceneCamera_, deltaTime);

    for (int i = 0; i < enemies_.size(); i++)
    {
        enemies_[i]->Integrate(sceneCamera_, deltaTime);
        enemies_[i]->setPosition(glm::vec3(100.0f * i, 400.0f, 0.0f));
    }
}

void GT_GameplayScene::renderAircrafts()
{
    fighter_->Draw(sceneCamera_);
    ussCarrier_->Draw(sceneCamera_);

    for (int i = 0; i < enemies_.size(); i++)
        enemies_[i]->Draw(sceneCamera_);
}

void GT_GameplayScene::missileFIRE()
{
//    for( auto it = enemies_.begin(); it != enemies_.end(); it++)
//    {
//        (*it)->fireMissile();
//    }

    if (fighter_->isLocked())
        fighter_->fireMissile();
}
