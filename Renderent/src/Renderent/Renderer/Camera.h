#pragma once

#include <glm/glm.hpp>


namespace Renderent {
	class OrthographicCamera
	{
	public:

		OrthographicCamera(float left, float right, float bottom, float top);
		
		void SetProjection(float left, float right, float bottom, float top);

		void SetPosition(const glm::vec3& position) {
			m_Position = position;
			RecalculateViewMatrix();
		}
		void SetRotation(float rotation) {
			m_Rotation = rotation;
			RecalculateViewMatrix();
		}


		const glm::vec3& GetPosition() const { return m_Position; }
		float GetRotation(float rotation) { return m_Rotation; }

		const glm::mat4& GetViewTransform() const { return m_ViewMatrix; }
		const glm::mat4& GetProjectionTransform() const { return m_ProjectionMatrix; }
		const glm::mat4& GetViewProjectionMatrix() const { return m_ProjectionViewMatrix; }

	private:
		void RecalculateViewMatrix();

	private:
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ProjectionViewMatrix;

		glm::vec3 m_Position = { 0.0, 0.0f, 0.0f };
		float m_Rotation = 0.0f;

	};


}

