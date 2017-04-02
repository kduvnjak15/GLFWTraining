#include <iostream>
#include <cmath>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "GT_Shader.h"
#include "GT_Camera.h"
#include "GT_Model.h"
#include "GT_Skybox.h"
#include "GT_Alphabet.h"
#include "GT_Rocket.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define VmodelShader "../Shaders/modelShader.vs"
#define FmodelShader "../Shaders/modelShader.fs"
#define vLightShader  "../Shaders/vLightShader.vs"
#define fLightShader  "../Shaders/fLightShader.fs"
#define vsSkyboxShader "../Shaders/skybox.vs"
#define fsSkyboxShader "../Shaders/skybox.fs"
#define vsFontShader "../Shaders/fontShader.vs"
#define fsFontShader "../Shaders/fontShader.fs"

bool keys[1024];
bool firstMouse = true;

const GLuint window_width = 1024;
const GLuint window_height = 600;
GLfloat lastX = window_width/2, lastY = window_height/2;
const GLfloat horizon = 50000;


GLfloat runTime = 0.0f;
GLfloat fps = 60.0f;

GLfloat currentFrame = 0.0f;
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

glm::vec3 lightPos(5.0f, 4.0f, 3.0f);

class initialCallbacks
{
public:
    virtual void KeyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mode) = 0;
    virtual void MouseCallback(GLFWwindow* window, double xpos, double ypos) = 0;
};

initialCallbacks* gamePointer = NULL;


// static callbacks
static void KeyboardCB(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    gamePointer->KeyboardCallback( window, key, scancode, action, mode );
}

static void MouseCB(GLFWwindow* window, double xpos, double ypos)
{
    gamePointer->MouseCallback(window, xpos, ypos);
}


class GAME : public initialCallbacks
{
public:
    GAME() : s_(0), rotate_(0), aimed_(false)

    {
        gamePointer = this;
        std::cout<<"GLFWTraining GAME class "<<std::endl;
    }
    virtual ~GAME()
    {

        delete shader_;
        delete lightShader_;
        delete cubemapShader_;
        delete skyboxShader_;
        delete fontShader_;

        delete camera_;
        delete skybox_;
        delete windowPtr_;
    }

    bool initGAME()
    {
        // GLFW initialize
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

        // CreateWindow
        windowPtr_ = glfwCreateWindow(window_width, window_height, "GLFWTraining", NULL, NULL);
        if (windowPtr_ == NULL)
        {
            std::cout<<"Failed to create GLFW window"<<std::endl;
            glfwTerminate();
            return -1;
        }
        glfwMakeContextCurrent(windowPtr_);

        // GLEW initialize
        glewExperimental = GL_TRUE;
        if (glewInit() != GLEW_OK)
        {
            std::cout << "Failed to initialize GLEW" << std::endl;
            return -1;
        }

        initSceneContext();
        initializeCallbacks();

        // OpenGL options
        glEnable(GL_DEPTH_TEST);


        return true;
    }



