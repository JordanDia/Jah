#pragma once

struct GLFWwindow;

namespace Jah {

	class GraphicsContext
	{
	public:
		GraphicsContext(GLFWwindow* window);

		void Init();
		void SwapBuffers();

	private:
		GLFWwindow* m_Window;
	};

}