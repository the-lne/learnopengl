#include "glad.h"
#include <GLFW/glfw3.h>
#define GLM_FORCE_CXX20 // sync glm and compiler standards
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "shaders.h"
#include "logging.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processEscapeKey(GLFWwindow *window);
void processWKey(GLFWwindow *window, int* current_polygon_mode);

int main() 
{
    if (!glfwInit())
        LOG_ERROR;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(800, 600, "myopengl", NULL, NULL);
    if (!window)
        LOG_ERROR;

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        LOG_ERROR;

    Shaders shaders;

    if (shaders.buildVertexShader())
        LOG_ERROR;

    if (shaders.buildFragmentShader())
        LOG_ERROR;

    unsigned int shaderProgram;
    if (shaders.linkShaders(&shaderProgram))
        LOG_ERROR;

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
         0.0f,  0.5f, 0.0f,  // top   
        // second triangle
         0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f   // top left
    }; 

    unsigned int vertices[] = {

    };

/*
    float vertices[] = {
        // first triangle
         0.5f,  0.5f, 0.0f,  // top right
         0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f,  0.5f, 0.0f,  // top left 
        // second triangle
         0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f   // top left
    }; 
    */

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