#include "EditorLayer.h"

#include <print>
#include <imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


namespace Jah {
	EditorLayer::EditorLayer()
		: Layer("Editor Layer"), m_CameraController(1600.0f / 900.0f, true), m_ParticleSystem(100000)
	{

	}

	void EditorLayer::OnAttach()
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

		Jah::FramebufferSpecification framebufferSpec;
		framebufferSpec.Width = Jah::Application::Get().GetWindow().GetWidth();
		framebufferSpec.Height = Jah::Application::Get().GetWindow().GetHeight();
		m_Framebuffer = std::make_shared<Jah::Framebuffer>(framebufferSpec);
	}

	void EditorLayer::OnDetach()
	{

	}

	void EditorLayer::OnUpdate(Jah::Timestep timestep)
	{
		m_FPS = 1.0f / timestep;

		static float rotation = 0.0f;
		rotation += timestep * 20.0f;

		if (m_ViewportFocused)
			m_CameraController.OnUpdate(timestep);

		Jah::Renderer2D::ResetStats();
		m_Framebuffer->Bind();

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

		Jah::Renderer2D::DrawRotatedQuad({ 0.0f, 0.0f }, { 1.0f, 1.0f }, rotation, m_GrassSprite);

		m_ParticleSystem.OnRender();

		Jah::Renderer2D::EndScene();
		m_Framebuffer->Unbind();
	}

	void EditorLayer::OnImGuiRender()
	{

		static bool dockspaceOpen = true;
		static bool opt_fullscreen = true;
		static bool opt_padding = false;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			const ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->WorkPos);
			ImGui::SetNextWindowSize(viewport->WorkSize);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}
		else
		{
			dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
		// and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		if (!opt_padding)
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
		if (!opt_padding)
			ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// Submit the DockSpace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				// Disabling fullscreen would allow the window to be moved to the front of other windows,
				// which we can't undo at the moment without finer window depth/z control.
				ImGui::MenuItem("Fullscreen", nullptr, &opt_fullscreen);
				ImGui::MenuItem("Padding", nullptr, &opt_padding);
				ImGui::Separator();

				if (ImGui::MenuItem("Exit"))
					Jah::Application::Get().Close();

				if (ImGui::MenuItem("Reset Layout"))
					ImGui::LoadIniSettingsFromMemory("");



				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

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


		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin("Viewport");

		m_ViewportFocused = ImGui::IsWindowFocused();
		m_ViewportHovered = ImGui::IsWindowHovered();
		Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportFocused || !m_ViewportHovered);

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		if ((m_ViewportSize.x != viewportPanelSize.x || m_ViewportSize.y != viewportPanelSize.y) && viewportPanelSize.x > 0 && viewportPanelSize.y > 0)
		{
			m_Framebuffer->Resize((uint32_t)viewportPanelSize.x, (uint32_t)viewportPanelSize.y);
			m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
				
			m_CameraController.OnResize(viewportPanelSize.x, viewportPanelSize.y);
		}
		
		uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
		ImGui::Image((void*)textureID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
		ImGui::End();
		ImGui::PopStyleVar();

		ImGui::End();
		

	}

	void EditorLayer::OnEvent(Jah::Event& event)
	{
		m_CameraController.OnEvent(event);
	}

}