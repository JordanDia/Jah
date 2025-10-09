#include "Application.h"
#include <iostream>


namespace Core {

	Application::Application()
	{
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback([this](Event& e) { OnEvent(e); });
	}

	Application::~Application()
	{

	}

	void Application::Run()
	{
		while (m_Running)
		{
			glClearColor(1, 0, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT);
			m_Window->OnUpdate();
		}
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);


		dispatcher.Dispatch<WindowResizeEvent>([this](WindowResizeEvent& event) {
			std::cout << event.ToString() << std::endl;
			return false;
		});

		dispatcher.Dispatch<WindowCloseEvent>([this](WindowCloseEvent& event) {
			std::cout << "Window closed" << std::endl;
			m_Running = false;
			return true;
		});

		dispatcher.Dispatch<KeyPressedEvent>([this](KeyPressedEvent& event) {
			std::cout << event.ToString() << std::endl;
			return false;
		});

		dispatcher.Dispatch<KeyReleasedEvent>([this](KeyReleasedEvent& event) {
			std::cout << event.ToString() << std::endl;
			return false;
		});

		dispatcher.Dispatch<MouseButtonPressedEvent>([this](MouseButtonPressedEvent& event) {
			std::cout << event.ToString() << std::endl;
			return false;
		});

		dispatcher.Dispatch<MouseButtonReleasedEvent>([this](MouseButtonReleasedEvent& event) {
			std::cout << event.ToString() << std::endl;
			return false;
		});

		dispatcher.Dispatch<MouseScrolledEvent>([this](MouseScrolledEvent& event) {
			std::cout << event.ToString() << std::endl;
			return false;
		});

		dispatcher.Dispatch<MouseMovedEvent>([this](MouseMovedEvent& event) {
			std::cout << event.ToString() << std::endl;
			return false;
		});

		


	}

}