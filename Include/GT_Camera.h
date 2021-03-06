#pragma once

#include <iostream>
#include <set>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

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
    DECELERATE,
    KEY_PRESSED,
    KEY_RELEASED
};

// Default camera values
const GLfloat YAW        = -90.0f;
const GLfloat PITCH      =  0.0f;
const GLfloat SPEED      =  150.0f;
const GLfloat SENSITIVITY =  0.25f;
const GLfloat ZOOM       =  45.0f;
const GLfloat ROLL_SENSITIVITY  = 1.85;
const GLfloat YAW_SENSITIVITY   = 1.85;
const GLfloat PITCH_SENSITIVITY = 1.85;
const GLfloat CAMERA_OFFSET = 20;
const GLfloat horizon = 50000;
const GLfloat MAX_SPEED = 600.0f;

static glm::vec3 lightPos(-42000.0f, 54800.0f, -71700.0f);


class GT_Camera

{
public:
    GT_Camera(); // c-tor

    void setCameraPos(glm::vec3 camPosition)    { cameraPos     = camPosition; }
    void setCameraFront(glm::vec3 camFront)     { cameraFront   = camFront; }
    void setCameraRight(glm::vec3 camRight)     { cameraRight   = camRight; }
    void setCameraUp(glm::vec3 camUp)           { cameraUp      = camUp;    }
    void setSpeed(GLfloat speed)             { speed_      = speed;    }

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
    GLfloat getSpeedOffset();

    GLfloat getKeyDelay()           { return keyDelay_; }
    GLfloat rollDelay_;
    GLfloat yawDelay_;
    GLfloat pitchDelay_;

    glm::mat4 GetViewMatrix();
    glm::mat4 GetProjectionMatrix() {return glm::perspective(ZOOM, (window_width*1.0f)/window_height, 0.1f, horizon);}

    void keyboardHandler(Camera_Movement direction, GLfloat deltaTime);
    void keyboardHandler(std::set<int> keysPressed, GLfloat deltaTime);
    void mouseHandler(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch = false );

    void pitchControl(Camera_Movement controlDirection, GLfloat deltaSpace);
    void rollControl(Camera_Movement controlDirection, GLfloat deltaSpace);
    void yawControl(Camera_Movement controlDirection, GLfloat deltaSpace);

    void bounceBBox() {cameraFront = - cameraFront; cameraRight = -cameraRight; }
    void enforceGravity(GLfloat factor) { cameraPos += glm::vec3(0.0f, -1.0f, 0.0f)*factor;}

    static GT_Camera* getCamera() { return cameraInstance_; }

    GLuint getWindowWidth() { return window_width; }
    GLuint getWindowHeight() { return window_height; }

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
    GLfloat speedOffset_;

    GLfloat keyDelay_;


    glm::vec3 getRotatedVector();

    static GT_Camera* cameraInstance_;

    const GLuint window_width   = glfwGetVideoMode(glfwGetPrimaryMonitor())->width;
    const GLuint window_height  = glfwGetVideoMode(glfwGetPrimaryMonitor())->height;

};
