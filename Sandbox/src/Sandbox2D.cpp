#include "Sandbox2D.h"

#include <chrono>

#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>


Sandbox2D::Sandbox2D() : Layer("Sandbox2D"), m_CameraController(1920.0f / 1080.0f, true){


	m_World = new b2World({ 0.0f, -10.0f });
	m_Player = new Player({ 0.0f, 2.0f }, { 0.5f, 1.0f }, { 0.8f, 0.2f, 0.8f, 1.0f }, * m_World);
	m_Floor = new Floor({ 0.0f, -3.0f }, { 10.0f, 5.0f }, { 2.0f, 0.8f, 0.7f, 1.0f }, *m_World);

}

void Sandbox2D::OnAttach() {

	m_Checkerboard = Renderent::Texture2D::Create("assets/textures/Checkerboard.png");

}
void Sandbox2D::OnDetach() {

}



void Sandbox2D::OnUpdate(Renderent::Timestep ts) {

	RE_PROFILE_FUNCTION();

	{
		RE_PROFILE_SCOPE("CameraController::OnUpdate");
		m_CameraController.OnUpdate(ts);
	}
	{
		RE_PROFILE_SCOPE("Player Movement");
		m_Player->processInput(ts);
	}
	{
		RE_PROFILE_SCOPE("Physics Calculations");
		m_World->Step(ts, 8, 3);
	}
	{
		RE_PROFILE_SCOPE("Renderer Prep");
		Renderent::RenderCommand::SetClearColor({ 0.2f, 0.2f, 0.2f, 1.0f });
		Renderent::RenderCommand::Clear();

	}

	{
		RE_PROFILE_SCOPE("Renderer Draw");
		Renderent::Renderer2D::BeginScene(m_CameraController.GetCamera());
		Renderent::Renderer2D::DrawQuad({ -1.0f, 0.0f, -0.01 }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
		Renderent::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, { 0.3f, 0.2f, 0.8f, 1.0f });
		Renderent::Renderer2D::DrawQuad({ 0.0f, 0.1f, -0.1f }, { 10.0f, 10.0f }, m_Checkerboard, { 0.3f, 0.3f, 0.35f, 1.0f });
		Renderent::Renderer2D::DrawQuad(m_Player->GetPosition(), m_Player->GetSize(), m_Player->GetColor());
		Renderent::Renderer2D::DrawQuad(m_Floor->GetPosition(), m_Floor->GetSize(), m_Floor->GetColor());
		Renderent::Renderer2D::EndScene();

	}

}
void Sandbox2D::OnImGuiRender() {

	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));

	ImGui::End();

}
void Sandbox2D::OnEvent(Renderent::Event& e) {
	m_CameraController.OnEvent(e);
	if (e.GetEventType() == Renderent::EventType::KeyPressed) {
		m_Player->processEvent((Renderent::KeyPressedEvent&)e);
	}
}