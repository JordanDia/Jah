#include "Sandbox2D.h"

#include <imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#

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

	m_CameraController.OnUpdate(timestep);

	Jah::Renderer::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	Jah::Renderer::Clear();

	Jah::Renderer2D::BeginScene(m_CameraController.GetCamera());

	Jah::Renderer2D::DrawQuad({ 0.0f, 0.0f }, { 1.0f, 1.0f }, m_GokuTexture);
	Jah::Renderer2D::DrawQuad({ -0.3f, -1.0f }, { 0.5f, 0.5f }, { 0.3f, 0.35f, 0.8f, 1.0f });

	Jah::Renderer2D::EndScene();
}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::End();
}

void Sandbox2D::OnEvent(Jah::Event& event)
{

}
