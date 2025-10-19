#pragma once


#include "Camera.h"
#include "Core/Timestep.h"

#include "Core/Events/MouseEvent.h"
#include "Core/Events/ApplicationEvent.h"

namespace Jah {

	class OrthographicCameraController
	{
	public:
		OrthographicCameraController(float aspectRatio, float rotation = 0.0f);

		void OnUpdate(Timestep timestep);
		void OnEvent(Event& e);

		inline OrthographicCamera& GetCamera() { return m_Camera; }
		inline const OrthographicCamera& GetCamera() const { return m_Camera; }

	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);

	private:
		float m_AspectRatio = 1.6f / 0.9f;
		float m_ZoomLevel = 1.0f;
		OrthographicCamera m_Camera;

		bool m_Rotation = 0.0f;
		glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
		float m_CameraRotation = 0.0f;
		float m_CameraTranslationSpeed = 10.0f;
		float m_CameraRotationSpeed = 30.0f;
	};
	

}