#include "GT_Missile.h"

#include "GT_Aircraft.h"

GT_Missile::GT_Missile(GT_Model *missileModel, void* ownerPtr)
    :
      missileModel_(missileModel),
      ownerPtr_(ownerPtr)

{
    missileShader_ = new GT_Shader(enemyShader, "../Shaders/modelShader.vs", "../Shaders/modelShader.fs");
    std::cout << "GT_Missile initialized "<< this << std::endl;
}

GT_Missile::~GT_Missile()
{

}

void GT_Missile::setPosition(const glm::vec3 pos_)
{
    GT_Aircraft* ownerAircraft = (GT_Aircraft*)ownerPtr_;

    position_ = pos_;
}

void GT_Missile::Draw(const GT_Camera *tempCam)
{
    missileShader_->Use();
    missileModel_->Draw(*missileShader_);
}

void GT_Missile::Integrate(const GLfloat DX_)
{

}
