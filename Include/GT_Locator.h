#pragma once

#include <iostream>

#include "GT_Ocean.h"
#include "GT_Skybox.h"
#include "GT_Aircraft.h"
#include "GT_Particle.h"
#include "GT_Alphabet.h"
#include "GT_Fighter.h"
#include "GT_USSCarrier.h"


class Audio
{
public:
    virtual ~Audio() {}
    virtual void playSound(int soundID) = 0;
    virtual void stopSound(int soundID) = 0;
    virtual void stopAllSounds() = 0;
};

class ConsoleAudio : public Audio
{
public:
    virtual void playSound(int soundID)
    {
        std::cout << "playSound "<< std::endl;
    }

    virtual void stopSound(int soundID)
    {
        std::cout << "stopSound" << std::endl;
    }

    virtual void stopAllSounds()
    {
        std::cout << "stopped all Sounds" << std::endl;
    }
};

class GT_Locator
{
public:


    GT_Locator()
    {

        skybox_ = new GT_Skybox();
        ocean_  = new GT_Ocean();
        particle_  = new GT_Particle();
        gameCamera_ = new GT_Camera();
        menuCamera_ = new GT_Camera();
        fonts_      = new GT_Alphabet(menuCamera_);

        models_.push_back(new GT_Model("../Content/FA-22_Raptor/FA-22_Raptor.obj"));
        modelMap_.insert(std::pair<MODEL_TYPE, GT_Model*>(F22, models_[0]));
        models_.push_back(new GT_Model("../Content/FA-18_RC/FA-18_RC.obj"));
        modelMap_.insert(std::pair<MODEL_TYPE, GT_Model*>(F18, models_[1]));
        models_.push_back(new GT_Model("../Content/AVMT300/AVMT300.3ds"));
        modelMap_.insert(std::pair<MODEL_TYPE, GT_Model*>(AIM, models_[2]));
        models_.push_back(new GT_Model("../Content/CV - Essex class/essex_scb-125_generic.obj"));
        modelMap_.insert(std::pair<MODEL_TYPE, GT_Model*>(USS, models_[3]));


        actors_.push_back(new GT_Actor(modelMap_[F18]));
        actorMap_.insert(std::pair<MODEL_TYPE, GT_Actor*>(F18, actors_[0]));
        actors_.push_back(new GT_Actor(modelMap_[F22]));
        actorMap_.insert(std::pair<MODEL_TYPE, GT_Actor*>(F22, actors_[1]));
        actors_.push_back(new GT_Actor(modelMap_[USS]));
        actorMap_.insert(std::pair<MODEL_TYPE, GT_Actor*>(USS, actors_[2]));
        std::cout <<" GT_Locator constructor " << std::endl;

        fighter_ = new GT_Fighter();
        ussCarrier_ = new GT_USSCarrier();

    }

    //getters

    static Audio* getAudio()
    {
        return service_;
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

    // providers

    static void provide(Audio* service)
    {
        service_ = service;
    }

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

    static Audio*       service_;
    static GT_Ocean*    ocean_;
    static GT_Skybox*   skybox_;
    static GT_Particle* particle_;
    static GT_Camera*   gameCamera_;
    static GT_Camera*   menuCamera_;
    static GT_Alphabet* fonts_;

    static GT_Fighter*  fighter_;
    static GT_USSCarrier*   ussCarrier_;

    static std::vector<GT_Model*> models_;
    static std::map<MODEL_TYPE, GT_Model*> modelMap_;
    static std::map<MODEL_TYPE, GT_Actor*> actorMap_;
    static std::vector<GT_Actor*> actors_;


};


