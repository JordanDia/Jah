#pragma once

#include <Jah.h>

#include <memory>
#include <glm/glm.hpp>

#include "ParticleSystem.h"

#include "Panels/SceneHierarchyPanel.h"
#include "Panels/ContentBrowserPanel.h"
#include "Renderer/EditorCamera.h"

namespace Jah {

	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		~EditorLayer() = default;

		void OnAttach() override;
		void OnDetach() override;

		void OnUpdate(Timestep timestep) override;
		void OnImGuiRender() override;
		void OnEvent(Event& event) override;

	private:
		bool OnKeyPressed(KeyPressedEvent& event);
		bool OnMouseButtonPressed(MouseButtonPressedEvent& event);
		

		void NewScene();
		void OpenScene();
		void OpenScene(const std::filesystem::path& path);
		void SaveScene();
		void SaveSceneAs();

		void SerializeScene(Shared<Scene> scene, const std::filesystem::path& path);

		void OnScenePlay();
		void OnSceneSimulate();
		void OnSceneStop();



		void OnOverlayRender();

		void DuplicateEntity();

		// UI Panels

		void UI_Toolbar();

		

	private:
		Shared<Scene> m_ActiveScene;
		Shared<Scene> m_EditorScene;
		std::filesystem::path m_EditorScenePath;

		// Cameras
		OrthographicCameraController m_CameraController;
		Entity m_CameraEntity;
		Entity m_SecondCamera;
		bool m_PrimaryCamera = true;

		Entity m_HoveredEntity;
		EditorCamera m_EditorCamera;

		Shared<Framebuffer> m_Framebuffer;

		// Paricle System
		glm::vec4 m_SquareColor = { 0.3f, 0.35f, 1.0f, 1.0f };
		ParticleProps m_ParticleProps;
		ParticleSystem m_ParticleSystem;

		
		int m_GizmoType = -1;

		// Physic Colliders
		bool m_ShowPhysicsColliders = false;
		glm::vec4 m_ColliderColor{ 0.0f, 1.0f, 0.0f, 1.0f };


		// Viewport
		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
		glm::vec2 m_ViewportBounds[2];
		bool m_ViewportFocused = false;
		bool m_ViewportHovered = false;

		float m_FPS = 0.0f;

		enum class SceneState
		{
			Edit, Play, Simulate
		};

		SceneState m_SceneState = SceneState::Edit;

		// Panels
		SceneHierarchyPanel m_SceneHierarchyPanel;
		ContentBrowserPanel m_ContentBrowserPanel;

		// Editor Resources
		Shared<Texture2D> m_IconPlay;
		Shared<Texture2D> m_IconSimulate;
		Shared<Texture2D> m_IconStop;
	};
}