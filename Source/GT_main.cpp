#include <iostream>
#include <cmath>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "GT_Shader.h"

#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 600

#define VShader "../Shaders/shader.vs"
#define FShader "../Shaders/shader.fs"

class initialCallbacks
{
public:
    virtual void KeyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mode) = 0;
};

initialCallbacks* gamePointer = NULL;


// static callbacks
static void KeyboardCB(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    gamePointer->KeyboardCallback( window, key, scancode, action, mode );
}





class GAME : public initialCallbacks
{
public:
    GAME()
    {
        gamePointer = this;
        std::cout<<"GLFWTraining GAME class "<<std::endl;
    }
    ~GAME() = default;

    bool initGAME()
    {
        // GLFW initialize
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

        // CreateWindow
        windowPtr_ = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "GLFWTraining", NULL, NULL);
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

        return true;
    }

    bool Run()
    {
        glGenVertexArrays(1, &VAO_);
        glBindVertexArray(VAO_);


        glGenBuffers(1, &VBO_);
        glBindBuffer(GL_ARRAY_BUFFER, VBO_);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


        glGenBuffers(1, &EBO_);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (const GLvoid*)0);
        glEnableVertexAttribArray(0);



glBindVertexArray(0);

        GT_Shader(VShader, FShader);

glBindVertexArray(VAO_);
        int b= 0;
        while (!glfwWindowShouldClose(windowPtr_))
        {
            b++;
            glfwPollEvents();
            /////////////////////    rendering    //////////////////

            glClearColor(0.0, 0.15f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glDrawElements(GL_TRIANGLES, 6,GL_UNSIGNED_INT, 0);



            ////////////////////////////////////////////////////////
            glfwSwapBuffers(windowPtr_);
        }
glBindVertexArray(0);
        glfwTerminate();
        return true;
    }

private:
//class private function members
    void initSceneContext()
    {
        glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    }

    void initializeCallbacks()
    {
        glfwSetKeyCallback(windowPtr_, KeyboardCB);
    }

    void KeyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
    {
        if (key == GLFW_KEY_ESCAPE || key == GLFW_KEY_Q  && action == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window, GL_TRUE);
        }

        if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
        {
            glDrawArrays(GL_TRIANGLES, 0, 3);
        }
    }

    GLfloat vertices[12] = {
    0.5f, 0.5f, 0.0f, // Top Right
    0.5f, -0.5f, 0.0f, // Bottom Right
    -0.5f, -0.5f, 0.0f, // Bottom Left
    -0.5f, 0.5f, 0.0f // Top Left
    };


    GLuint indices[6] = { 0, 1, 3,
                         1, 2, 3};

//class private members
private:
    GLuint VBO_;
    GLuint VAO_;
    GLuint EBO_;
    GLFWwindow* windowPtr_;
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

