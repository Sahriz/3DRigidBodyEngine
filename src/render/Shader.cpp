#include "Shader.hpp"

#include <fstream>
#include <iostream>
#include <iterator>

void Shader::InitProgram(std::string const source)
{
    std::string vertexShaderSourceString = readFile(source + "shader.vert");
    std::string fragmentShaderSourceString = readFile(source + "shader.frag");

    const char* vertexShaderSource = vertexShaderSourceString.c_str();
    const char* fragmentShaderSource = fragmentShaderSourceString.c_str();

    unsigned int vertexShader;
    unsigned int fragmentShader;

    vertexShader = glCreateShader(GL_VERTEX_SHADER);

    int  success;
    char infoLog[512];

    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    ShaderProgram = glCreateProgram();

    glAttachShader(ShaderProgram, vertexShader);
    glAttachShader(ShaderProgram, fragmentShader);
    glLinkProgram(ShaderProgram);

    glUseProgram(ShaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

Shader::~Shader()
{
    glDeleteProgram(ShaderProgram);
}

GLuint Shader::getShader()
{
    return ShaderProgram;
}

std::string Shader::readFile(std::string path)
{
    std::ifstream in(path);
    std::string contents((std::istreambuf_iterator<char>(in)),
        std::istreambuf_iterator<char>());

    return contents;
}
