#include "GT_Locator.h"

GT_Ocean*    GT_Locator::ocean_;
GT_Skybox*   GT_Locator::skybox_;
GT_Particle* GT_Locator::particle_;
GT_Camera*   GT_Locator::gameCamera_;
GT_Camera*   GT_Locator::menuCamera_;
GT_Alphabet* GT_Locator::fonts_;
GT_HUD*      GT_Locator::hud_;
GT_Audio*    GT_Locator::audio_;
GT_Image*    GT_Locator::wallp_;

GT_Fighter*  GT_Locator::fighter_;
GT_USSCarrier*  GT_Locator::ussCarrier_;

std::vector<GT_Model*>          GT_Locator::models_;
std::map<MODEL_TYPE, GT_Model*> GT_Locator::modelMap_;
std::map<MODEL_TYPE, GT_Actor*> GT_Locator::actorMap_;
std::vector<GT_Actor*>          GT_Locator::actors_;


GT_Locator::~GT_Locator()
{
    if (audio_)     delete audio_;
    if (ocean_)     delete ocean_;
    if (skybox_)    delete skybox_;
    if (particle_)  delete particle_;
    if (hud_)       delete hud_;

    for (auto it = models_.begin(); it != models_.end(); it++)
    {
        models_.erase(it);
    }

    for (auto it = actors_.begin(); it != actors_.end(); it++)
    {
        actors_.erase(it);
    }


}
