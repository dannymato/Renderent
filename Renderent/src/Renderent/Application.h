#pragma once
#include "Core.h"
#include "Window.h"
#include "Event/WindowEvent.h"
#include "Renderent/Layer.h"
#include "Renderent/LayerStack.h"
#include "Renderent/Event/KeyEvent.h"
#include "Renderent/Event/MouseEvent.h"
#include "Renderent/ImGui/ImGuiLayer.h"

#include "Renderent/Renderer/Shader.h"
#include "Renderent/Renderer/Buffer.h"
#include "Renderent/Renderer/VertexArray.h"

namespace Renderent {


	class RENDERENT_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline Window& GetWindow() { return *m_Window; }

		static inline Application& Get() { return *s_Instance; }
	

	private:
		bool OnWindowClose(WindowClosedEvent& e);

		LayerStack m_LayerStack;

		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		static Application* s_Instance;

		std::shared_ptr<ShaderProgram> m_Shader;
		std::shared_ptr<VertexArray> m_VertexArray;

		std::shared_ptr<VertexArray> m_SquareVA;
		std::shared_ptr<ShaderProgram> m_BlueShader;
	};

	// To be defined by client
	Application* CreateApplication();

}

