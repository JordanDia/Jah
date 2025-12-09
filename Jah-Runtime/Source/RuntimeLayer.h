#pragma once

#include <Jah.h>

namespace Jah {

	class RuntimeLayer : public Layer
	{
	public:

		RuntimeLayer();

		void OnAttach() override;
		void OnDetach() override;

		void OnUpdate(Timestep timestep) override;
		void OnImGuiRender() override;
		void OnEvent(Event& event) override;

	private:
		void LoadScene(const std::filesystem::path& filepath);

	private:
		Shared<Scene> m_Scene = nullptr;

		Shared<Framebuffer> m_Framebuffer;
		Shared<Shader> m_FramebufferShader;
		Shared<Texture2D> m_FrameBufferTexture;

		Shared<VertexArray> m_FullscreenQuadVA;

		// Viewport
		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
		glm::vec2 m_ViewportBounds[2];

	};
}