#pragma once

#include "Renderent/Core/TimeStep.h"
#include "Renderent/Event/MouseEvent.h"
#include "Renderent/Event/WindowEvent.h"
#include "Renderent/Renderer/Camera.h"

namespace Renderent {

	struct OrthographicCameraBounds {
		float Left, Right;
		float Top, Bottom;

		float GetWidth() { return Right - Left; }
		float GetHeight() { return Top - Bottom; }
	};

	class OrthographicCameraController {


	public:
		OrthographicCameraController(float aspectRatio, bool rotation = false);

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		OrthographicCamera& GetCamera() { return m_Camera; }
		const OrthographicCamera& GetCamera() const { return m_Camera; }
		void SetZoomLevel(float level) { m_ZoomLevel = level; CalculateView(); }
		float GetZoomLevel() const { return m_ZoomLevel; }
		
		const OrthographicCameraBounds& GetBounds() const { return m_Bounds; }

		void ChangeAspectRatio(float aspectRatio) { m_AspectRatio = aspectRatio; CalculateView(); }

	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizedEvent& e);

		void CalculateView();
	private:
		float m_AspectRatio;
		float m_ZoomLevel = 1.0f;
		OrthographicCameraBounds m_Bounds;
		OrthographicCamera m_Camera;

		bool m_Rotation;
		glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
		float m_CameraRotation = 0.0;
		float m_CameraTranslationSpeed = 1.0f;
		float m_CameraRotationSpeed = 180.0f;
	};

}