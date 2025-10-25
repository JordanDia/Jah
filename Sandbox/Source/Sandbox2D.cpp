#include "Sandbox2D.h"

#include <imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_CameraController(1600.0f / 900.0f, true)
{

}

void Sandbox2D::OnAttach()
{
	m_GokuTexture = std::make_shared<Jah::Texture2D>("Assets/goku_pfp.jpg");
}

void Sandbox2D::OnDetach()
{

}

void Sandbox2D::OnUpdate(Jah::Timestep timestep)
{
	static float rotation = 0.0f;
	rotation += timestep * 20.0f;

	m_CameraController.OnUpdate(timestep);

	Jah::Renderer2D::ResetStats();

	Jah::Renderer::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	Jah::Renderer::Clear();

	Jah::Renderer2D::BeginScene(m_CameraController.GetCamera());

	Jah::Renderer2D::DrawQuad({ 0.0f, 0.0f }, { 1.0f, 1.0f }, m_GokuTexture);
	Jah::Renderer2D::DrawQuad({ -0.5f, 0.4f }, { 0.5f, 0.5f }, { 0.3f, 0.35f, 0.8f, 1.0f });
	Jah::Renderer2D::DrawRotatedQuad({ 0.8f, 0.8f }, { 0.5f, 0.5f }, 45.0f, { 0.3f, 0.35f, 0.8f, 1.0f });

	Jah::Renderer2D::DrawRotatedQuad({ 0.0f, 0.0f }, { 1.0f, 1.0f }, rotation, m_GokuTexture);

	Jah::Renderer2D::EndScene();


	Jah::Renderer2D::BeginScene(m_CameraController.GetCamera());

	for (float y = -5.0f; y < 5.0f; y += 0.1f)
	{
		for (float x = -5.0f; x < 5.0f; x += 0.1f)
		{
			glm::vec4 color = { (x + 5.0f) / 10, (y + 5.0f) / 10, 0.0f, 0.5f };
			Jah::Renderer2D::DrawQuad({ x, y }, { 0.45f, 0.45f }, color);
		}
	}

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

	ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::End();
}

void Sandbox2D::OnEvent(Jah::Event& event)
{
	m_CameraController.OnEvent(event);
}
