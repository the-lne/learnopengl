#include "glad.h"
#include <GLFW/glfw3.h>
#define GLM_FORCE_CXX17 // sync glm and compiler versions
#include <glm/glm.hpp>
#include <iostream>

int main(int argc, char** argv)
{
	float mysupersin = glm::sin(3.14159/6);
	std::cout << mysupersin << std::endl;	
	return 0;
}
