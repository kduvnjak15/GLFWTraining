#pragma once


#include "GT_Shader.h"
#include "GT_Model.h"

#include "GT_Camera.h"

class GT_Enemy : public GT_Model
{
public:

    GT_Enemy(const char* modelPath);
    ~GT_Enemy();

    bool isHit();
    void explode();

    GT_Shader* getShader() { return enemyShader_; }

    GLuint hitLoc_;
    GLuint timeLoc_;

    virtual void Draw(GT_Camera* tempCam) = 0;

protected:
    GT_Shader* enemyShader_;
    bool gotHit_;
    GLfloat hitTime_;
    GLfloat explosionTime() { return glfwGetTime() - hitTime_ ;}


private:
    void defineEnemyShader();


};
