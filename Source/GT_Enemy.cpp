#include "GT_Enemy.h"


GT_Enemy::GT_Enemy(const char* modelPath, GT_Shader *shaderPtr)
    : GT_Model(modelPath)
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
