#include "GT_Camera.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

GT_Camera::GT_Camera()
    : cameraPos(0.0f, 400.0f, 0.0f),
      cameraFront(0.0f, 0.0f, -1.0f),
      cameraUp(0.0f, 1.0f, 0.0f),
      cameraWorldUp(0.0f, 1.0f, 0.0f),

      yaw(YAW),
      pitch(PITCH),
      MovementSpeed(SPEED),
      MouseSensitivity(SENSITIVITY),
      Zoom(ZOOM),
      speed_(SPEED),

      rollDelay_(0.0f),
      pitchDelay_(0.0f),
      yawDelay_(0.0f)

{
    cameraRight = glm::normalize(glm::cross(cameraFront, cameraUp));
}


glm::vec3 GT_Camera::getRotatedVector()
{
    glm::vec3 retVec;
    retVec.x = (a*(v*v+w*w) - u*(b*v+c*w-u*x-v*y-w*z))*(1-cos(th)) + L*x*cos(th)+ std::sqrt(L)*(-c*v+b*w-w*y+v*z)*sin(th);
    retVec.y = (b*(u*u+w*w) - v*(a*u+c*w-u*x-v*y-w*z))*(1-cos(th)) + L*y*cos(th)+ std::sqrt(L)*( c*u-a*w+w*x-u*z)*sin(th);
    retVec.z = (c*(u*u+v*v) - w*(a*u+b*v-u*x-v*y-w*z))*(1-cos(th)) + L*z*cos(th)+ std::sqrt(L)*(-b*u+a*v-v*x+u*y)*sin(th);
    glm::normalize(retVec);
    retVec = retVec * (1/L);

    return glm::normalize(retVec);

}

void GT_Camera::pitchControl(Camera_Movement controlDirection, GLfloat deltaSpace)
{
    a = 0; b = 0; c = 0;       // pivot point
    u = cameraRight.x; v = cameraRight.y; w = cameraRight.z; // axis
    x = cameraFront.x; y = cameraFront.y; z = cameraFront.z; // subject point

    th = deltaSpace;
    L = u*u + v*v + w*w;

    cameraFront = getRotatedVector();

    this->cameraUp    = glm::normalize(glm::cross(this->cameraRight, this->cameraFront));

}

void GT_Camera::rollControl(Camera_Movement controlDirection, GLfloat deltaSpace)
{
    a = 0.0; b = 0.0; c = 0.0;       // pivot point
    u = cameraFront.x; v = cameraFront.y; w = cameraFront.z; // axis
    x = cameraUp.x; y = cameraUp.y; z = cameraUp.z; // subject point

    th = deltaSpace;
    L = u*u + v*v + w*w;

    cameraUp = getRotatedVector();

    this->cameraRight = glm::normalize(glm::cross(this->cameraFront, this->cameraUp));

}
void GT_Camera::yawControl(Camera_Movement controlDirection, GLfloat deltaSpace)
{
    a = 0.0; b = 0.0; c = 0.0;       // pivot point
    u = cameraUp.x; v = cameraUp.y; w = cameraUp.z; // axis
    x = cameraRight.x; y = cameraRight.y; z = cameraRight.z; // subject point

    th = deltaSpace;
    L = u*u + v*v + w*w;

    cameraRight = getRotatedVector();

    this->cameraFront = glm::normalize(glm::cross(this->cameraUp, this->cameraRight));

}

