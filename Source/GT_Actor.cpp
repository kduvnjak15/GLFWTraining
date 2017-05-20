#include "GT_Actor.h"


GT_Actor::GT_Actor(GT_Model* actorModel)
    :
      actorModel_(actorModel),
      actorShader_(new GT_Shader(aircraftShader, "../Shaders/actorShader.vs", "../Shaders/actorShader.fs"))
{

}

void GT_Actor::setPosition(glm::vec3 pos_)
{
    position_ = pos_;
}

void GT_Actor::setFront(glm::vec3 frontVec_)
{
    front_ = frontVec_;
}

void GT_Actor::setUp(glm::vec3 up)
{
    up_ = up;
}

void GT_Actor::setRight(glm::vec3 right)
{
    right_ = right;
}

void GT_Actor::Draw(GT_Camera *tempCam)
{
    if (!actorModel_ || !actorShader_)
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

    glm::mat4 sc  = glm::scale(glm::mat4(1.0f), glm::vec3(100.0f));
    glm::mat4 rot = glm::rotate(glm::mat4(1.0f), (GLfloat)-3.14159/2.0f, glm::vec3(1.0f, 0.0f, 0.0f));
//    glm::mat4 yawRot   = glm::rotate(glm::mat4(1.0f), (GLfloat) 3.14159/72.0f * tempCam->yawDelay_,   glm::vec3(0.0f, 1.0f, 0.0f) );
//    glm::mat4 rollRot  = glm::rotate(glm::mat4(1.0f), (GLfloat)-3.14159/72.0f * tempCam->rollDelay_,  glm::vec3(0.0f, 0.0f, 1.0f) );
//    glm::mat4 pitchRot = glm::rotate(glm::mat4(1.0f), (GLfloat) 3.14159/72.0f * tempCam->pitchDelay_, glm::vec3(1.0f, 0.0f, 0.0f) );

    glm::mat4 yawRot   = glm::rotate(glm::mat4(1.0f), (GLfloat) 3.14159/72.0f * tempCam->yawDelay_,   this->up_ );
    glm::mat4 rollRot  = glm::rotate(glm::mat4(1.0f), (GLfloat)-3.14159/72.0f * tempCam->rollDelay_,  this->front_ );
    glm::mat4 pitchRot = glm::rotate(glm::mat4(1.0f), (GLfloat) 3.14159/72.0f * tempCam->pitchDelay_, tempCam->getCameraRight() );

  //  glm::mat4 tr  = glm::translate(glm::mat4(1.0f), position_ + glm::vec3(0.0f, -5.0f, -tempCam->getSpeedOffset()));
      glm::mat4 tr  = glm::translate(glm::mat4(1.0f), position_ );

      glm::mat4 planeOrient = glm::lookAt(glm::vec3(0.0f), this->front_, this->up_);

    glm::mat4 view = tempCam->GetViewMatrix();
  //  glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::perspective(ZOOM, (window_width*1.0f)/window_height, 0.1f, horizon);

    model =  tr * yawRot * rollRot * pitchRot * glm::inverse(planeOrient) * rot * sc;

    glUniformMatrix4fv(modelLoc_, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc_,  1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc_,  1, GL_FALSE, glm::value_ptr(projection));

    actorModel_->Draw(*actorShader_);

}

GT_Actor::~GT_Actor()
{

}



