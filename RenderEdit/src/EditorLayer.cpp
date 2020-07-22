#include "EditorLayer.h"

#include <chrono>

#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

namespace Renderent {

	EditorLayer::EditorLayer() : Layer("Sandbox2D"), m_CameraController(1920.0f / 1080.0f, true) {

		RE_PROFILE_FUNCTION();
	}

	void EditorLayer::OnAttach() {

		RE_PROFILE_FUNCTION();

		FramebufferProps fBufferProps;
		auto& window = Application::Get().GetWindow();
		fBufferProps.width = window.GetWidth();
		fBufferProps.height = window.GetHeight();
		fBufferProps.attachments = FramebufferAttachments::ColorAttachment | Renderent::FramebufferAttachments::DepthAttachment;
		m_Framebuffer = Framebuffer::Create(fBufferProps);

		m_Checkerboard = Texture2D::Create("assets/textures/Checkerboard.png");
		m_Spritesheet = Texture2D::Create("assets/game/texture/RPGpack_sheet_2X.png");

		m_TextureStairs = SubTexture2D::CreateFromCoords(m_Spritesheet, { 0, 11 }, { 128, 128 });
		m_TextureBarrel = SubTexture2D::CreateFromCoords(m_Spritesheet, { 8, 2 }, { 128, 128 });
		m_TextureTree = SubTexture2D::CreateFromCoords(m_Spritesheet, { 2, 1 }, { 128, 128 }, { 1, 2 });
		m_TextureGround = SubTexture2D::CreateFromCoords(m_Spritesheet, { 1, 11 }, { 128, 128 });
		m_CameraController.SetZoomLevel(0.25f);

	}
	void EditorLayer::OnDetach() {

	}



	void EditorLayer::OnUpdate(Renderent::Timestep ts) {

		RE_PROFILE_FUNCTION();

		m_CurrentTime += ts;

		{
			RE_PROFILE_SCOPE("CameraController::OnUpdate");
			m_CameraController.OnUpdate(ts);
		}

		Renderer2D::ResetStats();

		{
			RE_PROFILE_SCOPE("Renderer Prep");
			m_Framebuffer->Bind();

			RenderCommand::SetClearColor({ 0.2f, 0.2f, 0.2f, 0.2f });
			RenderCommand::Clear();

		}

		{
			static float rotation = 0.0f;
			rotation += ts * 50.0f;

			RE_PROFILE_SCOPE("Renderer Draw");
			Renderer2D::BeginScene(m_CameraController.GetCamera());
			Renderer2D::DrawQuad({ 0.0f, 0.1f, -0.4f }, { 10.0f, 10.0f }, m_Checkerboard);
			Renderer2D::DrawRotatedQuad({ 0.0f, 0.0f, -0.2f }, { 5.0f, 5.0f }, glm::radians(rotation), m_Checkerboard);
			Renderer2D::DrawQuad({ -1.0f, 0.0f, -0.1f }, { 0.8f, 0.8f }, m_SquareColor);
			Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, { 0.3f, 0.2f, 0.8f, 1.0f });
			

			for (float y = -5.0f; y < 5.0f; y += 0.5f) {
				for (float x = -5.0f; x < 5.0f; x += 0.5f) {
					glm::vec4 color = { (x + 5.0f) / 10.0f, 0.4f, (y + 5.0f) / 10.f, 0.5f };
					Renderer2D::DrawQuad({ x, y, -0.1f }, { 0.45f, 0.45f }, color);
				}
			}

			Renderer2D::EndScene();

			m_Framebuffer->Unbind();

		}

	}
	void EditorLayer::OnImGuiRender() {


		static bool opt_fullscreen_persistant = true;
		bool opt_fullscreen = opt_fullscreen_persistant;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
		static bool dockspaceOpen = false;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->GetWorkPos());
			ImGui::SetNextWindowSize(viewport->GetWorkSize());
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background 
		// and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
		ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// DockSpace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				// Disabling fullscreen would allow the window to be moved to the front of other windows,
				// which we can't undo at the moment without finer window depth/z control.
				//ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);
				if (ImGui::MenuItem("Exit"))
					Renderent::Application::Get().Close();

				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		ImGui::Begin("Settings");
		ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));

		auto stats = Renderent::Renderer2D::GetStats();
		ImGui::Text("Renderer2D Stats:");
		ImGui::Text("Draw Calls: %d", stats.DrawCalls);
		ImGui::Text("Quads: %d", stats.QuadCount);
		ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
		ImGui::Text("Indices: %d", stats.getTotalIndexCount());
		ImGui::End();
		
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2({ 0, 0 }));
		ImGui::Begin("Viewport");
		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		RE_WARN("Viewport size: {0}, {1}", viewportPanelSize.x, viewportPanelSize.y);
		if (m_ViewportSize != *((glm::vec2*) & viewportPanelSize)) {
			m_Framebuffer->Resize((uint32_t)viewportPanelSize.x, (uint32_t)viewportPanelSize.y);
			m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
		}
		uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
		ImGui::Image((void*)textureID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
		ImGui::End();
		ImGui::PopStyleVar();

		// Dockspace end
		ImGui::End();

	}
	void EditorLayer::OnEvent(Event& e) {
		m_CameraController.OnEvent(e);
	}

}