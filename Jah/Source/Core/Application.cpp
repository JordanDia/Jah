#include "Application.h"
#include "Core.h"
#include <iostream>


#include <glad/glad.h>
#include <glfw/glfw3.h>
#include "Core/Renderer/Renderer.h"



namespace Jah {

	Application* Application::s_Instance = nullptr;

	Application::Application()
	{

		JAH_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback([this](Event& e) { OnEvent(e); });

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);	
	}

	Application::~Application()
	{

	}

	void Application::Run()
	{
		while (m_Running)
		{
			float time = (float)glfwGetTime();
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			for (Layer* layer : m_LayerStack)
				layer->OnUpdate(timestep);

			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();
			m_ImGuiLayer->End();

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
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* overlay)
	{
		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}

}