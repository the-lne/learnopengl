#ifndef __SHADERS_H__
#define __SHADERS_H__

class Shaders {
    public:

        const char *vertex_shader_source = 
            "#version 330 core\n"
            "layout (location = 0) in vec3 aPos;\n"
            "void main()\n"
            "{\n"
            "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
            "}\0";
        const char *fragment_shader_source = 
            "#version 330 core\n"
            "out vec4 FragColor;\n"
            "void main()\n"
            "{\n"
            "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
            "}\n\0";

        unsigned int vertex_shader;
        unsigned int fragment_shader;

        int buildVertexShader(void);
        int buildFragmentShader(void);
        int linkShaders(unsigned int* shaderProgram);
};

#endif