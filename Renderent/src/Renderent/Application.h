#pragma once
#include "Core.h"
#include "Window.h"
#include "Event/WindowEvent.h"
#include "Renderent/Layer.h"
#include "Renderent/LayerStack.h"
#include "Renderent/Event/KeyEvent.h"
#include "Renderent/Event/MouseEvent.h"
#include "Renderent/ImGui/ImGuiLayer.h"
#include "Renderent/Core/TimeStep.h"

namespace Renderent {


	class RENDERENT_API Application
	{
	public:
		Application();
		Application(const WindowProps& props);
		virtual ~Application();

		void Run();
		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline Window& GetWindow() { return *m_Window; }

		static inline Application& Get() { return *s_Instance; }
	

	private:
		bool OnWindowClose(WindowClosedEvent& e);
		bool OnWindowResized(WindowResizedEvent& e);

	private:
		LayerStack m_LayerStack;

		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		static Application* s_Instance;

		float m_LastFrameTime = 0.0f;

		bool m_Minimized = false;
	};

	// To be defined by client
	Application* CreateApplication();

}

