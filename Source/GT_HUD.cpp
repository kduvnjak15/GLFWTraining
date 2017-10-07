#include "GT_HUD.h"

#include "SOIL/src/SOIL.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

GT_HUD::GT_HUD()
    : GT_Primitive()
{
    sideA_ = 1.0f;
    sideB_ = sideA_;

    transparent_ = true;

    initValues();
    defineVAO();
    defineShader();

    definePanels();
    installRadar();
}


void GT_HUD::defineShader()
{
    primitiveShader_ = new GT_Shader(particleShader, "../Shaders/HUDShader.vs", "../Shaders/HUDShader.fs");
    modelLoc_   =  glGetUniformLocation(primitiveShader_->shaderProgram_, "model");
    viewLoc_    = glGetUniformLocation(primitiveShader_->shaderProgram_, "view");
    projLoc_    = glGetUniformLocation(primitiveShader_->shaderProgram_, "projection");
}

void GT_HUD::definePanels()
{
    // Radar
    panel Radar;
    Radar.topLeft        = glm::vec2(0.6, -0.98);
    Radar.bottomRight    = glm::vec2(0.95, -0.45);

    // Mission objectives
    panel Evade;
    Evade.topLeft      = glm::vec2(0.60, 0.8);
    Evade.bottomRight  = glm::vec2(0.85, 0.92);

    panel Locked;
    Locked.topLeft     = glm::vec2(0.60, 0.65);
    Locked.bottomRight = glm::vec2(0.85, 0.78);

    panel fireLocked;
    fireLocked.topLeft       = glm::vec2(0.60, 0.50);
    fireLocked.bottomRight   = glm::vec2(0.85, 0.63);

    // Info
    panel Info;
    Info.topLeft       = glm::vec2(-0.98, 0.6);
    Info.bottomRight   = glm::vec2(-0.70, 0.92);

    panelSettings_.push_back(Info);
    panelSettings_.push_back(Radar);
    panelSettings_.push_back(fireLocked);
    panelSettings_.push_back(Locked);
    panelSettings_.push_back(Evade);
}

void GT_HUD::draw(GT_Camera *tempCam)
{
    primitiveShader_->Use();
    radar_->draw(tempCam);

    for (int i = 0; i < panelSettings_.size(); i++)
    {

        panel_.defineImageScreenCoordinates(panelSettings_[i].topLeft.x,
                                            panelSettings_[i].topLeft.y,
                                            panelSettings_[i].bottomRight.x,
                                            panelSettings_[i].bottomRight.y);
        panel_.Draw();
    }


}

void GT_HUD::installRadar()
{
    radar_ = new GT_Radar();
}
