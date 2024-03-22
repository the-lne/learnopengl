#ifdef _MSC_BUILD // visual c++ compiler
#define __DYNAMIC_FUNC_NAME__ __FUNCSIG__
#elif __GNUC__
#define __DYNAMIC_FUNC_NAME__ __PRETTY_FUNCTION__
#elif 
#define __DYNAMIC_FUNC_NAME__ __func__
#endif

#define ERROR_LEAVE_FUNC ErrorLeave(__FILE__, __LINE__, __DYNAMIC_FUNC_NAME__)

#include "glad.h"
#include <GLFW/glfw3.h>
#define GLM_FORCE_CXX20 // sync glm and compiler versions
#include <glm/glm.hpp>
#include <iostream>
#include "shaders.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processEscapeKey(GLFWwindow *window);
void processWKey(GLFWwindow *window, int* current_polygon_mode);
void ErrorLeave(const char* file, int line, const char* function);

int main() 
{
    if (!glfwInit())
        ERROR_LEAVE_FUNC;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(800, 600, "myopengl", NULL, NULL);
    if (window == NULL)
        ERROR_LEAVE_FUNC;

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        ERROR_LEAVE_FUNC;

    Shaders shaders;

    if (shaders.buildVertexShader())
        ERROR_LEAVE_FUNC;

    if (shaders.buildFragmentShader())
        ERROR_LEAVE_FUNC;

    unsigned int shaderProgram;
    if (shaders.linkShaders(&shaderProgram))
        ERROR_LEAVE_FUNC;

    /**
     * so far its looking pretty clean and understandable
     * error checking will need to be fixed at some point but whatever.
     * 
     * 
     * before abstracting vao vbo and ebo into a class or their own seperate classes,
     * actually draw things and get an intuitive understanding of how they work together
     * 
     * 
     * in the future, make it multithreaded, and make a debouncer!
    */

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        -0.5f, -0.5f, 0.0f, // left  
         0.5f, -0.5f, 0.0f, // right 
         0.0f,  0.5f, 0.0f  // top   
    }; 

    // generate vertex array object wrapper first
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    // generate vertex buffer object (the actual data) now that its wrapper has been created
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    // now bind the buffer and set its attributes
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // here we tell opengl how to interpret the vertex data
    // here is an outline of the function input (it largely depends on the glsl)
    // 1 - the first variable tells the location of the variable we want to change (location = 0 for position in glsl)
    // 2 - the second variable is the size of the attribute (its a vec3)
    // 3 - the datatype (in glsl, a vec* is a float)
    // 4 - want to normalize data? No! I don't know what that is!
    // 5 - the stride is the space between consecutive vertex attributes
    // 6 - where the data begins in the buffer
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's 
    // bound vertex buffer object so afterwards we can safely unbind
    //glBindBuffer(GL_ARRAY_BUFFER, 0); 

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0); 


    // uncomment this call to draw in wireframe polygons.
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    int current_polygon_mode = 0;

    //Shaders shaders;
    //shaders.buildVertexShader();

    // render loop
    // -----------
    double tick = glfwGetTime();
    while (!glfwWindowShouldClose(window)) 
    {
        processEscapeKey(window);
        //while (!glfwWindowShouldClose(window))
        tick = glfwGetTime();
        //std::cout << tick << std::endl;
        //shaders->buildVertexShader();
        if (tick > 0.010)
        {
            //tick = glfwGetTime();
            glfwSetTime(0);
            // input
            // -----
            //processEscapeKey(window);
            processWKey(window, &current_polygon_mode);

            // render
            // ------
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            // draw our first triangle
            glUseProgram(shaderProgram);
            glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
            glDrawArrays(GL_TRIANGLES, 0, 3);
            // glBindVertexArray(0); // no need to unbind it every time 
 
            // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
            // -------------------------------------------------------------------------------
            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
    glfwTerminate();

    return 0;
}

void processEscapeKey(GLFWwindow *window) 
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void processWKey(GLFWwindow *window, int* current_polygon_mode) 
{
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {
            if (!(*current_polygon_mode))
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            else
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
    (*current_polygon_mode) = !(*current_polygon_mode);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) 
{
    glViewport(0, 0, width, height);
}

void ErrorLeave(const char* file, int line, const char* function) 
{
    std::cout << "Failure!" << std::endl;
    std::cout << "File: " << file << std::endl;
    std::cout << "Line: " << line << std::endl;
    std::cout<< "Function: " << function << std::endl;
    glfwTerminate();
    exit(0);
}