#pragma once

#include <Jah.h>

#include <memory>
#include <glm/glm.hpp>

#include "ParticleSystem.h"

namespace Jah {

	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		~EditorLayer() = default;

		void OnAttach() override;
		void OnDetach() override;

		void OnUpdate(Jah::Timestep timestep) override;
		void OnImGuiRender() override;
		void OnEvent(Jah::Event& event) override;
	private:
		Jah::OrthographicCameraController m_CameraController;

		Jah::Shared<Jah::Framebuffer> m_Framebuffer;

		// Temporary
		Jah::Shared<Jah::VertexArray> m_SquareVertexArray;
		Jah::Shared<Jah::Shader> m_Shader;
		Jah::Shared<Jah::Texture2D> m_GokuTexture;
		Jah::Shared<Jah::Texture2D> m_SpriteSheet;
		Jah::Shared<Jah::SubTexture2D> m_GrassSprite;

		glm::vec4 m_SquareColor = { 0.3f, 0.35f, 1.0f, 1.0f };
		Jah::ParticleProps m_ParticleProps;
		Jah::ParticleSystem m_ParticleSystem;

		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };

		bool m_ViewportFocused = false;
		bool m_ViewportHovered = false;

		float m_FPS = 0.0f;

	};

}