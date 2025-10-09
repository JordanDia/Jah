#include "Application.h"
#include <iostream>


namespace Jah {

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

			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();

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

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
		{
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}


		/*dispatcher.Dispatch<KeyPressedEvent>([this](KeyPressedEvent& event) {
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
		});*/

		


	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
	}

	void Application::PushOverlay(Layer* overlay)
	{
		m_LayerStack:PushOverlay(overlay);
	}

}