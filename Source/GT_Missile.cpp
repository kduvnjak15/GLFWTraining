#include "GT_Missile.h"

#include "GT_Aircraft.h"

#include "GT_Locator.h"

#include "GT_Enemy.h"

GT_Missile::GT_Missile(GT_Model *missileModel, GT_Aircraft* ownerPtr, GLuint missileIndex)
    :
      GT_Actor(missileModel),
      ownerPtr_(ownerPtr),
      missileIndex_(missileIndex),
      contrailRate_(10),
      fired_(false),
      birthday_(0.0f),
      dead_(false)
{

    if ( missileIndex_ % 2 == 0)
        missileOffset_ = glm::vec3(-2.35f, 0.4f, -1.7f);
    else
        missileOffset_ = glm::vec3( 3.65f, 0.4f, -1.7f);

    position_ = ownerPtr_->getPosition() + missileOffset_;
    scaleActor_ = 0.1f;

    particle_ = GT_Locator::getParticle();

    std::cout << "GT_Missile initialized "<< this << std::endl;
}

GT_Missile::~GT_Missile()
{

}

void GT_Missile::Draw(GT_Camera *tempCam)
{
    if (dead_ )
    {
        return;
    }

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
    glm::mat4 model = glm::mat4(1.0f);

    glm::mat4 sc  = glm::scale(glm::mat4(1.0f), glm::vec3(scaleActor_));
    glm::mat4 rot = glm::rotate(glm::mat4(1.0f), (GLfloat)-3.14159/2.0f, glm::vec3(0.0f, 1.0f, 0.0f));

    glm::mat4 yawRot   = glm::rotate(glm::mat4(1.0f), (GLfloat) 3.14159/72.0f * tempCam->yawDelay_,   this->up_ );
    glm::mat4 rollRot  = glm::rotate(glm::mat4(1.0f), (GLfloat)-3.14159/72.0f * tempCam->rollDelay_,  this->front_ );
    glm::mat4 pitchRot = glm::rotate(glm::mat4(1.0f), (GLfloat) 3.14159/72.0f * tempCam->pitchDelay_, tempCam->getCameraRight() );

    glm::mat4 tr  = glm::translate(glm::mat4(1.0f), position_);

    glm::mat4 planeOrient = glm::lookAt(glm::vec3(0.0f), this->front_, this->up_);

    glm::mat4 view = tempCam->GetViewMatrix();
    glm::mat4 projection = glm::perspective(ZOOM, (window_width*1.0f)/window_height, 0.1f, horizon);

    model =  tr * yawRot * rollRot * pitchRot * glm::inverse(planeOrient) * rot * sc;

    glUniformMatrix4fv(modelLoc_, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc_,  1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc_,  1, GL_FALSE, glm::value_ptr(projection));

    glActiveTexture(GL_TEXTURE0);

    actorModel_->Draw(*actorShader_);

    particle_->primitiveShader_->Use();
    for (auto cit = contrail_.begin(); cit!= contrail_.end(); cit++)
    {
        tr  = glm::translate(glm::mat4(1.0f), (*cit) - front_ * 2.5f);
        sc  = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f));
        rot = glm::rotate(glm::mat4(1.0f), (GLfloat)-3.14159/2.0f, glm::vec3(1.0f, 0.0f, 0.0f));

        model =  tr * yawRot * rollRot * pitchRot * glm::inverse(planeOrient) * rot * sc;

        glUniformMatrix4fv(particle_->modelLoc_, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(particle_->viewLoc_,  1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(particle_->projLoc_,  1, GL_FALSE, glm::value_ptr(projection));
        particle_->draw();
    }



}

void GT_Missile::FIRE(GT_Aircraft* target)
{
    fired_ = true;
    birthday_ = glfwGetTime();

    if (dynamic_cast<GT_Fighter*>(target))
    {
        std::cout << "so true" << std::endl;
        ((GT_Fighter*)target)->evade(true);
    }

    if (dynamic_cast<GT_USSCarrier*>(target))
    {
        std::cout << "FUCK ME" << std::endl;
        dynamic_cast<GT_USSCarrier*>(target)->explode();
    }

    enemyTarget_ = target;

    std::cout << "FUCK me "<< std::endl;
}

void GT_Missile::aimEnemy()
{
    if (enemyTarget_ == nullptr || enemyTarget_->explode_)
        return ;

    glm::vec3 dir = enemyTarget_->getPosition() - this->position_;
    glm::vec3 frontDir = this->front_;

    dir = glm::normalize(dir);
    frontDir = glm::normalize(frontDir);

    GLfloat dotpr  =glm::dot(dir, frontDir);
    while (dotpr<0.85)
    {
        dir     = glm::normalize(dir + frontDir);
        dotpr   = glm::dot(dir, frontDir);
    }

    this->up_ = glm::normalize( glm::cross(this->front_, dir) );
    this->front_ = dir;//buggger
    this->right_ = glm::normalize(glm::cross(this->front_, this->up_));
}

void GT_Missile::Integrate(GLfloat DX_)
{

    if (!fired_)
    {
        position_   = ((GT_Aircraft*)ownerPtr_)->getPosition();
        this->front_      = ((GT_Aircraft*)ownerPtr_)->getFront();
        this->up_         = ((GT_Aircraft*)ownerPtr_)->getUp();
        this->right_      = ((GT_Aircraft*)ownerPtr_)->getRight();

        position_ += this->right_ * missileOffset_.x;
        position_ += this->up_    * missileOffset_.y;
        position_ += this->front_ * missileOffset_.z;
    }
    else
    {
        if (glfwGetTime() -  birthday_ > MISSILE_LIFE )
        {
            dead_ = true;
            if (dynamic_cast<GT_Fighter*>(enemyTarget_))
                ((GT_Fighter*)enemyTarget_)->evade(false);
            return;
        }

        glm::vec3 dir = enemyTarget_->getPosition() - this->position_;

        if (glm::length(dir) < 10)
        {
            dead_ = true;
            enemyTarget_->explode();
        }

        aimEnemy();

        position_ += this->front_ * DX_ * MISSILE_SPEED;
        contrailRate_++;
        if (contrailRate_ > 10 )
        {
            contrailRate_ = 0;

            contrail_.push_front(position_);
            if (contrail_.size() > 50)
                contrail_.pop_back();
        }


    }
}
