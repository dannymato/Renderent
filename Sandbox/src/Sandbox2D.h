#pragma once

#include <Renderent.h>

#include <glm/glm.hpp>
#include <box2d/b2_world.h>

#include "Entities.h"

class Sandbox2D : public Renderent::Layer {

public:
	Sandbox2D();
	virtual ~Sandbox2D() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	virtual void OnUpdate(Renderent::Timestep ts) override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(Renderent::Event& e) override;

private:
	Renderent::Ref<Renderent::VertexArray> m_SquareVA;
	Renderent::OrthographicCameraController m_CameraController;
	Renderent::Ref<Renderent::Shader> m_FlatColorShader;

	glm::vec4 m_SquareColor = { 1.0f, 1.0f, 1.0f, 1.0f };

	Renderent::Ref<Renderent::Texture2D> m_Checkerboard;

	Player* m_Player;
	b2World* m_World;
	Floor* m_Floor;

	float m_CurrentTime = 0.0f;
};

