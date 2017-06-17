#include "GT_GameplayScene.h"

#include <set>

GT_GameplayScene::GT_GameplayScene()
    :
      GT_Scene(gameplay),
      skybox_(GT_Locator::getSkybox()),
      ocean_(GT_Locator::getOcean()),
      level_(0)
{

    fighter_= GT_Locator::getFighter();
    ussCarrier_ = GT_Locator::getUSSCarrier();
    hud_ = GT_Locator::getHUD();
    hud_->setRadarEnemyListPtr(enemies_);

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

void GT_GameplayScene::printMSG()
{
    font_->PrintLine("Time: ", 25.0f, 25.0f, .50f, glm::vec3(1.0, 0.1f, 0.1f));
    font_->PrintLine( std::to_string(glfwGetTime()), 120.0f, 25.0f, .50f, glm::vec3(0.5, 0.8f, 0.2f));
    font_->PrintLine("FPS: ", 25.0f, 50.0f, .50f, glm::vec3(1.0, 0.1f, 0.1f));
    font_->PrintLine( std::to_string(60.0f - 1/deltaTime_), 120.0f, 50.0f, .50f, glm::vec3(0.5, 0.8f, 0.2f));
    font_->PrintLine("Speed: ", 25.0f, 75.0f, .50f, glm::vec3(1.0, 0.1f, 0.1f));
    font_->PrintLine( std::to_string(sceneCamera_->getSpeed()), 120.0f, 75.0f, .50f, glm::vec3(0.5, 0.8f, 0.2f));
    font_->PrintLine("Altitude: ", 25.0f, 100.0f, .50f, glm::vec3(1.0, 0.1f, 0.1f));
    font_->PrintLine( std::to_string(sceneCamera_->getCameraPos().y), 120.0f, 100.0f, .50f, glm::vec3(0.5, 0.8f, 0.2f));
    font_->PrintLine("Level: ", 25.0f, 125.0f, .50f, glm::vec3(1.0, 0.1f, 0.1f));
    font_->PrintLine( std::to_string(level_), 120.0f, 125.0f, .50f, glm::vec3(0.5, 0.8f, 0.2f));
    font_->PrintLine("Bogies: ", 25.0f, 150.0f, .50f, glm::vec3(1.0, 0.1f, 0.1f));
    font_->PrintLine( std::to_string(enemies_.size()), 120.0f, 150.0f, .50f, glm::vec3(0.5, 0.8f, 0.2f));

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


    if (fighter_->targetLocked())
    {
        font_->PrintLine("TARGET LOCKED!", 800.0f, 100.0f, .4f, glm::vec3(1.0, 0.1f, 0.1f));
        font_->PrintLine("        FIRE!    ", 800.0f, 120.0f, .50f, glm::vec3(1.0, 0.1f, 0.1f));
    }

    if (fighter_->isTarget())
    {
        if (sin(glfwGetTime()*20) > -0.7)
        {
            font_->PrintLine("LOCKED!", 820.0f, 200.0f, .60f, glm::vec3(1.0, 0.1f, 0.1f));
        }
    }

    if (fighter_->evade())
    {
        if (sin(glfwGetTime()*20) > 0)
        {
            font_->PrintLine(" EVADE!", 820.0f, 250.0f, .60f, glm::vec3(1.0, 0.1f, 0.1f));
        }
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

    ////////////////////////   rendering phase   /////////////////////////////

    skybox_->Draw(sceneCamera_);
    ocean_->draw(sceneCamera_);

    renderAircrafts();

    hud_->draw(sceneCamera_);

    printMSG();
    // TODO replace with HUD


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
    fighter_->lock( nullptr );
    fighter_->isTarget(false);
    glm::vec3 dir;
    GLfloat distance;

    for (auto enit = enemies_.begin(); enit != enemies_.end(); enit++)
    {
        if ((*enit)->explode_)
            continue;

        //////////////////////////////////////////////////////////////

        dir = (*enit)->getPosition() - fighter_->getPosition();
        distance = glm::length(dir);
        dir = glm::normalize(dir);

        if (distance < 2.0f * aimRange_)
        {
            if (!fighter_->targetLocked())
                if (glm::dot(dir, sceneCamera_->getCameraFront()) > 0.995)
                    fighter_->lock(*enit);
        }

        if (distance < aimRange_)
        {
            (*enit)->setPredatorMode(true);
            (*enit)->lock(fighter_);
            fighter_->isTarget(true);
        }
        else
        {
            (*enit)->setPredatorMode(false);
            (*enit)->lock(GT_Locator::getUSSCarrier());
        }


        ///////////////////////////////////////////////////////////////
        dir = (*enit)->target_->getPosition() - (*enit)->getPosition();
        distance = glm::length(dir);

        if ((*enit)->targetLocked())
            if (distance < weaponRange_)
            {
                (*enit)->fireMissile();
            }

    }
}

void GT_GameplayScene::integrateScene(GLfloat deltaTime)
{
    // Fly movement
    sceneCamera_->keyboardHandler(FORWARD, deltaTime);

    integrateAircrafts(deltaTime);

    checkCrosshair();
}


void GT_GameplayScene::nextLevel()
{
    level_++;

    for (int i = 0; i < level_ * 2 - 1  ; i++ )
    {
        enemies_.push_back(new GT_Enemy());
    }

    std::cout << "Level " << level_ << std::endl;
}

void GT_GameplayScene::integrateAircrafts(GLfloat deltaTime)
{
    fighter_->Integrate(sceneCamera_, deltaTime);

    auto eit = enemies_.begin();

    while ( eit != enemies_.end() )
    {
        if ((*eit)->isDead())
        {
           // delete *eit;
            eit = enemies_.erase(eit);
            continue;
        }

        (*eit)->Integrate(sceneCamera_, deltaTime);
        eit++;
        //enemies_[i]->Integrate(sceneCamera_, deltaTime);
    }

  //  std::cout << enemies_.size() << "level : "<< level_ << std::endl;
    if (enemies_.size() == 0)
        nextLevel();
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
    if (fighter_->targetLocked())
        fighter_->fireMissile();
}
