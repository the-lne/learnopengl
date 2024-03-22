#ifndef __SHADERS_H__
#define __SHADERS_H__

class Shaders {
    public:
        #include "shader.strings"

        unsigned int vertex_shader;
        unsigned int fragment_shader;

        int buildVertexShader();
        int buildFragmentShader();
        int linkShaders(unsigned int* shaderProgram);
};

#endif