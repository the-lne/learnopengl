#ifndef __SHADERS_H__
#define __SHADERS_H__

class Shaders {
    public:
        #include "glsl.mwahaha"
        #include "vertex.glsl"
        #include  "fragment.glsl"

        unsigned int vertex_shader;
        unsigned int fragment_shader;

        int buildVertexShader();
        int buildFragmentShader();
        int linkShaders(unsigned int* shaderProgram);
    private:
};

#endif