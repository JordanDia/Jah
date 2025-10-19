#include <Jah.h>
#include <iostream>
#include <print>
#include <memory>

#include <imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class ExampleLayer : public Jah::Layer
{
public:
	ExampleLayer()
		: Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_SquarePosition(0.0f), m_TriangleColor(0.0f)
	{
		m_VertexArray = std::make_shared<Jah::VertexArray>();
		m_VertexArray->Bind();

		float vertices[7 * 3] = {
			-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f
		};

		Jah::Shared<Jah::VertexBuffer> vertexBuffer = std::make_shared<Jah::VertexBuffer>(vertices, sizeof(vertices));
		Jah::BufferLayout layout = {
			{ Jah::ShaderDataType::Float3, "a_Position" },
			{ Jah::ShaderDataType::Float3, "a_Position" },
			{ Jah::ShaderDataType::Float4, "a_Color" },
		};
		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		unsigned int indices[3] = {
			0, 1, 2
		};
		Jah::Shared<Jah::IndexBuffer> indexBuffer = std::make_shared<Jah::IndexBuffer>(indices, sizeof(indices) / sizeof(uint32_t));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		m_SquareVertexArray = std::make_shared<Jah::VertexArray>();

		float squareVertices[4 * 5] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
		};
		std::shared_ptr<Jah::VertexBuffer> squareVB = std::make_shared<Jah::VertexBuffer>(squareVertices, sizeof(squareVertices));
		Jah::BufferLayout squareLayout = {
			{ Jah::ShaderDataType::Float3, "a_Position" },
			{ Jah::ShaderDataType::Float2, "a_TexCoord" },
		};
		squareVB->SetLayout(squareLayout);
		m_SquareVertexArray->AddVertexBuffer(squareVB);

		unsigned int squareIndices[6] = {
			0, 1, 2,
			2, 3, 0
		};

		std::shared_ptr<Jah::IndexBuffer> squareIB = std::make_shared<Jah::IndexBuffer>(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
		m_SquareVertexArray->SetIndexBuffer(squareIB);

		auto shader = m_ShaderLibrary.Load("Assets/Shaders/Shader.glsl");		
		auto squareShader = m_ShaderLibrary.Load("Assets/Shaders/SquareShader.glsl");
		auto textureShader = m_ShaderLibrary.Load("Assets/Shaders/Texture.glsl");
		
		m_Texture = std::make_shared<Jah::Texture2D>("Assets/goku_pfp.jpg");
		textureShader->Bind();
		textureShader->UploadUniformInt("u_Texture", 0);
	}

	void OnUpdate(Jah::Timestep timestep) override
	{
		// Move Square
		if (Jah::Input::IsKeyPressed(JAH_KEY_A))
			m_SquarePosition.x -= m_SquareMoveSpeed * timestep;

		if (Jah::Input::IsKeyPressed(JAH_KEY_D))
			m_SquarePosition.x += m_SquareMoveSpeed * timestep;

		if (Jah::Input::IsKeyPressed(JAH_KEY_W))
			m_SquarePosition.y += m_SquareMoveSpeed * timestep;

		if (Jah::Input::IsKeyPressed(JAH_KEY_S))
			m_SquarePosition.y -= m_SquareMoveSpeed * timestep;

		// Move Camera
		if (Jah::Input::IsKeyPressed(JAH_KEY_J))
			m_CameraPosition.x -= m_CameraSpeed * timestep;

		if (Jah::Input::IsKeyPressed(JAH_KEY_L))
			m_CameraPosition.x += m_CameraSpeed * timestep;

		if (Jah::Input::IsKeyPressed(JAH_KEY_I))
			m_CameraPosition.y += m_CameraSpeed * timestep;

		if (Jah::Input::IsKeyPressed(JAH_KEY_K))
			m_CameraPosition.y -= m_CameraSpeed * timestep;

		// Space to rotate
		if (Jah::Input::IsKeyPressed(JAH_KEY_SPACE))
			m_CameraRotation += m_CameraRotationSpeed * timestep;

		Jah::Renderer::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Jah::Renderer::Clear();

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);

		Jah::Renderer::BeginScene(m_Camera);

		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		glm::vec4 greenColor(40 / 255.0f, 111 / 255.0f, 80 / 255.0f, 1.0f);
		glm::vec4 redColor(78 / 255.0f, 33 / 255.0f, 53 / 255.0f, 1.0f);
		glm::vec4 blueColor(78 / 255.0f, 100 / 255.0f, 255 / 255.0f, 1.0f);

		auto squareShader = m_ShaderLibrary.Get("SquareShader");

		for (int y = 0; y < 20; y++)
		{
			for (int x = 0; x < 20; x++)
			{
				glm::vec3 pos(0.11f * x, 0.11f * y, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				if ((x + y) % 2 == 0)
					squareShader->UploadUniformFloat4("u_Color", greenColor);
				else
					squareShader->UploadUniformFloat4("u_Color", redColor);

				Jah::Renderer::Submit(squareShader, m_SquareVertexArray, transform);
			}
		}

		squareShader->UploadUniformFloat4("u_Color", blueColor);
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_SquarePosition);

		auto textureShader = m_ShaderLibrary.Get("Texture");
		textureShader->Bind();

		Jah::Renderer::Submit(textureShader, m_SquareVertexArray, transform, m_Texture);

		Jah::Renderer::EndScene();
	}

	void OnImGuiRender() override
	{
		/*ImGui::Begin("Settings");
		ImGui::ColorEdit3("Triangle Color", glm::value_ptr(m_TriangleColor));
		ImGui::End();*/
	}

	void OnEvent(Jah::Event& event) override
	{
		
	}

private:
	Jah::ShaderLibrary m_ShaderLibrary;
	
	Jah::Shared<Jah::VertexArray> m_VertexArray;
	Jah::Shared<Jah::VertexArray> m_SquareVertexArray;
	
	Jah::Shared<Jah::Texture2D> m_Texture;

	Jah::OrthographicCamera m_Camera;
	glm::vec3 m_CameraPosition{ 0.0f, 0.0f, 0.0f };
	float m_CameraSpeed = 1.0f;
	float m_CameraRotation = 0.0f;
	float m_CameraRotationSpeed = 90.0f;

	glm::vec3 m_SquarePosition;
	float m_SquareMoveSpeed = 2.0f;
	glm::vec3 m_TriangleColor;
};

class App : public Jah::Application
{
public:
	App()
	{
		PushLayer(new ExampleLayer());
	}

	~App()
	{

	}
};

Jah::Application* Jah::CreateApplication()
{
	return new App();
}