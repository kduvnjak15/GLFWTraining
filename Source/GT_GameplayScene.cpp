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

    sound_.setBuffer(*(GT_Locator::getAudio()->getSoundBuffMap(ENGINE)));
    sound_.setLoop(true);
    sound_.setVolume(soundVolume_);

    beepSound_.setBuffer(*(GT_Locator::getAudio()->getSoundBuffMap(BEEP)));

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
    font_->PrintLine("Time: ", 0.015f, 0.05f, .50f, glm::vec3(1.0, 0.1f, 0.1f));
    font_->PrintLine( std::to_string(glfwGetTime()), 0.07f, 0.05f, .50f, glm::vec3(0.5, 0.8f, 0.2f));
    font_->PrintLine("FPS: ",  0.015f, 0.07f, .50f, glm::vec3(1.0, 0.1f, 0.1f));
    font_->PrintLine( std::to_string(60.0f - 1/deltaTime_), 0.07f, 0.07f, .50f, glm::vec3(0.5, 0.8f, 0.2f));
    font_->PrintLine("Speed: ", 0.015f, 0.09f, .50f, glm::vec3(1.0, 0.1f, 0.1f));
    font_->PrintLine( std::to_string(sceneCamera_->getSpeed()), 0.07f, 0.09f, .50f, glm::vec3(0.5, 0.8f, 0.2f));
    font_->PrintLine("Altitude: ", 0.015f, 0.11f, .50f, glm::vec3(1.0, 0.1f, 0.1f));
    font_->PrintLine( std::to_string(sceneCamera_->getCameraPos().y), 0.07f, 0.11f, .50f, glm::vec3(0.5, 0.8f, 0.2f));
    font_->PrintLine("Level: ", 0.015f, 0.13f, .50f, glm::vec3(1.0, 0.1f, 0.1f));
    font_->PrintLine((fighter_->isFreeMode() ? "free mode" : std::to_string(level_)),0.07f, 0.13, .50f, glm::vec3(0.5, 0.8f, 0.2f));
    font_->PrintLine("Bogies: ", 0.015f, 0.15, .50f, glm::vec3(1.0, 0.1f, 0.1f));
    font_->PrintLine( std::to_string(enemies_.size()), 0.07f, 0.15f, .50f, glm::vec3(0.5, 0.8f, 0.2f));

    font_->PrintLine("Missiles: ", 0.015f, 0.17f, .50f, glm::vec3(1.0, 0.1f, 0.1f));
    font_->PrintLine( std::to_string(fighter_->getNumOfMissiles_()), 0.07f, 0.17f, .50f, glm::vec3(0.5, 0.8f, 0.2f));


    glm::vec3 activeCol(1.0f, 0.1f, 0.1f);
    glm::vec3 passiveCol(0.01f, 0.01f, 0.01f);
    glm::vec3 tempColor;

    tempColor = fighter_->targetLocked() ? activeCol : passiveCol;
    font_->PrintLine("TARGET LOCKED!", 0.820f, 0.08f, .4f, tempColor);
    font_->PrintLine("        FIRE!    ", 0.8250f, 0.05f, .50f, tempColor);


    if (tooFarMSGtime_ > 0 )
    {
        font_->PrintLine("TOO FAR!", 0.498, 0.91, .60f, glm::vec3(1.0, 0.1f, 0.1f));

        font_->PrintLine("Get back in arena!!!", 0.495, 0.90, .60f, glm::vec3(1.0, 0.1f, 0.1f));

        if (glfwGetTime() - tooFarMSGtime_ > 1.0f)
        {
            std::cout << tooFarMSGtime_ << std::endl;
            tooFarMSGtime_ = -1;
        }
    }


    tempColor = passiveCol;
    if (fighter_->isTarget())
    {
        if (sin(glfwGetTime()*10) > -0.7)
        {
            if (!fighter_->evade())
            {
                beepSound_.setVolume(20);
                beepSound_.play();
            }
            tempColor = activeCol;
        }
        else
        {
            beepSound_.stop();
            tempColor = passiveCol;
        }
    }
    font_->PrintLine("LOCKED!", 0.8300f, 0.135f, .60f, tempColor);


    tempColor = passiveCol;
    if (fighter_->evade())
    {
        if (sin(glfwGetTime()*20) > 0)
        {
            beepSound_.setVolume(100);
            beepSound_.play();
            tempColor = activeCol;
        }
        else
        {
            beepSound_.stop();
            tempColor = passiveCol;
        }
    }
    font_->PrintLine("EVADE!", 0.8350f, 0.210f, .65f, tempColor);


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

    if (dirtySound_)
    {
        sound_.play();
        dirtySound_ = false;
    }

    printMSG();
    // TODO replace with HUD


    ///////////////////////////////////////////////////////////////////////
}

void GT_GameplayScene::sceneKeyboardHandler(bool* keys, int key, int scancode, int action, int mode)
{

    keys_ = keys;

    if (keys[GLFW_KEY_ESCAPE] && action == GLFW_PRESS)
    {
        GT_Locator::getSceneManager()->activateScene( pauseScene );
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

    if (this->fighter_->explode_ || this->ussCarrier_->explode_)
        GT_Locator::getSceneManager()->activateScene( gameover );

    if (level_ > 2 && !(fighter_->isFreeMode()))
    {
        GT_Locator::getSceneManager()->activateScene( gameover );
    }

    // Fly movement
    sceneCamera_->keyboardHandler(FORWARD, deltaTime);

    integrateAircrafts(deltaTime);

    GLfloat temp_speed = GT_Locator::getGameCamera()->getSpeed();
    sound_.setVolume((temp_speed / MAX_SPEED)*100);


    GLfloat gravityFactor = (100 - temp_speed ) / 100 ;
    if (gravityFactor >= 0 && sceneCamera_->getCameraPos().y > 5)
        sceneCamera_->enforceGravity(gravityFactor);

    if (sceneCamera_->getCameraPos().y < 10)
        GT_Locator::getSceneManager()->activateScene( gameover );

    checkCrosshair();
}


void GT_GameplayScene::nextLevel()
{
    level_++;

    for (int i = 0; i < level_ * 2 - 1  ; i++ )
    {
        enemies_.push_back(new GT_Enemy());
        //fighter_->appendMissiles(3);
    }

    std::cout << "Level " << level_ << std::endl;
}

void GT_GameplayScene::checkForBouncingBox()
{
    if (sceneCamera_->getCameraPos().y > bouncingBoxThreshold_ / 2.0)
    {
        sceneCamera_->setCameraFront(-1.0f*sceneCamera_->getCameraFront());
    }

    if (sceneCamera_->getCameraPos().x > bouncingBoxThreshold_ || sceneCamera_->getCameraPos().x < -bouncingBoxThreshold_ ||
            sceneCamera_->getCameraPos().z > bouncingBoxThreshold_ || sceneCamera_->getCameraPos().z < -bouncingBoxThreshold_ )
    {

        std::cout << sceneCamera_->getCameraPos().x << std::endl;
        std::cout << sceneCamera_->getCameraPos().y << std::endl;
        std::cout << sceneCamera_->getCameraPos().z << std::endl;
        sceneCamera_->setCameraFront(-1.0f*sceneCamera_->getCameraFront());
        sceneCamera_->setCameraRight(-1.0f*sceneCamera_->getCameraRight());

        tooFarMSGtime_ = glfwGetTime();

    }


}

void GT_GameplayScene::integrateAircrafts(GLfloat deltaTime)
{

    checkForBouncingBox();

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
