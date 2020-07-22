#include "Sandbox2D.h"

#include <chrono>

#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

static const uint32_t s_MapWidth = 31;

static const char* s_MapTiles =
"wwwwwwwwwwwwwwwwwwwwwwwwwwwwwww"
"wwwwww01111111111111111112wwwww"
"ww0111k4444444444444444445wwwww"
"ww344444444444444444444445wwwww"
"ww677j44444444444444444445wwwww"
"wwwww6j4444444444444444445wwwww"
"wwwwww34444444444444444445wwwww"
"wwwww0k449aaab444444444445wwwww"
"wwwww3444cddde444444444445wwwww"
"wwwww3444cddde444444444445wwwww"
"wwwww3444cddde444444444445wwwww"
"wwwww3444fgggh444444444445wwwww"
"wwwww6j4444444444444444445wwwww"
"wwwwww34444444444444444445wwwww"
"wwwwww34444444444444444445wwwww"
"wwwwww34444444444444444445wwwww"
"wwwwww67777777777777777778wwwww"
"wwwwwwwwwwwwwwwwwwwwwwwwwwwwwww";


Sandbox2D::Sandbox2D() : Layer("Sandbox2D"), m_CameraController(1920.0f / 1080.0f, true){

	RE_PROFILE_FUNCTION();
	m_World = new b2World({ 0.0f, -20.0f });
	m_Player = new Player({ 0.0f, 2.0f }, { 0.5f, 1.0f }, { 0.8f, 0.2f, 0.8f, 1.0f }, *m_World);
	m_Floor = new Floor({ 0.0f, -3.0f }, { 10.0f, 5.0f }, { 2.0f, 0.8f, 0.7f, 1.0f }, *m_World);

}

