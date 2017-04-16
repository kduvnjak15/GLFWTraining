#include "GT_HUD.h"

#include "SOIL/SOIL.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

GT_HUD::GT_HUD()
    : GT_Primitive()
{
    sideA_ = 1.0f;
    sideB_ = sideA_;

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

    // Info
    panel Info;
    Info.position_  = glm::vec3(-0.70f, -0.35f, -1.0f);
    Info.volume_    = glm::vec3(.22f, .2f, 1.0f);

    // Radar
    panel Radar;
    Radar.position_ = glm::vec3(0.7f, -0.0f, -1.0f);
    Radar.volume_   = glm::vec3(.22f, .4f, 1.0f);

    // Mission objectives
    panel Objectives;
    Objectives.position_ = glm::vec3(0.7f, 0.4f, -1.0f);
    Objectives.volume_   = glm::vec3(.2f, .2f, 1.0f);

    panelSettings_.push_back(Info);
    panelSettings_.push_back(Radar);
    panelSettings_.push_back(Objectives);
}

void GT_HUD::draw(GT_Camera *tempCam)
{
    primitiveShader_->Use();
    for (int i = 0; i < panelSettings_.size(); i++)
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, panelSettings_[i].position_ );
        model = glm::scale(model, panelSettings_[i].volume_);
        model = glm::rotate(model, (GLfloat)-3.14159/2.0f, glm::vec3(1.0f, 0.0f, 0.0f));

        glm::mat4 view = tempCam->GetViewMatrix();
        model = glm::inverse(view) * model;
        glm::mat4 projection = tempCam->GetProjectionMatrix();
        glUniformMatrix4fv(modelLoc_, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc_, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc_, 1, GL_FALSE, glm::value_ptr(projection));

        glBindVertexArray(VAO_);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (const GLvoid*)0);

        glBindVertexArray(0);
    }

    radar_->draw(tempCam);

}

void GT_HUD::installRadar()
{
    radar_ = new GT_Radar();
    radar_->setScreenPos(panelSettings_[1].position_);
}
