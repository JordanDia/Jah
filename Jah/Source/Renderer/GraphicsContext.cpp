#include "jahpch.h"
#include "Renderer/GraphicsContext.h"
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

		JAH_CORE_INFO("OpenGL Info:");
		JAH_CORE_INFO("  Vendor: {0}", (const char*)glGetString(GL_VENDOR));
		JAH_CORE_INFO("  Renderer: {0}", (const char*)glGetString(GL_RENDERER));
		JAH_CORE_INFO("  Version: {0}", (const char*)glGetString(GL_VERSION));
	}

	void GraphicsContext::SwapBuffers()
	{
		glfwSwapBuffers(m_Window);
	}

}