#include "Sandbox2D.h"

#include <chrono>

#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>


Sandbox2D::Sandbox2D() : Layer("Sandbox2D"), m_CameraController(1920.0f / 1080.0f, true){

	RE_PROFILE_FUNCTION();
	m_World = new b2World({ 0.0f, -20.0f });
	m_Player = new Player({ 0.0f, 2.0f }, { 0.5f, 1.0f }, { 0.8f, 0.2f, 0.8f, 1.0f }, * m_World);
	m_Floor = new Floor({ 0.0f, -3.0f }, { 10.0f, 5.0f }, { 2.0f, 0.8f, 0.7f, 1.0f }, *m_World);

}

void Sandbox2D::OnAttach() {

	RE_PROFILE_FUNCTION();
	m_Checkerboard = Renderent::Texture2D::Create("assets/textures/Checkerboard.png");

}
void Sandbox2D::OnDetach() {

}



void Sandbox2D::OnUpdate(Renderent::Timestep ts) {

	RE_PROFILE_FUNCTION();

	m_CurrentTime += ts;

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

	Renderent::Renderer2D::ResetStats();

	{
		RE_PROFILE_SCOPE("Renderer Prep");
		Renderent::RenderCommand::SetClearColor({ 1.0f, 1.0f, 1.0f, 1.0f });
		Renderent::RenderCommand::Clear();

	}

	{

		static float rotation = 0.0f;
		rotation += ts * 50.0f;

		RE_PROFILE_SCOPE("Renderer Draw");
		Renderent::Renderer2D::BeginScene(m_CameraController.GetCamera());
		Renderent::Renderer2D::DrawQuad({ 0.0f, 0.1f, -0.4f }, { 10.0f, 10.0f }, m_Checkerboard);
		Renderent::Renderer2D::DrawRotatedQuad({ 0.0f, 0.0f, -0.2f }, { 5.0f, 5.0f }, glm::radians(rotation), m_Checkerboard);
		Renderent::Renderer2D::DrawQuad({ -1.0f, 0.0f, -0.1f }, { 0.8f, 0.8f }, m_SquareColor);
		Renderent::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, { 0.3f, 0.2f, 0.8f, 1.0f });
		Renderent::Renderer2D::DrawQuad(m_Player->GetPosition(), m_Player->GetSize(), m_Player->GetColor());
		Renderent::Renderer2D::DrawQuad(m_Floor->GetPosition(), m_Floor->GetSize(), m_Floor->GetColor());
		Renderent::Renderer2D::EndScene();

		Renderent::Renderer2D::BeginScene(m_CameraController.GetCamera());
		for (float y = -5.0f; y < 5.0f; y += 0.5f) {
			for (float x = -5.0f; x < 5.0f; x += 0.5f) {
				glm::vec4 color = { (x + 5.0f) / 10.0f, 0.4f, (y + 5.0f) / 10.f, 0.5f};
				Renderent::Renderer2D::DrawQuad({ x, y }, { 0.45f, 0.45f }, color);
			}
		}
		Renderent::Renderer2D::EndScene();
		
	}

}
void Sandbox2D::OnImGuiRender() {

	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));

	auto stats = Renderent::Renderer2D::GetStats();
	ImGui::Text("Renderer2D Stats:");
	ImGui::Text("Draw Calls: %d", stats.DrawCalls);
	ImGui::Text("Quads: %d", stats.QuadCount);
	ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
	ImGui::Text("Indices: %d", stats.getTotalIndexCount());
	ImGui::End();

}
void Sandbox2D::OnEvent(Renderent::Event& e) {
	m_CameraController.OnEvent(e);
	if (e.GetEventType() == Renderent::EventType::KeyPressed) {
		m_Player->processEvent((Renderent::KeyPressedEvent&)e);
	}
}