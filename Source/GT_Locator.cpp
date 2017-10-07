#include "GT_Locator.h"

#include "SFML/Audio.hpp"

GT_Ocean*    GT_Locator::ocean_;
GT_Skybox*   GT_Locator::skybox_;
GT_Particle* GT_Locator::particle_;
GT_Camera*   GT_Locator::gameCamera_;
GT_Camera*   GT_Locator::menuCamera_;
GT_Alphabet* GT_Locator::fonts_;
GT_HUD*      GT_Locator::hud_;
GT_Audio*    GT_Locator::audio_;
GT_SManager* GT_Locator::sceneManager_;

// SOUNDS
sf::SoundBuffer* GT_Locator::sfSpray_;
sf::SoundBuffer* GT_Locator::sfLand_;

GT_Fighter*  GT_Locator::fighter_;
GT_USSCarrier*  GT_Locator::ussCarrier_;

std::vector<GT_Model*>          GT_Locator::models_;
std::map<MODEL_TYPE, GT_Model*> GT_Locator::modelMap_;
std::map<MODEL_TYPE, GT_Actor*> GT_Locator::actorMap_;
std::vector<GT_Actor*>          GT_Locator::actors_;

GT_Locator::GT_Locator()
{

    skybox_ = new GT_Skybox();
    ocean_  = new GT_Ocean();
    particle_  = new GT_Particle();
    gameCamera_ = new GT_Camera();
    menuCamera_ = new GT_Camera();
    fonts_      = new GT_Alphabet(menuCamera_);
    hud_        = new GT_HUD();
    audio_      = new GT_Audio();
    sfSpray_     = new sf::SoundBuffer();
    sfLand_     = new sf::SoundBuffer();



    models_.push_back(new GT_Model("../Content/FA-22_Raptor/FA-22_Raptor.obj"));
    modelMap_.insert(std::pair<MODEL_TYPE, GT_Model*>(F22, models_[0]));
    models_.push_back(new GT_Model("../Content/FA-18_RC/FA-18_RC.obj"));
    modelMap_.insert(std::pair<MODEL_TYPE, GT_Model*>(F18, models_[1]));
    models_.push_back(new GT_Model("../Content/AVMT300/AVMT300.3ds"));
    modelMap_.insert(std::pair<MODEL_TYPE, GT_Model*>(AIM, models_[2]));
    models_.push_back(new GT_Model("../Content/CV - Essex class/essex_scb-125_generic.obj"));
    modelMap_.insert(std::pair<MODEL_TYPE, GT_Model*>(USS, models_[3]));

    // sounds
    sfSpray_->loadFromFile("../Content/spray.wav");
    sfLand_->loadFromFile("../Content/land.flac");

    actors_.push_back(new GT_Actor(modelMap_[F18]));
    actorMap_.insert(std::pair<MODEL_TYPE, GT_Actor*>(F18, actors_[0]));
    actors_.push_back(new GT_Actor(modelMap_[F22]));
    actorMap_.insert(std::pair<MODEL_TYPE, GT_Actor*>(F22, actors_[1]));
    actors_.push_back(new GT_Actor(modelMap_[USS]));
    actorMap_.insert(std::pair<MODEL_TYPE, GT_Actor*>(USS, actors_[2]));


    std::cout <<" GT_Locator constructor " << std::endl;

    fighter_ = new GT_Fighter();
    ussCarrier_ = new GT_USSCarrier();



    // has to be last ? // lousy design
    sceneManager_ = new GT_SManager();
}



GT_Locator::~GT_Locator()
{
    if (audio_)     delete audio_;
    if (ocean_)     delete ocean_;
    if (skybox_)    delete skybox_;
    if (particle_)  delete particle_;
    if (hud_)       delete hud_;
    if (fonts_)     delete fonts_;
    if (gameCamera_)delete gameCamera_;
    if (menuCamera_)delete menuCamera_;

    if (sfSpray_)   delete sfSpray_;
    if (sfLand_)    delete sfLand_;
    if (sceneManager_) delete sceneManager_;


    for (auto it = models_.begin(); it != models_.end(); )
    {
        it = models_.erase(it);
    }

    std::cout << "Models have " << models_.size() << "elements left"<< std::endl;

    for (auto it = actors_.begin(); it != actors_.end();)
    {
        it = actors_.erase(it);
    }

    std::cout << "Actors have " << models_.size() << "elements left"<< std::endl;

    delete fighter_;
    delete ussCarrier_;


}
