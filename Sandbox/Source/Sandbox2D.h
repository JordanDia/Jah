#pragma once

#include <Jah.h>

#include <memory>
#include <glm/glm.hpp>

#include "ParticleSystem.h"


class Sandbox2D : public Jah::Layer
{
public:
	Sandbox2D();
	~Sandbox2D() = default;

	void OnAttach() override;
	void OnDetach() override;

	void OnUpdate(Jah::Timestep timestep) override;
	void OnImGuiRender() override;
	void OnEvent(Jah::Event& event) override;
private:
	Jah::OrthographicCameraController m_CameraController;

	// Temporary
	Jah::Shared<Jah::VertexArray> m_SquareVertexArray;
	Jah::Shared<Jah::Shader> m_Shader;
	Jah::Shared<Jah::Texture2D> m_GokuTexture;

	glm::vec3 m_SquareColor = { 0.3f, 0.35f, 1.0f };
	Jah::ParticleProps m_ParticleProps;
	Jah::ParticleSystem m_ParticleSystem;

};