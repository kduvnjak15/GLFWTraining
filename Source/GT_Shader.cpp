#include <GT_Shader.h>
#include <iostream>
#include <sstream>
#include <fstream>



GT_Shader::GT_Shader(const char* vsFileName, const char* fsFileName)
{

    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    vShaderFile.open(vsFileName);
    fShaderFile.open(fsFileName);

    std::stringstream vShaderStream, fShaderStream;

    vShaderStream << vShaderFile.rdbuf();
    fShaderStream << fShaderFile.rdbuf();

    vShaderFile.close();
    fShaderFile.close();

    vertexShaderSource = vShaderStream.str();
    fragmentShaderSource = fShaderStream.str();

    const GLchar* vertexShaderCode = vertexShaderSource.c_str();
    const GLchar* fragmentShaderCode = fragmentShaderSource.c_str();


    ///////////////////     vertex shader    //////////////////////////////////
    vertexShader_ = glCreateShader(GL_VERTEX_SHADER);


    glShaderSource(vertexShader_, 1, &vertexShaderCode, NULL);
    glCompileShader(vertexShader_);

    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(vertexShader_, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader_, 512, NULL, infoLog);
        std::cout<< "ERROR:: VERTEX_SHADER_COMPILE_FAILED! "<<infoLog<<std::endl;
    }


    /////////////////////    fragment shader    ////////////////////////////////
    fragmentShader_ = glCreateShader(GL_FRAGMENT_SHADER);


    glShaderSource(fragmentShader_, 1, &fragmentShaderCode, NULL);
    glCompileShader(fragmentShader_);

    glGetShaderiv(fragmentShader_, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader_, 512, NULL, infoLog);
        std::cout<< "ERROR:: FRAGMENT_SHADER_COMPILE_FAILED! "<<infoLog<<std::endl;
    }


    ///////////////////////     shader program    //////////////////////////////
    this->shaderProgram_ =  glCreateProgram();
    glAttachShader(this->shaderProgram_, vertexShader_);
    glAttachShader(this->shaderProgram_, fragmentShader_);
    glLinkProgram(this->shaderProgram_);

    glGetProgramiv(this->shaderProgram_, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(this->shaderProgram_, 512, NULL, infoLog);
        std::cout<< "ERROR:: FRAGMENT_SHADER_COMPILE_FAILED! "<<infoLog<<std::endl;
    }

    glDeleteShader(vertexShader_);
    glDeleteShader(fragmentShader_);
}

void GT_Shader::Use()
{
    glUseProgram(this->shaderProgram_);
}

bool GT_Shader::ReadShader(const char* shaderFileName, std::string& outFile )
{
    bool ret = false;
    std::ifstream shaderFile(shaderFileName);

    if (shaderFile.is_open())
    {
        std::string line;
        while (getline(shaderFile, line)) {
            outFile.append(line);
            outFile.append("\n");
        }

        shaderFile.close();

        ret = true;
    }
    else {
        ret = false;
    }

    return ret;
}
