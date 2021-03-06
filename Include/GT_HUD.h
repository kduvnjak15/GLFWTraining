#pragma once

#include "GT_Model.h"
#include "GT_Primitive.h"
#include "GT_Radar.h"
#include "GT_Image.h"

#include "GT_Enemy.h"

#include <vector>

class GT_HUD : public GT_Primitive
{
public:

    friend class GT_Radar;

    GT_HUD();

    void setRadarEnemyListPtr(std::vector<GT_Enemy*> &enemyList) { radar_->setRadarEnemyList(enemyList); }
    void appendMissile(GT_Missile* missileInstance) { radar_->appendMissileToRadarList(missileInstance); }

    void draw(GT_Camera *tempCam);

    typedef struct
    {
        glm::vec2 topLeft;
        glm::vec2 bottomRight;
    }panel;

private:

    std::vector<panel> panelSettings_;
    std::vector<GT_Model*> actors_;
    GT_Radar* radar_;

    void defineShader();

    void definePanels();

    void installRadar();

    GT_Image panel_ = GT_Image(0.0f, 0.0f, 0.0f, 0.45f);

};