    bool Run()
    {

        camera_ = new GT_Camera();
        shader_ = new GT_Shader("modelShader", VmodelShader, FmodelShader);
        skybox_ = new GT_Skybox();
        skyboxShader_ = new GT_Shader("skyboxShader", vsSkyboxShader, fsSkyboxShader);
        fontShader_ = new GT_Shader("fontshader", vsFontShader, fsFontShader);

        font_ = new GT_Alphabet();

        actors_.push_back(new GT_Model("../Content/FA-22_Raptor/FA-22_Raptor.obj"));
        actors_.push_back(new GT_Model("../Content/CV - Essex class/essex_scb-125_generic.obj"));
        actors_.push_back(new GT_Model("../Content/FA-18_RC/FA-18_RC.obj"));
        actors_.push_back(new GT_Rocket("../Content/AVMT300/AVMT300.3ds"));
        actors_.push_back(new GT_Rocket("../Content/AVMT300/AVMT300.3ds"));


        while (!glfwWindowShouldClose(windowPtr_))
        {


            //////////////////////   timer ////////////////////////

            deltaTime = 0.0f;
            while (deltaTime < (1.0f/fps))
            {
                currentFrame = glfwGetTime();
                deltaTime = currentFrame - lastFrame;
            }
                lastFrame = currentFrame;


            ///////////////////////////////////////////////////////

            glfwPollEvents();
            do_movement();

            /////////////////////   color buffer   ////////////////

            glClearColor(0.0, 0.15f, 0.2f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            ///////////////////////////////////////////////////////

            gameRules();

            target_ = nullptr;
            aimed(actors_[1]);
            aimed(actors_[0]);

            updateRockets();


            //////////////////    camera movement    ///////////////

            model_ = actors_[1]; // uss Carrier
            model_->modelPos = glm::vec3(-1000.0f, -10.0f, -2500);
            shader_->Use();
            GLuint modelLoc  = glGetUniformLocation(shader_->shaderProgram_, "model");
            GLuint viewLoc  = glGetUniformLocation(shader_->shaderProgram_, "view");
            GLuint projLoc  = glGetUniformLocation(shader_->shaderProgram_, "projection");
            glm::mat4 model = glm::mat4(1.0f);

            model = glm::translate(model, model_->modelPos);
            model = glm::scale(model, glm::vec3(1000.0f));
            glm::mat4 view = camera_->GetViewMatrix();

            glm::mat4 projection = glm::perspective(ZOOM, (window_width*1.0f)/window_height, 0.1f, horizon);
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
            glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
            model_->Draw(*shader_);

            /****************************************************************/

            model_ = actors_[2]; // Fighter
            model_->modelPos = camera_->getCameraPos() +  glm::vec3(0.0f, -5.0f, -25.0f);
            shader_->Use();
            modelLoc  = glGetUniformLocation(shader_->shaderProgram_, "model");
            viewLoc  = glGetUniformLocation(shader_->shaderProgram_, "view");
            projLoc  = glGetUniformLocation(shader_->shaderProgram_, "projection");
            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(0.0f, -5.0f, -25.0f));
            model = glm::rotate(model,(GLfloat)-3.14159/2.0f,  glm::vec3(1.0f, 0.0f, 0.0f));

            view = glm::mat4(1.0f);
            projection = glm::perspective(ZOOM, (window_width*1.0f)/window_height, 0.1f, horizon);
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
            glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
            model_->Draw(*shader_);

            /****************************************************************/

            model_ = actors_[0]; // Enemy
            model_->modelPos = glm::vec3(5000,500,-2600);
            shader_->Use();
            modelLoc  = glGetUniformLocation(shader_->shaderProgram_, "model");
            viewLoc  = glGetUniformLocation(shader_->shaderProgram_, "view");
            projLoc  = glGetUniformLocation(shader_->shaderProgram_, "projection");
            model = glm::mat4(1.0f);
            model = glm::translate(model, model_->modelPos);
            model = glm::rotate(model,(GLfloat)-3.14159/2.0f,  glm::vec3(1.0f, 0.0f, 0.0f));

            model = glm::scale(model, glm::vec3(2.0f));

            view = camera_->GetViewMatrix();
            projection = glm::perspective(ZOOM, (window_width*1.0f)/window_height, 0.1f, 10000.0f);
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
            glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
            model_->Draw(*shader_);


            /****************************************************************/

            model_ = actors_[3]; // Missile
            shader_->Use();
            modelLoc  = glGetUniformLocation(shader_->shaderProgram_, "model");
            viewLoc  = glGetUniformLocation(shader_->shaderProgram_, "view");
            projLoc  = glGetUniformLocation(shader_->shaderProgram_, "projection");

            model = glm::mat4(1.0f);


            if (model_->isFired())
            {
                view = camera_->GetViewMatrix();
                model = glm::translate(model, actors_[3]->modelPos);
            }
            else
            {
                model = glm::translate(model, glm::vec3(-7.0f, -5.0f, -35.0f));
                actors_[3]->modelPos = camera_->getCameraPos() + glm::vec3(-7.0f, -5.0f, -35.0f);
                view = glm::mat4(1.0f);
            }


            model = glm::scale(model, glm::vec3(0.3f));
            model = glm::rotate(model,(GLfloat)-3.14159/2.0f,  glm::vec3(0.0f, 1.0f, 0.0f));
            projection = glm::perspective(ZOOM, (window_width*1.0f)/window_height, 0.1f, 10000.0f);
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
            glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
            model_->Draw(*shader_);


            model_ = actors_[4]; // Missile
            model = glm::mat4(1.0f);
            if (model_->isFired())
            {
                view = camera_->GetViewMatrix();
                model = glm::translate(model, actors_[4]->modelPos);
            }
            else
            {
                model = glm::translate(model,glm::vec3(12.0f, -5.0f, -35.0f));
                actors_[4]->modelPos = camera_->getCameraPos() + glm::vec3(12.0f, -5.0f, -35.0f);
                view = glm::mat4(1.0f);
            }


            model = glm::scale(model, glm::vec3(0.3f));
            model = glm::rotate(model,(GLfloat)-3.14159/2.0f,  glm::vec3(0.0f, 1.0f, 0.0f));


            projection = glm::perspective(ZOOM, (window_width*1.0f)/window_height, 0.1f, 10000.0f);
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
            glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
            model_->Draw(*shader_);

            ///////////////////////////////////////////////////////////////////////////////////////
            // skybox
            glDepthFunc(GL_LEQUAL);
            skyboxShader_->Use();


            viewLoc  = glGetUniformLocation(skyboxShader_->shaderProgram_, "view");
            projLoc  = glGetUniformLocation(skyboxShader_->shaderProgram_, "projection");
            view = glm::mat4(glm::mat3(camera_->GetViewMatrix()));
            projection = glm::perspective(ZOOM, (window_width*1.0f)/window_height, 0.1f, 10000.0f);
            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
            glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

            glBindVertexArray(skybox_->skyboxVAO);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_CUBE_MAP, skybox_->skyboxTexID);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            glBindVertexArray(0);
            glDepthFunc(GL_LESS);

            // Fonts
            projection = glm::ortho(0.0f, static_cast<GLfloat>(window_width), 0.0f, static_cast<GLfloat>(window_height));
            fontShader_->Use();
            glUniformMatrix4fv(glGetUniformLocation(fontShader_->shaderProgram_, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
            font_->RenderText(*fontShader_, std::to_string(currentFrame), 25.0f, 25.0f, .50f, glm::vec3(0.5, 0.8f, 0.2f));
            font_->RenderText(*fontShader_, std::to_string(1/deltaTime), 2.0f, 2.0f, .50f, glm::vec3(0.5, 0.8f, 0.2f));
            font_->RenderText(*fontShader_, std::to_string(camera_->getSpeed()), 50.0f, 50.0f, .50f, glm::vec3(0.5, 0.8f, 0.2f));
            font_->RenderText(*fontShader_, std::to_string(fly_), 10.0f, 60.0f, .50f, glm::vec3(0.5, 0.8f, 0.2f));

            // camera position
            font_->RenderText(*fontShader_, std::to_string(camera_->getCameraPos().x), 5.0f, 500.0f, .50f, glm::vec3(0.5, 0.8f, 0.2f));
            font_->RenderText(*fontShader_, std::to_string(camera_->getCameraPos().y), 130.0f, 500.0f, .50f, glm::vec3(0.5, 0.8f, 0.2f));
            font_->RenderText(*fontShader_, std::to_string(camera_->getCameraPos().z), 240.0f, 500.0f, .50f, glm::vec3(0.5, 0.8f, 0.2f));
            // camera Front
            font_->RenderText(*fontShader_, std::to_string(camera_->getCameraFront().x), 5.0f, 480.0f, .50f, glm::vec3(0.5, 0.8f, 0.2f));
            font_->RenderText(*fontShader_, std::to_string(camera_->getCameraFront().y), 130.0f, 480.0f, .50f, glm::vec3(0.5, 0.8f, 0.2f));
            font_->RenderText(*fontShader_, std::to_string(camera_->getCameraFront().z), 240.0f, 480.0f, .50f, glm::vec3(0.5, 0.8f, 0.2f));
            // enemy pos
            font_->RenderText(*fontShader_, std::to_string(actors_[0]->modelPos.x), 5.0f, 460.0f, .50f, glm::vec3(0.5, 0.8f, 0.2f));
            font_->RenderText(*fontShader_, std::to_string(actors_[0]->modelPos.y), 130.0f, 460.0f, .50f, glm::vec3(0.5, 0.8f, 0.2f));
            font_->RenderText(*fontShader_, std::to_string(actors_[0]->modelPos.z), 240.0f, 460.0f, .50f, glm::vec3(0.5, 0.8f, 0.2f));

            /////////////////////    rendering    //////////////////



             ////////////////////////////////////////////////////////
            glfwSwapBuffers(windowPtr_);
        }
glBindVertexArray(0);
        glfwTerminate();
        return true;
    }


    void updateRockets()
    {
        if (actors_[3]->isFired())
            actors_[3]->move();
        if (actors_[4]->isFired())
            actors_[4]->move();
    }

    void handleCrash()
    {


        glfwWindowShouldClose(windowPtr_);

        glfwTerminate();

    }

    void gameRules()
    {

    }


    void aimed(GT_Model* target)
    {
        glm::vec3 enemyPos = target->modelPos;
        glm::vec3 fighterPos = actors_[2]->modelPos;
        glm::vec3 diagonal =  glm::normalize( enemyPos - fighterPos );

        // enemy pos
        font_->RenderText(*fontShader_, std::to_string(diagonal.x), 5.0f, 730.0f, .50f, glm::vec3(0.5, 0.8f, 0.2f));
        font_->RenderText(*fontShader_, std::to_string(diagonal.y), 130.0f, 730.0f, .50f, glm::vec3(0.5, 0.8f, 0.2f));
        font_->RenderText(*fontShader_, std::to_string(diagonal.z), 240.0f, 730.0f, .50f, glm::vec3(0.5, 0.8f, 0.2f));


        GLfloat tempDOT = glm::dot(camera_->getCameraFront(), diagonal );
        if (tempDOT > 0.997 )
        {
            font_->RenderText(*fontShader_, "AIMED!!!", 700.0f, 100.0f, 1.0f, glm::vec3(1.0, 0.0f, .4f));
            if (sin(glfwGetTime()*6) >0)
                if (actors_[3]->isFired() && actors_[4]->isFired())
                    font_->RenderText(*fontShader_, "OUT OF MISSILES!!!", 630.0f, 65.0f, 0.8f, glm::vec3(1.0, 0.0f, .4f));
                else
                    font_->RenderText(*fontShader_, "FIRE!!!", 710.0f, 65.0f, 0.8f, glm::vec3(1.0, 0.0f, .4f));
            target_ = target;

        }
        else
        {
//            std::cout<<    glm::dot(camera_->getCameraFront(), diagonal )<<std::endl;

        }


    }

private:
//class private function members
    void initSceneContext()
    {
        glViewport(0, 0, window_width, window_height);
    }

    void do_movement()
    {
#ifdef oldcode
        // Camera controls
        if (keys[GLFW_KEY_W])
            camera_->keyboardHandler(FORWARD, deltaTime);
        if (keys[GLFW_KEY_S])
            camera_->keyboardHandler(BACKWARD, deltaTime);
        if (keys[GLFW_KEY_A])
        {
            camera_->keyboardHandler(LEFT, deltaTime);
            rotate_ += 0.005f;
        }
        if (keys[GLFW_KEY_D])
        {
            camera_->keyboardHandler(RIGHT, deltaTime);
            rotate_ -= 0.005f;
        }
#else
    if (fly_)
        camera_->keyboardHandler(FORWARD, deltaTime);
    if (keys[GLFW_KEY_W])
        camera_->keyboardHandler(PITCH_D, deltaTime);
    if (keys[GLFW_KEY_S])
        camera_->keyboardHandler(PITCH_U, deltaTime);
    if (keys[GLFW_KEY_A])
        camera_->keyboardHandler(ROLL_L, deltaTime);
    if (keys[GLFW_KEY_D])
        camera_->keyboardHandler(ROLL_R, deltaTime);
    if (keys[GLFW_KEY_Q])
        camera_->keyboardHandler(YAW_L, deltaTime);
    if (keys[GLFW_KEY_E])
        camera_->keyboardHandler(YAW_R, deltaTime);
    if (keys[GLFW_KEY_LEFT_CONTROL])
        camera_->keyboardHandler(ACCELERATE, deltaTime);
    if (keys[GLFW_KEY_LEFT_SHIFT])
        camera_->keyboardHandler(DECELERATE, deltaTime);

#endif
    }

    void initializeCallbacks()
    {
        glfwSetKeyCallback(windowPtr_, KeyboardCB);
        glfwSetCursorPosCallback(windowPtr_, MouseCB);
    }

    void KeyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
    {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window, GL_TRUE);
        }

