#pragma once


#include <GT_Actor.h>
#include <glm/glm.hpp>

enum AIRCRAFT
{
    F18,
    F22,
    AIM,
    USS
};

class GT_Aircraft : public GT_Actor
{
public:

    GT_Aircraft(GT_Model* aircraftModel, GT_Model* missileModel);
    ~GT_Aircraft();

    void Draw(GT_Camera *tempCam);
    void Integrate(GLfloat DT_);

    void loadMissiles(GT_Model* missileModel);

private:

    GLuint modelLoc_;
    GLuint viewLoc_;
    GLuint projLoc_;
    GLuint lightPosLoc_;
    GLuint viewPosLoc_;

    GLfloat speed_;

    GLuint numOfMissiles_;
    GT_Model* missileModel_;
};
