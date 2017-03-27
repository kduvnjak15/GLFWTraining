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
    cameraRight = glm::normalize(glm::cross(cameraFront, cameraUp));
    //this->updateCameraVectors();

}


glm::vec3 GT_Camera::getRotatedVector()
{
    // Calculate the new Front vector
//    glm::vec3 front;
//    front.x = cos(glm::radians(this->yaw))* cos(glm::radians(this->pitch));
//    front.y = sin(glm::radians(this->pitch));
//    front.z = sin(glm::radians(this->yaw))*cos(glm::radians(this->pitch));
//    this->cameraFront = glm::normalize(front);

    std::cout<<"NA: "<<a<<b<<c<<"\n"<<u<<v<<w<<"\n"<<x<<z<<y<<std::endl;

    glm::vec3 retVec;

    retVec.x = (a*(v*v+w*w) - u*(b*v+c*w-u*x-v*y-w*z))*(1-cos(th)) + L*x*cos(th)+ std::sqrt(L)*(-c*v+b*w-w*y+v*z)*sin(th);
    retVec.y = (b*(u*u+w*w) - v*(a*u+c*w-u*x-v*y-w*z))*(1-cos(th)) + L*y*cos(th)+ std::sqrt(L)*( c*u-a*w+w*x-u*z)*sin(th);
    retVec.z = (c*(u*u+v*v) - w*(a*u+b*v-u*x-v*y-w*z))*(1-cos(th)) + L*z*cos(th)+ std::sqrt(L)*(-b*u+a*v-v*x+u*y)*sin(th);

    std::cout<<"revec "<<retVec.x<<", "<<retVec.y<<", "<<retVec.z<<"L "<<L<<std::endl;
    glm::normalize(retVec);
    retVec = retVec * (1/L);
    return glm::normalize(retVec);

//    std::cout<<"CF: "<<cameraFront.x<<", "<<cameraFront.y<<", "<<cameraFront.z<<"; "<<cameraWorldUp.x<<", "<<cameraWorldUp.y<<", "<<cameraWorldUp.z<<";"<<cameraPos.x<<", "<<cameraPos.y<<", "<<cameraPos.z<<std::endl;

    //Also recalculate Right and Up vector
//    this->cameraRight = glm::normalize( glm::cross(this->cameraFront, this->cameraWorldUp));
//    this->cameraUp    = glm::normalize( glm::cross(this->cameraRight, this->cameraFront));

}

void GT_Camera::pitchControl(Camera_Movement controlDirection, GLfloat deltaSpace)
{
    a = 0; b = 0; c = 0;       // pivot point
    u = cameraRight.x; v = cameraRight.y; w = cameraRight.z; // axis
    x = cameraFront.x; y = cameraFront.y; z = cameraFront.z; // subject point

    th = deltaSpace;
    L = u*u + v*v + w*w;

    cameraFront = getRotatedVector();

 //   this->cameraRight = glm::normalize(glm::cross(this->cameraFront, this->cameraUp));
    this->cameraUp    = glm::normalize(glm::cross(this->cameraRight, this->cameraFront));

}

void GT_Camera::rollControl(Camera_Movement controlDirection, GLfloat deltaSpace)
{
    th = deltaSpace;
    L = u*u + v*v + w*w;

    a = cameraPos.x; b = cameraPos.y; c = cameraPos.z;       // pivot point
    u = cameraFront.x; v = cameraFront.y; w = cameraFront.z; // axis
    x = cameraUp.x; y = cameraUp.y; z = cameraUp.z; // subject point

    cameraUp = getRotatedVector();

    this->cameraRight = glm::normalize(glm::cross(this->cameraFront, this->cameraUp));
   // this->cameraUp    = glm::normalize(glm::cross(this->cameraRight, this->cameraFront));


}
void GT_Camera::yawControl(Camera_Movement controlDirection, GLfloat deltaSpace)
{
    th = deltaSpace;
    L = u*u + v*v + w*w;

    a = cameraPos.x; b = cameraPos.y; c = cameraPos.z;       // pivot point
    u = cameraUp.x; v = cameraUp.y; w = cameraUp.z; // axis
    x = cameraFront.x; y = cameraFront.y; z = cameraFront.z; // subject point

    cameraFront = getRotatedVector();

    this->cameraRight = glm::normalize(glm::cross(this->cameraFront, this->cameraUp));
    //this->cameraUp    = glm::normalize(glm::cross(this->cameraRight, this->cameraFront));

}

void GT_Camera::keyboardHandler(Camera_Movement direction, GLfloat deltaTime)
{

    GLfloat deltaSpace = SPEED * deltaTime;
    if (direction == Camera_Movement::FORWARD)
        cameraPos += cameraFront * deltaSpace;
    if (direction == Camera_Movement::PITCH_D)
        pitchControl(PITCH_D, -deltaSpace);
    if (direction == Camera_Movement::PITCH_U)
        pitchControl(PITCH_D, deltaSpace);
    if (direction == Camera_Movement::ROLL_L)
        rollControl(PITCH_D, -deltaSpace);
    if (direction == Camera_Movement::ROLL_R)
        rollControl(PITCH_D, deltaSpace);
    if (direction == Camera_Movement::YAW_L)
        yawControl(PITCH_D, deltaSpace);
    if (direction == Camera_Movement::YAW_R)
        yawControl(PITCH_D, -deltaSpace);

//    this->updateCameraVectors();
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

//    this->updateCameraVectors();
}


glm::mat4 GT_Camera::GetViewMatrix()
{
    return glm::lookAt(this->cameraPos, this->cameraPos + this->cameraFront, this->cameraUp );
}

