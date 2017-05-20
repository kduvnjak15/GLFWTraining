#pragma once

#include "GT_Model.h"
#include <glm/glm.hpp>
#include "GT_Camera.h"

enum MODEL_TYPE
{
    F18,
    F22,
    AIM,
    USS
};

class GT_Actor
{
public:

    GT_Actor(GT_Model *actorModel);
    ~GT_Actor();

    virtual void Draw(GT_Camera* tempCam);

    void setPosition(glm::vec3 pos_);
    void setFront(glm::vec3 frontVec_);
    void setUp(glm::vec3 up_);
    void setRight(glm::vec3 right_);

protected:

    GT_Model* actorModel_;
    GT_Shader* actorShader_;

    glm::vec3 position_;
    glm::vec3 front_;
    glm::vec3 up_;
    glm::vec3 right_;

    GLuint modelLoc_;
    GLuint viewLoc_;
    GLuint projLoc_;
    GLuint lightPosLoc_;
    GLuint viewPosLoc_;


};

