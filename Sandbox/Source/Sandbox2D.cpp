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
	m_SpriteSheet = std::make_shared<Jah::Texture2D>("Assets/Textures/tilemap_packed.png");
	m_GrassSprite = Jah::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 0, 8 }, { 18, 18 });
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
	Jah::Renderer2D::DrawRotatedQuad({ 0.0f, 0.0f }, { 1.0f, 1.0f }, rotation, m_GrassSprite);
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
