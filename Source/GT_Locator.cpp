#include "GT_Locator.h"

Audio*       GT_Locator::service_;
GT_Ocean*    GT_Locator::ocean_;
GT_Skybox*   GT_Locator::skybox_;
GT_Particle* GT_Locator::particle_;
GT_Camera*   GT_Locator::gameCamera_;
GT_Camera*   GT_Locator::menuCamera_;

std::vector<GT_Model*>          GT_Locator::models_;
std::map<MODEL_TYPE, GT_Model*> GT_Locator::modelMap_;
std::map<MODEL_TYPE, GT_Actor*> GT_Locator::actorMap_;
std::vector<GT_Actor*>          GT_Locator::actors_;


GT_Locator::~GT_Locator()
{
    if (service_)   delete service_;
    if (ocean_)     delete ocean_;
    if (skybox_)    delete skybox_;
    if (particle_)  delete particle_;


    for (auto it = models_.begin(); it != models_.end(); it++)
    {
        models_.erase(it);
    }

    for (auto it = actors_.begin(); it != actors_.end(); it++)
    {
        actors_.erase(it);
    }


}
