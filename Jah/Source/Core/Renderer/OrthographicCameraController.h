#pragma once


#include "Camera.h"
#include "Core/Timestep.h"

#include "Core/Events/MouseEvent.h"
#include "Core/Events/ApplicationEvent.h"

namespace Jah {

	struct OrthographicCameraBounds
	{
		float Left, Right;
		float Bottom, Top;

		float GetWidth() { return Right - Left; }
		float GetHeight() { return Bottom - Top; }
	};

	class OrthographicCameraController
	{
	public:
		OrthographicCameraController(float aspectRatio, float rotation = 0.0f);

		void OnUpdate(Timestep timestep);
		void OnEvent(Event& e);

		inline OrthographicCamera& GetCamera() { return m_Camera; }
		inline const OrthographicCamera& GetCamera() const { return m_Camera; }

		float GetZoomLevel() const { return m_ZoomLevel; }
		float SetZoomLevel(float level) { m_ZoomLevel = level; }

		const OrthographicCameraBounds& GetBounds() const { return m_Bounds; }

	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);

	private:
		float m_AspectRatio = 1.6f / 0.9f;
		float m_ZoomLevel = 1.0f;
		OrthographicCamera m_Camera;
		OrthographicCameraBounds m_Bounds;

		bool m_Rotation = 0.0f;

		glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
		float m_CameraRotation = 0.0f;
		float m_CameraTranslationSpeed = 10.0f;
		float m_CameraRotationSpeed = 30.0f;
	};
	

}