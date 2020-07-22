#pragma once

#include <Renderent.h>

#include <glm/glm.hpp>

namespace Renderent {

	class EditorLayer : public Layer {

	public:
		EditorLayer();
		virtual ~EditorLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		virtual void OnUpdate(Timestep ts) override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& e) override;

	private:
		Ref<VertexArray> m_SquareVA;
		OrthographicCameraController m_CameraController;
		Ref<Shader> m_FlatColorShader;

		glm::vec4 m_SquareColor = { 1.0f, 1.0f, 1.0f, 1.0f };

		Ref<Texture2D> m_Checkerboard;
		Ref<Texture2D> m_Spritesheet;

		Ref<SubTexture2D> m_TextureStairs, m_TextureBarrel, m_TextureTree, m_TextureGround;

		Ref<Framebuffer> m_Framebuffer;

		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };

		float m_CurrentTime = 0.0f;
	};

}

