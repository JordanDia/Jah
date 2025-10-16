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

		m_VertexArray = std::make_shared<VertexArray>();
		m_VertexArray->Bind();

		float vertices[7 * 3] = {
			-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f
		};

		m_VertexBuffer = std::make_shared<VertexBuffer>(vertices, sizeof(vertices));
		BufferLayout layout = {
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Color" },
		};
		m_VertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(m_VertexBuffer);

		unsigned int indices[3] = {
			0, 1, 2
		};
		m_IndexBuffer = std::make_shared<IndexBuffer>(indices, sizeof(indices) / sizeof(uint32_t));
		m_VertexArray->SetIndexBuffer(m_IndexBuffer);

		m_SquareVertexArray = std::make_shared<VertexArray>();

		float squareVertices[4 * 3] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f,
		};
		std::shared_ptr<VertexBuffer> squareVB = std::make_shared<VertexBuffer>(squareVertices, sizeof(squareVertices));
		BufferLayout squareLayout = {
			{ ShaderDataType::Float3, "a_Position" },
		};
		squareVB->SetLayout(squareLayout);
		m_SquareVertexArray->AddVertexBuffer(squareVB);

		unsigned int squareIndices[6] = {
			0, 1, 2,
			2, 3, 0
		};

		std::shared_ptr<IndexBuffer> squareIB = std::make_shared<IndexBuffer>(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
		m_SquareVertexArray->SetIndexBuffer(squareIB);

		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = vec4(a_Position, 1.0);

			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			in vec4 v_Color;

			void main()
			{
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
				color = v_Color;
			}
		)";

		m_Shader.reset(new Shader(vertexSrc, fragmentSrc));

		std::string squareVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;

			out vec3 v_Position;

			void main()
			{
				v_Position = a_Position;
				gl_Position = vec4(a_Position, 1.0);

			}
		)";

		std::string squareFragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;

			void main()
			{
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
			}
		)";

		m_SquareShader.reset(new Shader(vertexSrc, fragmentSrc));
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