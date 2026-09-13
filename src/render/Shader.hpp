#pragma once

#include <glad/glad.h>
#include <string>

class Shader {
public:
    void InitProgram(std::string const source);
    ~Shader();
    GLuint getShader();

private:
    GLuint ShaderProgram = 0;
    std::string readFile(std::string path);
};
