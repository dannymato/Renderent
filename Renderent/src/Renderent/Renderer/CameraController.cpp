#include "repch.h"
#include "CameraController.h"

#include "Renderent/Core/Core.h"
#include "Renderent/Core/Input.h"
#include "Renderent/Core/KeyCodes.h"

namespace Renderent {

	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
	:m_AspectRatio(aspectRatio), m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel),
		m_Rotation(rotation)
	{
		
	}


	void OrthographicCameraController::OnUpdate(Timestep ts) {

		RE_PROFILE_FUNCTION();
		if (Input::IsKeyPressed(RE_KEY_A))
			m_CameraPosition.x -= m_CameraTranslationSpeed * ts;
		else if (Input::IsKeyPressed(RE_KEY_D))
			m_CameraPosition.x += m_CameraTranslationSpeed * ts;

		if (Input::IsKeyPressed(RE_KEY_W))
			m_CameraPosition.y += m_CameraTranslationSpeed * ts;
		else if (Input::IsKeyPressed(RE_KEY_S))
			m_CameraPosition.y -= m_CameraTranslationSpeed * ts;

		if (m_Rotation) {
			if (Input::IsKeyPressed(RE_KEY_Q))
				m_CameraRotation += m_CameraRotationSpeed * ts;
			else if (Renderent::Input::IsKeyPressed(RE_KEY_E))
				m_CameraRotation -= m_CameraRotationSpeed * ts;
			m_Camera.SetRotation(m_CameraRotation);
		}

		m_Camera.SetPosition(m_CameraPosition);

		m_CameraTranslationSpeed = m_ZoomLevel;
	}

	void OrthographicCameraController::OnEvent(Event& e) {
		RE_PROFILE_FUNCTION();
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(
			RE_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizedEvent>(
			RE_BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));

	}

	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e) {
		RE_PROFILE_FUNCTION();
		m_ZoomLevel -= e.GetYOffset() * 0.5f;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}

	bool OrthographicCameraController::OnWindowResized(WindowResizedEvent& e) {
		RE_PROFILE_FUNCTION();
		m_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}
}