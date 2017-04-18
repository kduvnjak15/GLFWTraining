#include "GT_Enemy.h"
#include "GLFW/glfw3.h"

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

void GT_Enemy::falloutMove()
{
     if (isHit())
     {
         std::cout << "muvam" << this <<std::endl;
         modelPos += glm::vec3(0.0f, -.1f * glfwGetTime()*glfwGetTime(), 0.0f );
     }
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


