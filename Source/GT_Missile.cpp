#include "GT_Missile.h"

GT_Missile::GT_Missile(GT_Model *missileModel)
    :
      missileModel_(missileModel)

{
    missileShader_ = new GT_Shader(enemyShader, "../Shaders/modelShader.vs", "../Shaders/modelShader.fs");
    std::cout << "GT_Missile initialized "<< this << std::endl;
}

GT_Missile::~GT_Missile()
{

}

void GT_Missile::setPosition(const glm::vec3 pos_)
{

    position_ = pos_;
}

void GT_Missile::Draw(const GT_Camera *tempCam)
{
    missileModel_->Draw(*missileShader_);
}

void GT_Missile::Integrate(const GLfloat DX_)
{

}
