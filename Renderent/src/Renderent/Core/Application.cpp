#include "repch.h"
#include "Application.h"

#include "Renderent/Core/KeyCodes.h"
#include "Renderent/Core/Input.h"
#include "Renderent/Event/MouseEvent.h"
#include "Renderent/Event/WindowEvent.h"
#include "Renderent/Renderer/RenderCommand.h"
#include "Renderent/Renderer/Renderer.h"

namespace Renderent {

	Application* Application::s_Instance = nullptr;

	Application::Application() : Application::Application(WindowProps()) {}

	Application::Application(const WindowProps& props) {

		RE_PROFILE_FUNCTION();
	
		RE_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create(props));
		m_Window->SetEventCallback(RE_BIND_EVENT_FN(Application::OnEvent));

		Renderer::Init();

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

	}

	Application::~Application() {

	}

	void Application::PushLayer(Layer* layer) {
		RE_PROFILE_FUNCTION();
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer) {
		RE_PROFILE_FUNCTION();
		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	void Application::OnEvent(Event& e) {
		RE_PROFILE_FUNCTION();
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowClosedEvent>(RE_BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizedEvent>(RE_BIND_EVENT_FN(Application::OnWindowResized));

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();) {
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}

	}


	void Application::Run() {
		RE_PROFILE_FUNCTION();
		while (m_Running) {

			RE_PROFILE_SCOPE("RunLoop");
			float time = m_Window->GetTime();
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			if (!m_Minimized) {
				{
					RE_PROFILE_SCOPE("LayerStack OnUpdate");
					// Run the on update for all the layers
					for (Layer* layer : m_LayerStack)
						layer->OnUpdate(timestep);
				}
				{
					RE_PROFILE_SCOPE("LayerStack OnImGuiRender");
					m_ImGuiLayer->Begin();
					for (Layer* layer : m_LayerStack)
						layer->OnImGuiRender();
					m_ImGuiLayer->End();
				}
			}
			// Run all imgui code

			m_Window->OnUpdate();
			
		}
	}

	void Application::Close() {
		m_Running = false;
	}

	bool Application::OnWindowClose(WindowClosedEvent& e) {
		m_Running = false;
		return true;
	}

	bool Application::OnWindowResized(WindowResizedEvent& e) {

		RE_PROFILE_FUNCTION();

		if (e.GetWidth() == 0 || e.GetHeight() == 0) {
			m_Minimized = true;
			return false;
		}
		m_Minimized = false;

		Renderer::OnWindowResized(e.GetWidth(), e.GetHeight());

		return false;
	}


}