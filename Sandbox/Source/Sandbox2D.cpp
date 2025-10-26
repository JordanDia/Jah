#include "Sandbox2D.h"

#include <imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_CameraController(1600.0f / 900.0f, true), m_ParticleSystem(100000)
{

}

void Sandbox2D::OnAttach()
{
	m_GokuTexture = std::make_shared<Jah::Texture2D>("Assets/Textures/goku_pfp.jpg");
	m_SpriteSheet = std::make_shared<Jah::Texture2D>("Assets/Textures/tilemap_packed.png");

	m_GrassSprite = Jah::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 0, 8 }, { 18, 18 });
	m_SquareColor = { 0.0f, 0.4f, 1.0f, 1.0f };


	m_ParticleProps.ColorBegin = { 0.0f, 0.4f, 1.0f, 1.0f };
	m_ParticleProps.ColorEnd = { 0.1f, 0.2f, 0.5f, 0.0f };
	m_ParticleProps.SizeBegin = 0.5f;
	m_ParticleProps.SizeVariation = 0.3f;
	m_ParticleProps.SizeEnd = 0.0f;
	m_ParticleProps.Lifetime = 5.0f;
	m_ParticleProps.Velocity = { 0.0f, 1.0f };
	m_ParticleProps.VelocityVariation = { 3.0f, 1.0f };
	m_ParticleProps.Position = { 0.0f, 0.0f };
}

void Sandbox2D::OnDetach()
{

}

void Sandbox2D::OnUpdate(Jah::Timestep timestep)
{
	m_FPS = 1.0f / timestep;

	static float rotation = 0.0f;
	rotation += timestep * 20.0f;

	m_CameraController.OnUpdate(timestep);

	Jah::Renderer2D::ResetStats();

	Jah::Renderer::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	Jah::Renderer::Clear();

	Jah::Renderer2D::BeginScene(m_CameraController.GetCamera());

#if 0
	//Jah::Renderer2D::DrawQuad({ 0.0f, 0.0f }, { 1.0f, 1.0f }, m_GokuTexture);
	Jah::Renderer2D::DrawQuad({ -0.5f, 0.4f }, { 0.5f, 0.5f }, { 0.3f, 0.35f, 0.8f, 1.0f });
	Jah::Renderer2D::DrawRotatedQuad({ 0.8f, 0.8f }, { 0.5f, 0.5f }, 45.0f, { 0.3f, 0.35f, 0.8f, 1.0f });

	Jah::Renderer2D::DrawRotatedQuad({ 0.0f, 0.0f }, { 1.0f, 1.0f }, rotation, m_GokuTexture);

	for (float y = -5.0f; y < 5.0f; y += 0.5f)
	{
		for (float x = -5.0f; x < 5.0f; x += 0.5f)
		{
			glm::vec4 color = { (x + 5.0f) / 10, (y + 5.0f) / 10, 0.0f, 0.5f };
			Jah::Renderer2D::DrawQuad({ x, y }, { 0.45f, 0.45f }, color);
		}
	}

#endif

	if (Jah::Input::IsMouseButtonPressed(JAH_MOUSE_BUTTON_1))
	{

		auto [x, y] = Jah::Input::GetMousePosition();
		auto width = Jah::Application::Get().GetWindow().GetWidth();
		auto height = Jah::Application::Get().GetWindow().GetHeight();

		auto bounds = m_CameraController.GetBounds();
		x = (x / width) * bounds.GetWidth() - bounds.GetWidth() * 0.5f;
		y = (y / height) * bounds.GetHeight() - bounds.GetHeight() * 0.5f;

		// Apply camera offset so mouse world position follows camera
		x += m_CameraController.GetCamera().GetPosition().x;
		y += m_CameraController.GetCamera().GetPosition().y;

		m_ParticleProps.Position = { x, y };

		for (int i = 0; i < 5; i++)
			m_ParticleSystem.Emit(m_ParticleProps);
	}

	m_ParticleProps.ColorBegin = m_SquareColor;

	m_ParticleSystem.OnUpdate(timestep);


	for (float x = -5.0f; x < 5.0f; x += 0.5f)
	{
		Jah::Renderer2D::DrawQuad({ x, -0.3f }, { 0.5f, 0.5f }, m_GrassSprite);
	}


	Jah::Renderer2D::DrawRotatedQuad({ 0.0f, 0.0f }, { 1.0f, 1.0f }, rotation, m_GrassSprite);

	
	m_ParticleSystem.OnRender();
	Jah::Renderer2D::EndScene();
}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Settings");

	auto& stats = Jah::Renderer2D::GetStats();

	ImGui::Text("Renderer2D Stats");
	ImGui::Text("Draw Calls: %d", stats.DrawCalls);
	ImGui::Text("Quads: %d", stats.QuadCount);
	ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
	ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
	ImGui::Text("FPS: %.1f", m_FPS);

	ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::End();
}

void Sandbox2D::OnEvent(Jah::Event& event)
{
	m_CameraController.OnEvent(event);
}
