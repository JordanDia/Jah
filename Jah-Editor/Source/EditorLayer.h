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
		Shared<Scene> m_ActiveScene;
		OrthographicCameraController m_CameraController;

		Shared<Jah::Framebuffer> m_Framebuffer;

		// Temporary
		Shared<Jah::VertexArray> m_SquareVertexArray;
		Shared<Jah::Shader> m_Shader;
		Shared<Jah::Texture2D> m_GokuTexture;
		Shared<Jah::Texture2D> m_SpriteSheet;
		Shared<Jah::SubTexture2D> m_GrassSprite;

		glm::vec4 m_SquareColor = { 0.3f, 0.35f, 1.0f, 1.0f };
		ParticleProps m_ParticleProps;
		ParticleSystem m_ParticleSystem;

		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };

		bool m_ViewportFocused = false;
		bool m_ViewportHovered = false;

		float m_FPS = 0.0f;

	};

}