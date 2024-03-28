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

int main(void) 
{
    if (!glfwInit())
        LOG_ERROR;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(800, 600, "my opengl", NULL, NULL);
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



    // plot da pointzz
    float vertices[] = { 
        -1.0f, 0.0f, 0.0f,
        0.0f, 1.0f,  0.0f,
        1.0f, 0.0f, 0.0f, 
        0.0f, -1.0f, 0.0f
    };

    unsigned int indices[] = {
        0, 1, 2,
        0, 2, 3
    };




    // memory time!
    unsigned int VAO, VBO, EBO;

    // generate the buffers
    glGenVertexArrays(1, &VAO); // you can also use glGenVertexArray(&var) to generate just one vertex array
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // attach the shit to the thingy so it can do stuff
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    // copy the data!
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);






    // here we tell opengl how to interpret the vertex data
    // here is an outline of the function input (it largely depends on the glsl)
    // 1 - the first variable tells the location of the variable we want to change (location = 0 for position in glsl)
    // 2 - the second variable is the size of the attribute (its a vec3 its size is 3)
    // 3 - the datatype (in glsl, a vec* is a float)
    // 4 - want to normalize data? No! I don't know what that is!
    // 5 - the stride is the space between consecutive vertex attributes
    // 6 - where the data begins in the buffer
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);



    // uncomment this call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    int current_polygon_mode = 0;




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
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); 
            // glBindVertexArray(0); // no need to unbind it every time // apparently, glBind...() unbinds buffers?
 
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

void framebuffer_size_callback(GLFWwindow* window, int width, int height) { glViewport(0, 0, width, height); }
