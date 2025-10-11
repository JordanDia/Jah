#include "Core/Input.h"
#include "Core/Application.h"
#include <GLFW/glfw3.h>

namespace Jah {



	bool Input::IsKeyPressed(int keycode)
	{
		Application& app = Application::Get();
		GLFWwindow* window = app.GetWindow().GetNativeWindow();
		bool state = glfwGetKey(window, keycode);

		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool Input::IsMouseButtonPressed(int button)
	{
		Application& app = Application::Get();
		GLFWwindow* window = app.GetWindow().GetNativeWindow();
		bool state = glfwGetMouseButton(window, button);

		return state == GLFW_PRESS;
	}

	std::pair<float, float> Input::GetMousePosition()
	{
		Application& app = Application::Get();
		GLFWwindow* window = app.GetWindow().GetNativeWindow();

		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		return { (float)xpos, (float)ypos };
	}

	float Input::GetMouseX()
	{
		auto [x, y] = GetMousePosition();
		return x;
	}

	float Input::GetMouseY()
	{
		auto [x, y] = GetMousePosition();
		return y;
	}

}