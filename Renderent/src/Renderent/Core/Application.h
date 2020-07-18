#pragma once

#include "Renderent/Core/Core.h"
#include "Renderent/Core/Layer.h"
#include "Renderent/Core/LayerStack.h"
#include "Renderent/Core/TimeStep.h"
#include "Renderent/Core/Window.h"
#include "Renderent/Event/KeyEvent.h"
#include "Renderent/Event/MouseEvent.h"
#include "Renderent/Event/WindowEvent.h"
#include "Renderent/ImGui/ImGuiLayer.h"

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

		void Close();
	

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

