#include <iostream>
#include "glad.h"
#include <GLFW/glfw3.h>
#include "shaders.h"


int Shaders::buildVertexShader() {
    Shaders::vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(Shaders::vertex_shader, 1, &vertex_shader_source, NULL);
    glCompileShader(Shaders::vertex_shader);
    // check for compilation errors //
    int success;
    char infoLog[512];
    glGetShaderiv(Shaders::vertex_shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(Shaders::vertex_shader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        return 1;
    }
    return 0;
}

int Shaders::buildFragmentShader() {
    Shaders::fragment_shader = glCreateShader(GL_FRAGMENT_SHADER); 
    glShaderSource(Shaders::fragment_shader, 1, &fragment_shader_source, NULL); 
    glCompileShader(Shaders::fragment_shader); 
    // check for compilation errors //
    int success;
    char infoLog[512];
    glGetShaderiv(Shaders::fragment_shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(Shaders::fragment_shader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        return 1;
    }
    return 0;
}

int Shaders::linkShaders(unsigned int* shaderProgram) {
    *shaderProgram = glCreateProgram();
    glAttachShader(*shaderProgram, Shaders::vertex_shader);
    glAttachShader(*shaderProgram, Shaders::fragment_shader);
    glLinkProgram(*shaderProgram);
    // check for linking errors
    int success;
    char infoLog[512];
    glGetProgramiv(*shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(*shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        return 1;
    }
    glDeleteShader(Shaders::vertex_shader);
    glDeleteShader(Shaders::fragment_shader);
    return 0;
}