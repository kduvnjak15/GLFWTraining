#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

enum shaderTag
{
    enemyShader,
    modelShader,
    fontShader,
    skyboxShader,
    terrainShader

};

class GT_Shader
{
public:
    GT_Shader(shaderTag shaderTypeTag , const char* vsFileName = nullptr, const char* fsFileName = nullptr, const char* gsFileName = nullptr);

    void Use();

    GLuint shaderProgram_;

    shaderTag getShaderTag();


private:

    void validateShaderProgram();

    GLuint addShader(const char* shaderPath, GLenum shaderType);

private:

    shaderTag shaderTag_;

    GLuint vertexShader_;
    GLuint fragmentShader_;
    GLuint geometryShader_;

};

