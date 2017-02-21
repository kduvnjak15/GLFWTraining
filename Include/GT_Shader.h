#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string.h>

class GT_Shader
{
public:
    GT_Shader(const char* vsFileName, const char* fsFileName);

    void Use();

    GLuint shaderProgram_;

private:

    bool ReadShader(const char* shaderFileName, std::string& outFile );

private:
    GLuint vertexShader_;
    GLuint fragmentShader_;

    std::string vertexShaderSource;
    std::string fragmentShaderSource;
};

