#include "Core/Renderer/GraphicsContext.h"
#include "Core/Core.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Jah {

	GraphicsContext::GraphicsContext(GLFWwindow* window)
		: m_Window(window)
	{
		JAH_ASSERT(window, "Window is NULL!");
	}

	void GraphicsContext::Init()
	{
		glfwMakeContextCurrent(m_Window);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		JAH_ASSERT(status, "Failed to initialize Glad!")
	}

	void GraphicsContext::SwapBuffers()
	{
		glfwSwapBuffers(m_Window);
	}

}