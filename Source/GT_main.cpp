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
#include "GT_CreditScene.h"
#include "GT_GameoverScene.h"
#include "GT_IntroScene.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "SFML/Audio.hpp"

#include <time.h>

bool keys[1024];

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
    GAME()
    {
        srand ( time(NULL) );
        gamePointer = this;
        std::cout<<"GLFWTraining GAME class info "<<std::endl;
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
//        glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

        GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* videoMode = glfwGetVideoMode(primaryMonitor);
        GLuint monitor_width    = videoMode->width;
        GLuint monitor_height   = videoMode->height;

        std::cout << monitor_width << ", "<< monitor_height <<std::endl;

        // CreateWindow
        windowPtr_ = glfwCreateWindow(monitor_width, monitor_height, "SkyKnightsDEMO", glfwGetPrimaryMonitor(), NULL);
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
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        return true;
    }

    void loadGame()
    {
        GT_Locator_ = new GT_Locator();
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
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            currScene_ = GT_Locator::getSceneManager()->getActiveScene();
            if (currScene_ != nullptr )
                currScene_->renderScene();
            else
                glfwSetWindowShouldClose(windowPtr_, GL_TRUE );

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
        glViewport(0, 0, glfwGetVideoMode(glfwGetPrimaryMonitor())->width, glfwGetVideoMode(glfwGetPrimaryMonitor())->height);

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

        if(action == GLFW_PRESS)
            keys[key] = true;
        else if(action == GLFW_RELEASE)
            keys[key] = false;

        if (currScene_)
            currScene_->sceneKeyboardHandler(keys, key, scancode, action, mode);
        else
        {
            std::cout << "NO CURSCENE_; CATASTRPHIC CRASH" << std::endl;
            glfwSetWindowShouldClose(window, GL_TRUE);
        }

     }

    void MouseCallback(GLFWwindow* window, double xpos, double ypos)
    {
//        if (firstMouse)
//        {
//            lastX = xpos;
//            lastY = ypos;
//            firstMouse = false;
//        }

//        GLfloat xoffset = xpos -lastX;
//        GLfloat yoffset = ypos -lastY;

//        lastX = xpos;
//        lastY = ypos;

        //camera_->mouseHandler(xoffset, -yoffset);
    }

//class private members
private:

    GLFWwindow* windowPtr_;

    std::vector<GT_Scene*> scenes_;
    GT_Scene* currScene_;

    GT_Locator* GT_Locator_;

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
