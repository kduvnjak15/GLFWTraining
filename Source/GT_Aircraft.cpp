#include <iostream>

#include "GT_Aircraft.h"

GT_Aircraft::GT_Aircraft(GT_Model *aircraftModel, GT_Model* missileModel)
    :
      GT_Aircraft(aircraftModel, missileModel, 2)
{

}

GT_Aircraft::GT_Aircraft(GT_Model *aircraftModel, GT_Model* missileModel, GLuint missileCount)
    :
      GT_Actor(aircraftModel),
      numOfMissiles_(missileCount),
      missileModel_(missileModel),
      aircraftSpeed_(10.0f),
      explode_(0)

{
    position_    = glm::vec3(0.0f);
    actorModel_  = aircraftModel;
    actorShader_ = new GT_Shader(aircraftShader, "../Shaders/aircraftShader.vs", "../Shaders/aircraftShader.fs", "../Shaders/aircraftShader.gs");
    modelLoc_ = glGetUniformLocation(actorShader_->shaderProgram_, "model");
    viewLoc_ = glGetUniformLocation(actorShader_->shaderProgram_, "view");
    projLoc_  = glGetUniformLocation(actorShader_->shaderProgram_, "projection");
    lightPosLoc_ = glGetUniformLocation(actorShader_->shaderProgram_, "light.position");
    viewPosLoc_ = glGetUniformLocation(actorShader_->shaderProgram_, "viewPos");
    explodeLoc_ = glGetUniformLocation(actorShader_->shaderProgram_, "isHit");

    for (int i = 0 ; i < numOfMissiles_; i++)
    {
        missiles_.push_back(new GT_Missile(missileModel_, (void*)this, i));
    }

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
    glUniform1i(glGetUniformLocation(actorShader_->shaderProgram_, "material.diffuse"),  0);
    glUniform1i(glGetUniformLocation(actorShader_->shaderProgram_, "material.specular"), 1);

    glUniform3f(lightPosLoc_,    lightPos.x, lightPos.y, lightPos.z);
    glUniform3f(viewPosLoc_,     tempCam->getCameraPos().x, tempCam->getCameraPos().y, tempCam->getCameraPos().z);
    // Set lights properties
    glUniform3f(glGetUniformLocation(actorShader_->shaderProgram_, "light.ambient"),  0.50, 0.50f, 0.50f);
    glUniform3f(glGetUniformLocation(actorShader_->shaderProgram_, "light.diffuse"),  0.80, 0.80f, 0.80f);
    glUniform3f(glGetUniformLocation(actorShader_->shaderProgram_, "light.specular"), 0.1, 0.1f, 0.1f);
    // Set material properties
    glUniform1f(glGetUniformLocation(actorShader_->shaderProgram_, "material.shininess"), 1.0f);

    glUniform1i(explodeLoc_, explode_);
    glUniform1f(glGetUniformLocation(actorShader_->shaderProgram_, "time"), glfwGetTime());

    glm::mat4 model = glm::mat4(1.0f);

    glm::mat4 sc  = glm::scale(glm::mat4(1.0f), glm::vec3(scaleActor_));
    glm::mat4 rot = glm::rotate(glm::mat4(1.0f), (GLfloat)-3.14159/2.0f, glm::vec3(1.0f, 0.0f, 0.0f));

    glm::mat4 tr  = glm::translate(glm::mat4(1.0f), position_ );

    glm::mat4 planeOrient = glm::lookAt(glm::vec3(0.0f), this->front_, this->up_);

    glm::mat4 view = tempCam->GetViewMatrix();
    glm::mat4 projection = glm::perspective(ZOOM, (window_width*1.0f)/window_height, 0.1f, horizon);

    model =  tr * glm::transpose(planeOrient) * rot * sc;

    glUniformMatrix4fv(modelLoc_, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc_,  1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc_,  1, GL_FALSE, glm::value_ptr(projection));

    actorModel_->Draw(*actorShader_);

    int br = 0;
    for (auto mit = missiles_.begin(); mit != missiles_.end() ; mit++)
    {
        (*mit)->Draw(tempCam);
        if (!(*mit)->isFired())
            br++;

        if (br>1)
            break;
    }    
}

void GT_Aircraft::Integrate(GT_Camera* tempCam, GLfloat DT_)
{

    this->front_    = tempCam->getCameraFront();
    this->up_       = tempCam->getCameraUp();
    this->right_    = tempCam->getCameraRight();

    //movement

    for (auto mit = missiles_.begin(); mit != missiles_.end(); mit++)
    {
        (*mit)->Integrate(tempCam, DT_);
    }

}

void GT_Aircraft::fireMissile()
{

    for (int i = 0; i < missiles_.size(); i++)
        if (missiles_[i]->isFired())
            continue;
        else
        {
            missiles_[i]->FIRE();
            break;
        }
}

GT_Aircraft::~GT_Aircraft()
{

    delete actorShader_;

    std::cout << "GT_Aircraft dtor" << this << std::endl;
}

