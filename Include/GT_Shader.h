#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string.h>
#include <fstream>

class GT_Shader
{
public:
    GT_Shader(const char* vsFileName, const char* fsFileName);

private:

    bool ReadShader(const char* shaderFileName, std::string& outFile );

private:
    GLuint vertexShader_;
    GLuint fragmentShader_;
    GLuint shaderProgram_;

    std::string vertexShaderSource;
    std::string fragmentShaderSource;
};

