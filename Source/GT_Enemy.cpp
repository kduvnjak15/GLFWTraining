#include "GT_Enemy.h"


GT_Enemy::GT_Enemy(const char* modelPath, GT_Shader *shaderPtr)
    : GT_Model(modelPath),
      gotHit_(false)
{
    if (shaderPtr->getShaderTag() != shaderTag::enemyShader )
    {
        std::cout<< "CATASTROPHIC ERROR; shader not 'enemyShader'"<<std::endl;
        return;
    }

    std::cout<< "Enemy initialized" << this<<std::endl;

    enemyShader_ = shaderPtr;

}

GT_Enemy::~GT_Enemy()
{
    delete enemyShader_;
}

bool GT_Enemy::isHit()
{
    return gotHit_;
}

void GT_Enemy::explode()
{
    gotHit_ = true;
    glUniform1i(glGetUniformLocation(enemyShader_->shaderProgram_, "isHit"), 10);
    std::cout<<"EXPLOOODEEE!!"<<this<<std::endl;
    hitTime_ = glfwGetTime();
}
