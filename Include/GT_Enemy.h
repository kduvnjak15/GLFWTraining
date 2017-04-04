#pragma once


#include "GT_Shader.h"
#include "GT_Model.h"

class GT_Enemy : public GT_Model
{
public:

    GT_Enemy(const char* modelPath, GT_Shader* shaderPtr);
    ~GT_Enemy();

    bool isHit();
    void explode();

    GT_Shader* getShader() { return enemyShader_; }

private:
    GT_Shader* enemyShader_;
    bool gotHit_;
};
