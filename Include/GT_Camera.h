#pragma once

#include <iostream>
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define CAMERA_SPEED 0.05f;

enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    PITCH_D,
    PITCH_U,
    YAW_L,
    YAW_R,
    ROLL_R,
    ROLL_L,
    ACCELERATE,
    DECELERATE
};

// Default camera values
const GLfloat YAW        = -90.0f;
const GLfloat PITCH      =  0.0f;
const GLfloat SPEED      =  0.0050f;
const GLfloat SENSITIVITY =  0.25f;
const GLfloat ZOOM       =  45.0f;
const GLfloat rollSensitivity = .5;

class GT_Camera

{
public:
    GT_Camera(); // c-tor

    glm::vec3 getCameraPos()        { return cameraPos; }
    glm::vec3 getCameraFront()      { return cameraFront; }
    glm::vec3 getCameraUp()         { return cameraUp; }
    glm::vec3 getCameraRight()      { return cameraRight; }
    glm::vec3 getCameraWorldUp()    { return cameraWorldUp; }
    //Euler angles
    GLfloat getYaw()                { return yaw; }
    GLfloat getPitch()              { return pitch; }
    //Camera options
    GLfloat getMovementSpeed()      { return MovementSpeed; }
    GLfloat getMouseSensitivity()   { return MouseSensitivity; }
    GLfloat getZoom()               { return Zoom; }
    GLfloat getSpeed()              { return speed_; }

    glm::mat4 GetViewMatrix();

    void keyboardHandler(Camera_Movement direction, GLfloat deltaTime);
    void mouseHandler(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch = false );

    void pitchControl(Camera_Movement controlDirection, GLfloat deltaSpace);
    void rollControl(Camera_Movement controlDirection, GLfloat deltaSpace);
    void yawControl(Camera_Movement controlDirection, GLfloat deltaSpace);

private:
    //Camera Cartesian
    glm::vec3 cameraPos;
    glm::vec3 cameraFront;
    glm::vec3 cameraUp;
    glm::vec3 cameraRight;
    glm::vec3 cameraWorldUp;
    //Euler angles
    GLfloat yaw;
    GLfloat pitch;
    //Camera options
    GLfloat MovementSpeed;
    GLfloat MouseSensitivity;
    GLfloat Zoom;

    // rotation constants
    GLfloat a, b, c;
    GLfloat x, y, z;
    GLfloat u, v, w;
    GLfloat th;
    GLfloat L;


    GLfloat speed_;

    glm::vec3 getRotatedVector();


};
