/** MAIN.CC
 * I guess just comment every single line and try your best to understand what the fuck is happening
 */

#include "glad.h"
#include <GLFW/glfw3.h>
#define GLM_FORCE_CXX17 // sync glm and compiler versions
#include <glm/glm.hpp>
#include <iostream>

int main(int argc, char** argv)
{
	float mysupersin = glm::sin(3.14159/6);
	std::cout << mysupersin << std::endl;	


    glfwInit(); // inits gl functino wrangler
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // this function is here to configure glfw
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // so what is happening with glfwWindowHint is we are taking a variable such as GLFW_CONTENXT_VERSION_MINOR and giving it a value such as 3
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	

	// now that glfw is initialized and configured, its time to make a window
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);


	//glad manages function pointers for opengl, so now its time to get glad up and running
	
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}    
	

	// now would be a good time to define the viewport 
	glViewport(0, 0, 800, 600);

	return 0;
}
