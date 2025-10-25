#include "OrthographicCameraController.h"

#include "Core/Input.h"
#include "Core/KeyCodes.h"

namespace Jah {

	OrthographicCameraController::OrthographicCameraController(float aspectRatio, float rotation)
		: m_AspectRatio(aspectRatio), m_Rotation(rotation), m_Bounds({ -m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel }), m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio* m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel)
	{
	}

	void OrthographicCameraController::OnUpdate(Timestep timestep)
	{
		if (Input::IsKeyPressed(JAH_KEY_J))
			m_CameraPosition.x -= m_CameraTranslationSpeed * timestep;

		if (Input::IsKeyPressed(JAH_KEY_L))
			m_CameraPosition.x += m_CameraTranslationSpeed * timestep;

		if (Input::IsKeyPressed(JAH_KEY_I))
			m_CameraPosition.y += m_CameraTranslationSpeed * timestep;

		if (Input::IsKeyPressed(JAH_KEY_K))
			m_CameraPosition.y -= m_CameraTranslationSpeed * timestep;

		// Space to rotate
		if (m_Rotation)
		{
			if (Input::IsKeyPressed(JAH_KEY_Q))
				m_CameraRotation += m_CameraRotationSpeed * timestep;
			if (Input::IsKeyPressed(JAH_KEY_E))
				m_CameraRotation -= m_CameraRotationSpeed * timestep;

			m_Camera.SetRotation(m_CameraRotation);
		}

		m_Camera.SetPosition(m_CameraPosition);

		m_CameraTranslationSpeed = m_ZoomLevel;
	}

	void OrthographicCameraController::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);

		dispatcher.Dispatch<MouseScrolledEvent>([this](MouseScrolledEvent& event) {
			OnMouseScrolled(event);
			return false;
		});

		dispatcher.Dispatch<WindowResizeEvent>([this](WindowResizeEvent& event) {
			OnWindowResized(event);
			return false;
		});
	}

	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		m_ZoomLevel -= e.GetYOffset() * 0.25f;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
		m_Bounds = { -m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel };
		m_Camera.SetProjection(m_Bounds.Left, m_Bounds.Right, m_Bounds.Bottom, m_Bounds.Top);
		return false;
	}

	bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& e)
	{
		m_ZoomLevel = (float)e.GetWidth() / (float)e.GetHeight();
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
		m_Bounds = { -m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel };
		m_Camera.SetProjection(m_Bounds.Left, m_Bounds.Right, m_Bounds.Bottom, m_Bounds.Top);
		return false;
	}

}