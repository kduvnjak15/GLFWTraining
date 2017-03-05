#include "GT_Camera.h"
#include <GL/glew.h>

GT_Camera::GT_Camera()
    : cameraPos(0.0f, 0.0f, 0.5f),
      cameraFront(0.0f, 0.0f, -1.0f),
      cameraUp(0.0f, 1.0f, 0.0f),
      cameraWorldUp(0.0f, 1.0f, 0.0f),

      yaw(YAW),
      pitch(PITCH),
      MovementSpeed(SPEED),
      MouseSensitivity(SENSITIVITY),
      Zoom(ZOOM)
{
    this->updateCameraVectors();
}


void GT_Camera::updateCameraVectors()
{
    // Calculate the new Front vector
    glm::vec3 front;
    front.x = cos(glm::radians(this->yaw))* cos(glm::radians(this->pitch));
    front.y = sin(glm::radians(this->pitch));
    front.z = sin(glm::radians(this->yaw))*cos(glm::radians(this->pitch));
    this->cameraFront = glm::normalize(front);


    std::cout<<cameraFront.x<<", "<<cameraFront.y<<", "<<cameraFront.z<<"; "<<cameraWorldUp.x<<", "<<cameraWorldUp.y<<", "<<cameraWorldUp.z<<";"<<cameraPos.x<<", "<<cameraPos.y<<", "<<cameraPos.z<<std::endl;

    //Also recalculate Right and Up vector
    this->cameraRight = glm::normalize( glm::cross(this->cameraFront, this->cameraWorldUp));
    this->cameraUp    = glm::normalize( glm::cross(this->cameraRight, this->cameraFront));

}


void GT_Camera::keyboardHandler(Camera_Movement direction, GLfloat deltaTime)
{


    GLfloat deltaSpace = SPEED * deltaTime;
    std::cout<<deltaSpace<<std::endl;
    if (direction == Camera_Movement::FORWARD)
        this->cameraPos += this->cameraFront * deltaSpace;
    if (direction == Camera_Movement::BACKWARD)
        this->cameraPos -= this->cameraFront * deltaSpace;
    if (direction == Camera_Movement::LEFT)
        this->cameraPos -= this->cameraRight * deltaSpace;
    if (direction == Camera_Movement::RIGHT)
        this->cameraPos += this->cameraRight * deltaSpace;

    this->updateCameraVectors();
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

    this->updateCameraVectors();
}


glm::mat4 GT_Camera::GetViewMatrix()
{
    return glm::lookAt(this->cameraPos, this->cameraPos + this->cameraFront, this->cameraUp );
}

