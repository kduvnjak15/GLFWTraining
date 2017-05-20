#include "GT_Missile.h"

#include "GT_Aircraft.h"

GT_Missile::GT_Missile(GT_Model *missileModel, void* ownerPtr, GLuint missileIndex)
    :
      GT_Actor(missileModel),
      ownerPtr_(ownerPtr),
      missileIndex_(missileIndex),
      fired_(false)

{
    actorShader_ = new GT_Shader(enemyShader, "../Shaders/actorShader.vs", "../Shaders/actorShader.fs");
    position_ = ((GT_Aircraft*)ownerPtr_)->getPosition();

    if ( missileIndex_ % 2 == 0)
        missileOffset_ = glm::vec3(-2.0f, 0.0f, -1.0f);
    else
        missileOffset_ = glm::vec3( 3.0f, 0.0f, -1.0f);

    std::cout << "GT_Missile initialized "<< this << std::endl;
}

GT_Missile::~GT_Missile()
{

}

void GT_Missile::Draw(GT_Camera *tempCam)
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
    glUniform3f(glGetUniformLocation(actorShader_->shaderProgram_, "light.specular"), 0.1, 0.1f, 0.1f);
    // Set material properties
    glUniform1f(glGetUniformLocation(actorShader_->shaderProgram_, "material.shininess"), 1.0f);
    glm::mat4 model = glm::mat4(1.0f);

    glm::mat4 sc  = glm::scale(glm::mat4(1.0f), glm::vec3(0.10f));
    glm::mat4 rot = glm::rotate(glm::mat4(1.0f), (GLfloat)-3.14159/2.0f, glm::vec3(0.0f, 1.0f, 0.0f));
//    glm::mat4 yawRot   = glm::rotate(glm::mat4(1.0f), (GLfloat) 3.14159/72.0f * tempCam->yawDelay_,   glm::vec3(0.0f, 1.0f, 0.0f) );
//    glm::mat4 rollRot  = glm::rotate(glm::mat4(1.0f), (GLfloat)-3.14159/72.0f * tempCam->rollDelay_,  glm::vec3(0.0f, 0.0f, 1.0f) );
//    glm::mat4 pitchRot = glm::rotate(glm::mat4(1.0f), (GLfloat) 3.14159/72.0f * tempCam->pitchDelay_, glm::vec3(1.0f, 0.0f, 0.0f) );

    glm::mat4 yawRot   = glm::rotate(glm::mat4(1.0f), (GLfloat) 3.14159/72.0f * tempCam->yawDelay_,   this->up_ );
    glm::mat4 rollRot  = glm::rotate(glm::mat4(1.0f), (GLfloat)-3.14159/72.0f * tempCam->rollDelay_,  this->front_ );
    glm::mat4 pitchRot = glm::rotate(glm::mat4(1.0f), (GLfloat) 3.14159/72.0f * tempCam->pitchDelay_, tempCam->getCameraRight() );

  //  glm::mat4 tr  = glm::translate(glm::mat4(1.0f), position_ + glm::vec3(0.0f, -5.0f, -tempCam->getSpeedOffset()));
    glm::mat4 tr  = glm::translate(glm::mat4(1.0f), position_);
    glm::mat4 tranX  = glm::translate(glm::mat4(1.0f), this->right_ * missileOffset_.x);
    glm::mat4 tranY  = glm::translate(glm::mat4(1.0f), this->up_    * missileOffset_.y);

    glm::mat4 planeOrient = glm::lookAt(glm::vec3(0.0f), this->front_, this->up_);

    glm::mat4 view = tempCam->GetViewMatrix();
  //  glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::perspective(ZOOM, (window_width*1.0f)/window_height, 0.1f, horizon);

    model =  tr * yawRot * rollRot * pitchRot * glm::inverse(planeOrient) * rot * sc;

    glUniformMatrix4fv(modelLoc_, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc_,  1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc_,  1, GL_FALSE, glm::value_ptr(projection));

    actorModel_->Draw(*actorShader_);

    std::cout << fired_ << std::endl;

}

void GT_Missile::FIRE()
{
    fired_ = true;
}


void GT_Missile::Integrate(GT_Camera* tempCam, GLfloat DX_)
{
    if (!fired_)
    {
        position_   = ((GT_Aircraft*)ownerPtr_)->getPosition();
        this->front_      = tempCam->getCameraFront();
        this->up_         = tempCam->getCameraUp();
        this->right_      = tempCam->getCameraRight();

        position_ += this->right_ * missileOffset_.x;
        position_ += this->up_    * missileOffset_.y;
        position_ += this->front_ * missileOffset_.z;
    }
    else
    {
        position_ += this->front_ * DX_ * MISSILE_SPEED;
    }
}
