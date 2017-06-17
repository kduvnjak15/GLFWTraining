#pragma once

#include "GT_Model.h"
#include "GT_Primitive.h"
#include "GT_Radar.h"

#include "GT_Enemy.h"

#include <vector>

class GT_HUD : public GT_Primitive
{
public:

    GT_HUD();

    void setRadarEnemyListPtr(std::vector<GT_Enemy*> &enemyList) { radar_->setRadarEnemyList(enemyList); }
    void appendMissile(GT_Missile* missileInstance) { radar_->appendMissileToRadarList(missileInstance); }

    void draw(GT_Camera *tempCam);

    typedef struct
    {
        glm::vec3 position_;
        glm::vec3 volume_;
    }panel;

private:

    std::vector<panel> panelSettings_;
    std::vector<GT_Model*> actors_;
    GT_Radar* radar_;

    void defineShader();

    void definePanels();

    void installRadar();

};


