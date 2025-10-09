#include "Core.h"

#include <GLFW/glfw3.h>
#include <iostream>

namespace Core {

	void PrintWelcome()
	{
		std::cout << "Welcome to Jah Engine" << std::endl;
		std::cin.get();
	}

	void InitGLFW()
	{
		if (!glfwInit())
		{
			std::cout << "Failed to initialize GLFW\n";
		}
		else
		{
			std::cout << "GLFW initialized successfully\n";
		}


		GLFWwindow* window = glfwCreateWindow(800, 600, "Test Window", nullptr, nullptr);
		if (window) {
			glfwMakeContextCurrent(window);
			while (!glfwWindowShouldClose(window)) {
				glfwSwapBuffers(window);
				glfwPollEvents();
			}
			glfwDestroyWindow(window);
		}
		glfwTerminate();

	}

}