void GT_Camera::keyboardHandler(std::set<int> keysPressed, GLfloat deltaTime)
{
    GLfloat deltaSpace = speed_ * deltaTime;
    GLfloat flightOffset = ROLL_SENSITIVITY * deltaTime ;

    /////////////////////////         pitch          /////////////////////////

    if (keysPressed.find(GLFW_KEY_W) != keysPressed.end())
    {
        pitchDelay_ -= 0.05f;
        if (pitchDelay_ < -2.0f) pitchDelay_ = -2.0f;
    }
    else if (keysPressed.find(GLFW_KEY_S) != keysPressed.end())
    {
        pitchDelay_ += 0.05f;
        if (pitchDelay_ > 2.0f) pitchDelay_ = 2.0f;
    }
    else
    {
        if (abs(pitchDelay_ )< 0.05f) pitchDelay_ = 0.0f;
        else pitchDelay_ += -pitchDelay_/abs(pitchDelay_)*0.05f;
    }
    /////////////////////////////////////////////////////////////////////////////


    ///////////////////////       roll             //////////////////////////////
    if (keysPressed.find(GLFW_KEY_A) != keysPressed.end())
    {
        rollDelay_ -= 0.08f;
        if (rollDelay_ < -2.0f) rollDelay_ = -2.0f;
    }
    else if (keysPressed.find(GLFW_KEY_D) != keysPressed.end())
    {
        rollDelay_ += 0.08f;
        if (rollDelay_  > 2.0f) rollDelay_ = 2.0f;
    }
    else
    {
        if (abs(rollDelay_ )< 0.08f) rollDelay_ = 0.0f;
        else rollDelay_ += -rollDelay_/abs(rollDelay_)*0.08f;
    }
    /////////////////////////////////////////////////////////////////////////////

    ///////////////////           YAW           ///////////////////////////////////

    if (keysPressed.find(GLFW_KEY_E) != keysPressed.end())
    {
        yawDelay_ -= 0.05f;
        if (yawDelay_ < -2.0f) yawDelay_ = -2.0f;
    }
    else if (keysPressed.find(GLFW_KEY_Q) != keysPressed.end())
    {
        yawDelay_ += 0.05f;
        if (yawDelay_ > 2.0f) yawDelay_ = 2.0f;
    }
    else
    {
        if (abs(yawDelay_ )< 0.05f) yawDelay_ = 0.0f;
        else yawDelay_ += -yawDelay_/abs(yawDelay_)*0.05f;
    }

    /////////////////////////////////////////////////////////////////////////////

    pitchControl(PITCH_D,   pitchDelay_ * flightOffset * PITCH_SENSITIVITY);
    yawControl(PITCH_D,     yawDelay_   * flightOffset * YAW_SENSITIVITY);
    rollControl(PITCH_D,    rollDelay_  * flightOffset * ROLL_SENSITIVITY);
}

void GT_Camera::keyboardHandler(Camera_Movement direction, GLfloat deltaTime)
{
    GLfloat deltaSpace = speed_ * deltaTime;
    GLfloat flightOffset = ROLL_SENSITIVITY * deltaTime;

    if (direction == Camera_Movement::FORWARD)
        cameraPos += cameraFront * deltaSpace;
    if (direction == Camera_Movement::PITCH_D)
        pitchControl(PITCH_D, -flightOffset);
    if (direction == Camera_Movement::PITCH_U)
        pitchControl(PITCH_D, flightOffset);
    if (direction == Camera_Movement::ROLL_L)
        rollControl(PITCH_D, -flightOffset);
    if (direction == Camera_Movement::ROLL_R)
        rollControl(PITCH_D, flightOffset);
    if (direction == Camera_Movement::YAW_L)
        yawControl(PITCH_D, flightOffset);
    if (direction == Camera_Movement::YAW_R)
        yawControl(PITCH_D, -flightOffset);
    if (direction == Camera_Movement::ACCELERATE)
    {
        speed_ += 2.0f;
        if (speed_ > MAX_SPEED) speed_ = MAX_SPEED;
    }
    if (direction == Camera_Movement::DECELERATE)
    {
        speed_ -= 2.0f;
        if (speed_ < 0) speed_ = 0.0f;
    }
}


void GT_Camera::mouseHandler(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch  )
{
    xoffset *= this->MouseSensitivity;
    yoffset *= this->MouseSensitivity;

    this->yaw   +=xoffset;
    this->pitch +=yoffset;

    if (constrainPitch)
    {
        if (this->pitch > 89.0f)
            this->pitch > 89.0f;
        if (this->pitch < -89.0f)
            this->pitch = -89.0f;
    }
}


glm::mat4 GT_Camera::GetViewMatrix()
{
    glm::vec3 horizontalOffset = -this->cameraFront*this->getSpeedOffset();
    glm::vec3 verticalOffset   =  this->cameraUp * 5.0f;
    return glm::lookAt(this->cameraPos+ horizontalOffset + verticalOffset, this->cameraPos + this->cameraFront, this->cameraUp );
}

inline GLfloat GT_Camera::getSpeedOffset()
{
     return ((speed_ - MAX_SPEED/4.0f)/MAX_SPEED)*CAMERA_OFFSET/3.0f + CAMERA_OFFSET;
}
