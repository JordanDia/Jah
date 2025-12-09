#include "RuntimeLayer.h"

#include "Core/Log.h"
#include <imgui.h>

namespace Jah {

	RuntimeLayer::RuntimeLayer()
	{
		m_Scene = CreateShared<Scene>();
		auto filepath = std::filesystem::absolute("Assets/Scenes/Physics2D.jah");
		LoadScene(filepath);

		FramebufferSpecification framebufferSpec;
		framebufferSpec.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::Depth };
		framebufferSpec.Width = Application::Get().GetWindow().GetWidth();
		framebufferSpec.Height = Application::Get().GetWindow().GetHeight();
		m_Framebuffer = CreateShared<Framebuffer>(framebufferSpec);

		m_FramebufferShader = CreateShared<Shader>("Assets/Shaders/Framebuffer.glsl");

		m_FrameBufferTexture = CreateShared<Texture2D>(m_Framebuffer->GetColorAttachmentRendererID(), framebufferSpec.Width, framebufferSpec.Height);

	}

	void RuntimeLayer::OnAttach()
	{
		float quadVertices[] = {
			// positions      // tex coords
			-1.0f, -1.0f,     0.0f, 0.0f,
			 1.0f, -1.0f,     1.0f, 0.0f,
			 1.0f,  1.0f,     1.0f, 1.0f,
			-1.0f,  1.0f,     0.0f, 1.0f
		};

		uint32_t quadIndices[] = {
			0, 1, 2,
			2, 3, 0
		};

		m_FullscreenQuadVA = CreateShared<VertexArray>();
		auto vb = CreateShared<VertexBuffer>(quadVertices, sizeof(quadVertices));
		vb->SetLayout({
			{ ShaderDataType::Float2, "a_Position" },
			{ ShaderDataType::Float2, "a_TexCoord" }
			});
		m_FullscreenQuadVA->AddVertexBuffer(vb);

		auto ib = CreateShared<IndexBuffer>(quadIndices, 6);
		m_FullscreenQuadVA->SetIndexBuffer(ib);


		m_Scene->OnRuntimeStart();
	}

	void RuntimeLayer::OnDetach()
	{
		m_Scene->OnRuntimeStop();
	}

	void RuntimeLayer::OnUpdate(Timestep timestep)
	{

		if (FramebufferSpecification spec = m_Framebuffer->GetSpecification();
			m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f &&
			(spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
		{
			m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);

			m_FrameBufferTexture->SetRendererID(m_Framebuffer->GetColorAttachmentRendererID());
			m_FrameBufferTexture->SetSize(m_ViewportSize.x, m_ViewportSize.y);

			if (m_Scene)
				m_Scene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		}

		m_Framebuffer->Bind();

		Renderer::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Renderer::Clear();

		// Clear our entity ID attachment to -1
		m_Framebuffer->ClearAttachment(1, -1);

		m_Scene->OnUpdateRuntime(timestep);
		
		m_Framebuffer->Unbind();


		float width = Application::Get().GetWindow().GetWidth();
		float height = Application::Get().GetWindow().GetHeight();
		float aspectRatio = (float)width / (float)height;
		float orthoWidth = 16.0f; // for example
		float orthoHeight = orthoWidth / aspectRatio;

		
		auto transform = glm::mat4(1.0f);

		Renderer::BeginScene(glm::mat4(1.0f));
		Renderer::Submit(m_FramebufferShader, m_FullscreenQuadVA, transform, m_FrameBufferTexture);
		Renderer::EndScene();

	}

	void RuntimeLayer::OnImGuiRender()
	{
		static bool dockspaceOpen = false;
		static bool opt_fullscreen = true;
		static bool opt_padding = false;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_AutoHideTabBar;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoMove;
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
		ImGuiStyle& style = ImGui::GetStyle();
		float minWinSizeX = style.WindowMinSize.x;
		style.WindowMinSize.x = 200.0f;
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		style.WindowMinSize.x = minWinSizeX;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin("Viewport"); // Viewport

		auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
		auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
		auto viewportOffset = ImGui::GetWindowPos(); // Includes tab bar

		m_ViewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
		m_ViewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

		uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
		ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

		ImGui::End(); // Viewport
		ImGui::PopStyleVar();


		ImGui::End();
	
	}

	void RuntimeLayer::OnEvent(Event& event)
	{}

	void RuntimeLayer::LoadScene(const std::filesystem::path& filepath)
	{
		SceneSerializer serializer(m_Scene);
		serializer.Deserialize(filepath.string());
	}

}