void Sandbox2D::OnAttach() {

	RE_PROFILE_FUNCTION();

	m_Checkerboard = Renderent::Texture2D::Create("assets/textures/Checkerboard.png");
	m_Spritesheet = Renderent::Texture2D::Create("assets/game/texture/RPGpack_sheet_2X.png");

	m_TextureStairs = Renderent::SubTexture2D::CreateFromCoords(m_Spritesheet, { 0, 11 }, { 128, 128 });
	m_TextureBarrel = Renderent::SubTexture2D::CreateFromCoords(m_Spritesheet, { 8, 2 }, { 128, 128 });
	m_TextureTree = Renderent::SubTexture2D::CreateFromCoords(m_Spritesheet, { 2, 1 }, { 128, 128 }, { 1, 2 });
	m_TextureGround = Renderent::SubTexture2D::CreateFromCoords(m_Spritesheet, { 1, 11 }, { 128, 128 });

	s_TextureMap['w'] = Renderent::SubTexture2D::CreateFromCoords(m_Spritesheet, { 6, 11 }, { 128, 128 });
	s_TextureMap['0'] = Renderent::SubTexture2D::CreateFromCoords(m_Spritesheet, { 0, 12 }, { 128, 128 });
	s_TextureMap['1'] = Renderent::SubTexture2D::CreateFromCoords(m_Spritesheet, { 1, 12 }, { 128, 128 });
	s_TextureMap['2'] = Renderent::SubTexture2D::CreateFromCoords(m_Spritesheet, { 2, 12 }, { 128, 128 });
	s_TextureMap['3'] = Renderent::SubTexture2D::CreateFromCoords(m_Spritesheet, { 0, 11 }, { 128, 128 });
	s_TextureMap['4'] = Renderent::SubTexture2D::CreateFromCoords(m_Spritesheet, { 1, 11 }, { 128, 128 });
	s_TextureMap['5'] = Renderent::SubTexture2D::CreateFromCoords(m_Spritesheet, { 2, 11 }, { 128, 128 });
	s_TextureMap['6'] = Renderent::SubTexture2D::CreateFromCoords(m_Spritesheet, { 0, 10 }, { 128, 128 });
	s_TextureMap['7'] = Renderent::SubTexture2D::CreateFromCoords(m_Spritesheet, { 1, 10 }, { 128, 128 });
	s_TextureMap['8'] = Renderent::SubTexture2D::CreateFromCoords(m_Spritesheet, { 2, 10 }, { 128, 128 });
	s_TextureMap['9'] = Renderent::SubTexture2D::CreateFromCoords(m_Spritesheet, { 10, 12 }, { 128, 128 });
	s_TextureMap['a'] = Renderent::SubTexture2D::CreateFromCoords(m_Spritesheet, { 11, 12 }, { 128, 128 });
	s_TextureMap['b'] = Renderent::SubTexture2D::CreateFromCoords(m_Spritesheet, { 12, 12 }, { 128, 128 });
	s_TextureMap['c'] = Renderent::SubTexture2D::CreateFromCoords(m_Spritesheet, { 10, 11 }, { 128, 128 });
	s_TextureMap['d'] = Renderent::SubTexture2D::CreateFromCoords(m_Spritesheet, { 11, 11 }, { 128, 128 });
	s_TextureMap['e'] = Renderent::SubTexture2D::CreateFromCoords(m_Spritesheet, { 12, 11 }, { 128, 128 });
	s_TextureMap['f'] = Renderent::SubTexture2D::CreateFromCoords(m_Spritesheet, { 10, 10 }, { 128, 128 });
	s_TextureMap['g'] = Renderent::SubTexture2D::CreateFromCoords(m_Spritesheet, { 11, 10 }, { 128, 128 });
	s_TextureMap['h'] = Renderent::SubTexture2D::CreateFromCoords(m_Spritesheet, { 12, 10 }, { 128, 128 });
	s_TextureMap['j'] = Renderent::SubTexture2D::CreateFromCoords(m_Spritesheet, { 4, 12 }, { 128, 128 });
	s_TextureMap['k'] = Renderent::SubTexture2D::CreateFromCoords(m_Spritesheet, { 4, 11 }, { 128, 128 });

	m_MapWidth = s_MapWidth;
	m_MapHeight = strlen(s_MapTiles) / m_MapWidth;

	m_Particle.ColorBegin = { 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
	m_Particle.ColorEnd = { 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f };
	m_Particle.SizeBegin = 0.5f; m_Particle.SizeVariation = 0.3f; m_Particle.SizeEnd = 0.0f;
	m_Particle.LifeTime = 2.0f;
	m_Particle.Velocity = { 0.0f, 0.0f };
	m_Particle.VelocityVariation = { 3.0f, 1.0f };
	m_Particle.Position = { 0.0f, 0.0f };

	m_CameraController.SetZoomLevel(0.25f);

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
		Renderent::RenderCommand::SetClearColor({ 0.2f, 0.2f, 0.2f, 0.2f });
		Renderent::RenderCommand::Clear();

	}

	{
#if 0
		static float rotation = 0.0f;
		rotation += ts * 50.0f;

		RE_PROFILE_SCOPE("Renderer Draw");
		Renderent::Renderer2D::BeginScene(m_CameraController.GetCamera());
		Renderent::Renderer2D::DrawQuad({ 0.0f, 0.1f, -0.4f }, { 10.0f, 10.0f }, m_Checkerboard);
		//Renderent::Renderer2D::DrawRotatedQuad({ 0.0f, 0.0f, -0.2f }, { 5.0f, 5.0f }, glm::radians(rotation), m_Checkerboard);
		Renderent::Renderer2D::DrawQuad({ -1.0f, 0.0f, -0.1f }, { 0.8f, 0.8f }, m_SquareColor);
		Renderent::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, { 0.3f, 0.2f, 0.8f, 1.0f });
		Renderent::Renderer2D::DrawQuad(m_Player->GetPosition(), m_Player->GetSize(), m_Player->GetColor());
		Renderent::Renderer2D::DrawQuad(m_Floor->GetPosition(), m_Floor->GetSize(), m_Floor->GetColor());


		for (float y = -5.0f; y < 5.0f; y += 0.5f) {
			for (float x = -5.0f; x < 5.0f; x += 0.5f) {
				glm::vec4 color = { (x + 5.0f) / 10.0f, 0.4f, (y + 5.0f) / 10.f, 0.5f};
				Renderent::Renderer2D::DrawQuad({ x, y, -0.1f }, { 0.45f, 0.45f }, color);
			}
		}
		Renderent::Renderer2D::EndScene();

		m_Player->OnUpdate(ts, m_ParticleSystem);

#endif

		
		if (Renderent::Input::IsMouseButtonPressed(RE_MOUSE_BUTTON_1)) {
			auto [x, y] = Renderent::Input::GetMousePosition();
			auto width = Renderent::Application::Get().GetWindow().GetWidth();
			auto height = Renderent::Application::Get().GetWindow().GetHeight();

			auto bounds = m_CameraController.GetBounds();
			auto pos = m_CameraController.GetCamera().GetPosition();
			x = (x / width) * bounds.GetWidth() - bounds.GetWidth() * 0.5f;
			y = bounds.GetHeight() * 0.5f - (y / height) * bounds.GetHeight();
			m_Particle.Position = { x + pos.x, y + pos.y };
			for (int i = 0; i < 15; i++) {
				m_ParticleSystem.Emit(m_Particle);
			}
		}

		m_ParticleSystem.OnUpdate(ts);
		m_ParticleSystem.OnRender(m_CameraController.GetCamera());

		Renderent::Renderer2D::BeginScene(m_CameraController.GetCamera());
		
		for (uint32_t y = 0; y < m_MapHeight; y++) {
			for (uint32_t x = 0; x < m_MapWidth; x++) {
				char tileType = s_MapTiles[x + y * m_MapWidth];
				Renderent::Ref<Renderent::SubTexture2D> texture;
				if (s_TextureMap.find(tileType) != s_TextureMap.end()) {
					texture = s_TextureMap[tileType];
				}
				else
					texture = m_TextureBarrel;

				Renderent::Renderer2D::DrawQuad({ x - m_MapWidth / 2.0f, m_MapHeight - y - m_MapHeight / 2.0f, 0.5f }, { 1.0f, 1.0f }, texture);
			}
		}

		Renderent::Renderer2D::DrawQuad({ 0.0f, 0.0f, 0.5f }, { 1.0f, 1.0f }, m_TextureStairs);
		Renderent::Renderer2D::DrawQuad({ 1.0f, 0.0f, 0.5f }, { 1.0f, 1.0f }, m_TextureBarrel);
		Renderent::Renderer2D::DrawQuad({ -1.0f, 0.0f, 0.5f }, { 1.0f, 2.0f }, m_TextureTree);
		
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