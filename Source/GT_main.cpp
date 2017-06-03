#include <iostream>
#include <cmath>
#include <vector>
#include <map>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "GT_Locator.h"
#include "GT_MenuScene.h"
#include "GT_GameplayScene.h"
#include "GT_PauseScene.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "SFML/Audio.hpp"

bool keys[1024];
bool firstMouse = true;

GLfloat lastX = window_width/2, lastY = window_height/2;

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
    GAME() : s_(0), crashTime_(0.0), fly_(true)
    {
        gamePointer = this;
        std::cout<<"GLFWTraining GAME class "<<std::endl;
    }
    virtual ~GAME()
    {
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
        windowPtr_ = glfwCreateWindow(window_width, window_height, "SkyKnightsDEMO", NULL, NULL);
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
        glEnable(GL_BLEND);
        glBlendFunc(GL_DST_ALPHA, GL_ONE);
        return true;
    }

    void loadGame()
    {
        GT_Locator_ = new GT_Locator();

        std::cout << "Loading scenes "<< std::endl;

        scenes_.push_back(new GT_MenuScene());
        scenes_.push_back(new GT_GameplayScene());
        scenes_.push_back(new GT_PauseScene());
        curScene_ = scenes_[0];

    }

    bool Run()
    {
        glfwSwapInterval(1);
        loadGame();

        std::cout << "Starting GAME LOOP "<< std::endl;
        while (!glfwWindowShouldClose(windowPtr_))
        {

            ///////////////////////////////////////////////////////

            glfwPollEvents();

            /////////////////////    rendering    //////////////////

            curScene_->renderScene();

            /////////////////    gameplay control  //////////////////

            if (curScene_->nextScene_ != nonType )
            {
                if (curScene_->nextScene_ == gameplay)
                {
                    std::cout << "Next scene gameplay"<< std::endl;
                    curScene_->nextScene_ = nonType;
                    curScene_ = scenes_[1];
                }
                else if (curScene_->nextScene_ == menuScene)
                {
                    std::cout << "Next scene Menuscene"<< std::endl;
                    curScene_->nextScene_ = nonType;
                    curScene_ = scenes_[0];
                }
                else if (curScene_->nextScene_ == pauseScene)
                {
                    std::cout << "Next scene pausescene" << std::endl;
                    curScene_->nextScene_ = nonType;
                    curScene_ = scenes_[2];
                }
                else if (curScene_->nextScene_ == exitGame)
                    return true;
                else
                    curScene_ = scenes_[0];
            }


            ////////////////////////////////////////////////////////

            glfwSwapBuffers(windowPtr_);
        }

        glfwTerminate();
        return true;
    }


private:
//class private function members
    void initSceneContext()
    {
        glViewport(0, 0, window_width, window_height);
    }

    void initializeCallbacks()
    {
        glfwSetKeyCallback(windowPtr_, KeyboardCB);
        glfwSetCursorPosCallback(windowPtr_, MouseCB);
    }

    void KeyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
    {

        if (key == GLFW_KEY_ESCAPE && key == GLFW_KEY_RIGHT_SHIFT && action == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window, GL_TRUE);
        }

        if (key == GLFW_KEY_T )
        {
            toggle_ = !toggle_;
        }

        if(action == GLFW_PRESS)
            keys[key] = true;
        else if(action == GLFW_RELEASE)
            keys[key] = false;

        if (curScene_)
            curScene_->sceneKeyboardHandler(keys, key, scancode, action, mode);

        else
        {
            std::cout << "NO CURSCENE_; CATASTRPHIC CRASH" << std::endl;
            glfwSetWindowShouldClose(window, GL_TRUE);
        }

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

    std::vector<GT_Scene*> scenes_;
    GT_Scene* curScene_;

    GT_Locator* GT_Locator_;

    // uniforms
    GLfloat s_;
    GLfloat crashTime_;
    GLboolean fly_;
    GLboolean toggle_;

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