        if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
        {
            fly_ = !fly_;
        }

        if (key == GLFW_KEY_T )
        {
            toggle_ = !toggle_;
        }

        if (key == GLFW_KEY_ENTER && action == GLFW_PRESS)
        {
            std::cout<<(target_ == nullptr)<<std::endl;
            if (target_ != nullptr)
            {
                if (!actors_[3]->isFired())
                    actors_[3]->Fire(target_);
                else if (!actors_[4]->isFired())
                    actors_[4]->Fire(target_);
            }
        }

        if(action == GLFW_PRESS)
            keys[key] = true;
        else if(action == GLFW_RELEASE)
            keys[key] = false;
     }

    void MouseCallback(GLFWwindow* window, double xpos, double ypos)
    {
        if (firstMouse)
        {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        GLfloat xoffset = xpos -lastX;
        GLfloat yoffset = ypos -lastY;

        lastX = xpos;
        lastY = ypos;

        //camera_->mouseHandler(xoffset, -yoffset);
    }

//class private members
private:

    GLFWwindow* windowPtr_;

    // shaders
    std::map<std::string, GT_Shader> shaders_;
    GT_Shader* shader_;
    GT_Shader* lightShader_;
    GT_Shader* cubemapShader_;
    GT_Shader* skyboxShader_;
    GT_Shader* fontShader_;


    // classes
    GT_Camera* camera_;
    GT_Skybox* skybox_;
    GT_Alphabet* font_;

    // Models
    std::vector<GT_Model*> actors_;
    GT_Model* model_;

    // uniforms
    GLfloat s_;
    GLboolean fly_;
    GLboolean toggle_;
    GLboolean aimed_;
    GT_Model* target_ ;
    GLfloat rotate_;

    GLfloat blinker_ ;

};


int main(int argc, char** argv)
{
    GAME* gamePtr = new GAME();

    if ( !gamePtr->initGAME() )
    {
        std::cout<<" GameInit Failed! "<<std::endl;
        return -1;
    }

    if ( !gamePtr->Run() )
    {
        std::cout<<" GameInit Failed! "<<std::endl;
        return -1;
    }


    std::cout<<"GameOver"<<std::endl;
    return 0;
}
