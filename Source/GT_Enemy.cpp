#include "GT_Enemy.h"


GT_Enemy::GT_Enemy(const char* modelPath)
    : GT_Model(modelPath), gotHit_(false)
{

    defineEnemyShader();

    if (enemyShader_->getShaderTag() != shaderTag::enemyShader )
    {
        std::cout<< "CATASTROPHIC ERROR; shader not 'enemyShader'"<<std::endl; return;
    }

    std::cout<< "Enemy initialized" << this<<std::endl;
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

    std::cout<<"EXPLOOODEEE!!"<<this<<std::endl;
    hitTime_ = glfwGetTime();
}


void GT_Enemy::defineEnemyShader()
{
    enemyShader_ = new GT_Shader(enemyShader, "../Shaders/enemyShader.vs", "../Shaders/enemyShader.fs", "../Shaders/enemyShader.gs");
    modelLoc_    =  glGetUniformLocation(enemyShader_->shaderProgram_, "model");
    viewLoc_     = glGetUniformLocation(enemyShader_->shaderProgram_, "view");
    projLoc_     = glGetUniformLocation(enemyShader_->shaderProgram_, "projection");
    hitLoc_       = glGetUniformLocation(enemyShader_->shaderProgram_, "isHit");
    timeLoc_     = glGetUniformLocation(enemyShader_->shaderProgram_, "time");
}

void GT_Enemy::Draw(GT_Camera *tempCam)
{



}
