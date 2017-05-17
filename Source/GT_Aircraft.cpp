#include <iostream>

#include "GT_Aircraft.h"

GT_Aircraft::GT_Aircraft(GT_Model *aircraftModel, GT_Model* missileModel)
    : GT_Actor(),
      speed_(1),
      numOfMissiles_(10),
      missileModel_(missileModel)
{
    actorModel_  = aircraftModel;
    actorShader_ = new GT_Shader(aircraftShader, "../Shaders/aircraftShader.vs", "../Shaders/aircraftShader.fs");


    std::cout << "GT_Aircraft initialized " << this << std::endl;
}

void GT_Aircraft::Draw(GT_Camera* tempCam)
{

    if (!actorModel_)
    {
        std::cout << "actorModel non existing " << std::endl;
        return;
    }

    this->actorShader_->Use();
    modelLoc_  = glGetUniformLocation(actorShader_->shaderProgram_, "model");
    viewLoc_   = glGetUniformLocation(actorShader_->shaderProgram_, "view");
    projLoc_   = glGetUniformLocation(actorShader_->shaderProgram_, "projection");
    glUniform1i(glGetUniformLocation(actorShader_->shaderProgram_, "material.diffuse"),  0);
    glUniform1i(glGetUniformLocation(actorShader_->shaderProgram_, "material.specular"), 1);
    lightPosLoc_    = glGetUniformLocation(actorShader_->shaderProgram_, "light.position");
    viewPosLoc_     = glGetUniformLocation(actorShader_->shaderProgram_, "viewPos");
    glUniform3f(lightPosLoc_,    lightPos.x, lightPos.y, lightPos.z);
    glUniform3f(viewPosLoc_,     tempCam->getCameraPos().x, tempCam->getCameraPos().y, tempCam->getCameraPos().z);
    // Set lights properties
    glUniform3f(glGetUniformLocation(actorShader_->shaderProgram_, "light.ambient"),  0.50, 0.50f, 0.50f);
    glUniform3f(glGetUniformLocation(actorShader_->shaderProgram_, "light.diffuse"),  0.80, 0.80f, 0.80f);
    glUniform3f(glGetUniformLocation(actorShader_->shaderProgram_, "light.specular"), 0.05, 0.05f, 0.05f);
    // Set material properties
    glUniform1f(glGetUniformLocation(actorShader_->shaderProgram_, "material.shininess"), 1.0f);
    glm::mat4 model = glm::mat4(1.0f);

    glm::mat4 sc  = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f));
    glm::mat4 rot = glm::rotate(glm::mat4(1.0f), (GLfloat)-3.14159/2.0f, glm::vec3(1.0f, 0.0f, 0.0f));
    glm::mat4 yawRot   = glm::rotate(glm::mat4(1.0f), (GLfloat) 3.14159/72.0f * tempCam->yawDelay_,   glm::vec3(0.0f, 1.0f, 0.0f) );
    glm::mat4 rollRot  = glm::rotate(glm::mat4(1.0f), (GLfloat)-3.14159/72.0f * tempCam->rollDelay_,  glm::vec3(0.0f, 0.0f, 1.0f) );
    glm::mat4 pitchRot = glm::rotate(glm::mat4(1.0f), (GLfloat) 3.14159/72.0f * tempCam->pitchDelay_, glm::vec3(1.0f, 0.0f, 0.0f) );

    glm::mat4 tr  = glm::translate(glm::mat4(1.0f),  glm::vec3(0.0f, -5.0f, -tempCam->getSpeedOffset()));

    model =  tr * yawRot * rollRot * pitchRot * rot * sc;

    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = tempCam->GetProjectionMatrix();
    glUniformMatrix4fv(modelLoc_, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc_,  1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc_,  1, GL_FALSE, glm::value_ptr(projection));

    actorModel_->Draw(*actorShader_);

    std::cout << tempCam->getCameraFront().x << std::endl;
}

void GT_Aircraft::Integrate(GLfloat DT_)
{
    DT_ *=speed_;
    position_ += front_*DT_;
}

GT_Aircraft::~GT_Aircraft()
{

    delete actorShader_;

    std::cout << "GT_Aircraft dtor" << this << std::endl;
}

