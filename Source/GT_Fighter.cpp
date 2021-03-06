#include "GT_Fighter.h"
#include "GT_Locator.h"

GT_Fighter::GT_Fighter()
    :
      GT_Aircraft(GT_Locator::getModel(F18)),
       GT_Weapon(20, this),
       evade_(0),
       isTarget_(0)
{
    this->position_ = glm::vec3(170.0f, 58.0f, -32.0f);
    this->front_    = glm::vec3(0.98f, 0.0f, 0.07f);
    this->right_    = glm::vec3(0.0f, 0.0f, 1.0f);
    this->up_       = glm::vec3(0.0f, 1.0f, 0.0f);

    GT_Weapon::Integrate(0.0f);

    std::cout << "GT_Fighter initialized " << std::endl;

}

void GT_Fighter::Draw(GT_Camera *tempCam)
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
    glUniform1f(glGetUniformLocation(actorShader_->shaderProgram_, "time"), glfwGetTime() - explodeTime_);

    glm::mat4 model = glm::mat4(1.0f);

    glm::mat4 sc  = glm::scale(glm::mat4(1.0f), glm::vec3(scaleActor_));
    glm::mat4 rot = glm::rotate(glm::mat4(1.0f), (GLfloat)-3.14159/2.0f, glm::vec3(1.0f, 0.0f, 0.0f));

    glm::mat4 yawRot   = glm::rotate(glm::mat4(1.0f), (GLfloat) 3.14159/48.0f * tempCam->yawDelay_,   this->up_ );
    glm::mat4 rollRot  = glm::rotate(glm::mat4(1.0f), (GLfloat) 3.14159/48.0f * tempCam->rollDelay_,  this->front_ );
    glm::mat4 pitchRot = glm::rotate(glm::mat4(1.0f), (GLfloat) 3.14159/48.0f * tempCam->pitchDelay_, tempCam->getCameraRight() );

    glm::mat4 tr  = glm::translate(glm::mat4(1.0f), position_ );

    glm::mat4 planeOrient = glm::lookAt(glm::vec3(0.0f), this->front_, this->up_);

    glm::mat4 view = tempCam->GetViewMatrix();

    glm::mat4 projection = glm::perspective(ZOOM, (tempCam->getWindowWidth()*1.0f)/tempCam->getWindowHeight(), 0.1f, horizon);

    model =  tr * yawRot * rollRot * pitchRot * glm::transpose(planeOrient) * rot * sc;

    glUniformMatrix4fv(modelLoc_, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc_,  1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc_,  1, GL_FALSE, glm::value_ptr(projection));

    actorModel_->Draw(*actorShader_);

    GT_Weapon::Draw(tempCam);
}

void GT_Fighter::Integrate(GT_Camera *tempCam, GLfloat DT_)
{



    this->setPosition(tempCam->getCameraPos());
    GT_Aircraft::Integrate(tempCam, DT_);
    GT_Weapon::Integrate(DT_);

}

void GT_Fighter::isTarget(bool flag)
{
    if (flag)
        isTarget_++;
    else
        isTarget_--;
    if (isTarget_ < 0)
        isTarget_ = 0;
}

void GT_Fighter::evade(bool flag)
{
    if (flag)
        evade_++;
    else
        evade_--;
    if (evade_ < 0)
        evade_ = 0;
}


GT_Fighter::~GT_Fighter()
{
    std::cout << "fighter deleted " << std::endl;
}
