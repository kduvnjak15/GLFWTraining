#pragma once

#include <iostream>

#include "GT_Ocean.h"
#include "GT_Skybox.h"
#include "GT_Aircraft.h"
#include "GT_Particle.h"
#include "GT_Alphabet.h"
#include "GT_Fighter.h"
#include "GT_USSCarrier.h"
#include "GT_HUD.h"
#include "GT_Audio.h"
#include "GT_Image.h"
#include "GT_SManager.h"

#define PI 3.1415926

class GT_Locator
{
public:


    GT_Locator();

    //getters

    static GT_SManager* getSceneManager()
    {
        return sceneManager_;
    }

    static GT_Audio* getAudio()
    {
        return audio_;
    }

    static GT_Ocean* getOcean()
    {
        return ocean_;
    }

    static GT_Skybox* getSkybox()
    {
        return skybox_;
    }

    static GT_Particle* getParticle()
    {
        return particle_;
    }

    static GT_Model* getModel(MODEL_TYPE modelEnum)
    {
        return modelMap_[modelEnum];
    }

    static GT_Actor* getActor(MODEL_TYPE modelEnum)
    {
        return actorMap_[modelEnum];
    }

    static GT_Camera* getGameCamera()
    {
        return gameCamera_;
    }

    static GT_Camera* getMenuCamera()
    {
        return menuCamera_;
    }

    static GT_Alphabet* getFonts()
    {
        return fonts_;
    }

    static GT_Fighter* getFighter()
    {
        return fighter_;
    }

    static GT_USSCarrier* getUSSCarrier()
    {
        return ussCarrier_;
    }

    static GT_HUD* getHUD()
    {
        return hud_;
    }

    static sf::SoundBuffer* getSpray()
    {
        return sfSpray_;
    }

    static sf::SoundBuffer* getLand()
    {
        return sfLand_;
    }

    // providers

    static void provide(GT_Skybox* skybox)
    {
        skybox_ = skybox;
    }

    static void provide(GT_Ocean* ocean)
    {
        ocean_ = ocean;
    }

    static void provide(GT_Particle* particle)
    {
        particle_ = particle;
    }

    ~GT_Locator();


private:

    static GT_Ocean*    ocean_;
    static GT_Skybox*   skybox_;
    static GT_Particle* particle_;
    static GT_Camera*   gameCamera_;
    static GT_Camera*   menuCamera_;
    static GT_Alphabet* fonts_;
    static GT_HUD*      hud_;
    static GT_Audio*    audio_;
    static GT_SManager*    sceneManager_;

    static sf::SoundBuffer* sfSpray_;
    static sf::SoundBuffer* sfLand_;

    static GT_Fighter*  fighter_;
    static GT_USSCarrier*   ussCarrier_;

    static std::vector<GT_Model*> models_;
    static std::map<MODEL_TYPE, GT_Model*> modelMap_;
    static std::map<MODEL_TYPE, GT_Actor*> actorMap_;
    static std::vector<GT_Actor*> actors_;


};


