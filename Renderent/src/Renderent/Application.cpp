#include "repch.h"
#include "Application.h"
#include "Renderent/Event/MouseEvent.h"
#include "Renderent/Event/WindowEvent.h"
#include "Input.h"

#include "Renderer/RenderCommand.h"
#include "Renderer/Renderer.h"


namespace Renderent {

	Application* Application::s_Instance = nullptr;

	

	Application::Application() {

		RE_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

		m_VertexArray.reset(VertexArray::Create());


		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5,  0.0f, 0.2f, 0.8f, 0.8f, 1.0f,
			 0.0f,  0.5,  0.0f, 0.8f, 0.8f, 0.2f, 1.0f,
		};

		std::shared_ptr<VertexBuffer> vertexBuffer;
		vertexBuffer.reset(VertexBuffer::Create(sizeof(vertices), vertices));

		BufferLayout layout = {
			{ShaderDataType::Float3, "a_Position"},
			{ShaderDataType::Float4, "a_Color" }
			};

		vertexBuffer->SetLayout(layout);

		m_VertexArray->AddVertexBuffer(vertexBuffer);


		uint32_t indices[3] = { 0, 1, 2 };

		std::shared_ptr<IndexBuffer> indexBuffer;
		indexBuffer.reset(IndexBuffer::Create(3, indices));
		m_VertexArray->AddIndexBuffer(indexBuffer);

		m_SquareVA.reset(VertexArray::Create());

		float squareVertices[3 * 4] = {
			-0.75f, -0.75f,  0.0f,
			 0.75f, -0.75f,  0.0f,
			 0.75f,  0.75f,  0.0f,
			-0.75f,  0.75f,  0.0f
		};

		std::shared_ptr<VertexBuffer> squareVB;
			squareVB.reset(VertexBuffer::Create(sizeof(squareVertices), squareVertices));

		squareVB->SetLayout({
			{ShaderDataType::Float3, "a_Position"}
		});

		m_SquareVA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<IndexBuffer> square_IB;
			square_IB.reset(IndexBuffer::Create(6, squareIndices));
		m_SquareVA->AddIndexBuffer(square_IB);

		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;
			out vec4 v_Color;

			void main() {
				
				gl_Position = vec4(a_Position, 1);
				v_Color = a_Color;

			}


		)";

		std::string fragmentSrc = R"(
			#version 330 core
			
			in vec4 v_Color;
			layout(location = 0) out vec4 color;

			void main() {
				
				color = v_Color;

			}


		)";

		m_Shader.reset(ShaderProgram::Create(vertexSrc, fragmentSrc));

		std::string blueShaderVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;

			void main() {
				
				gl_Position = vec4(a_Position, 1);

			}


		)";

		std::string blueShaderFragmentSrc2 = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			void main() {
				
				color = vec4(0.2, 0.4, 0.8, 1.0);

			}


		)";

		m_BlueShader.reset(ShaderProgram::Create(blueShaderVertexSrc, blueShaderFragmentSrc2));

	}

	Application::~Application() {

	}

	void Application::PushLayer(Layer* layer) {
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer) {
		m_LayerStack.PushOverlay(layer);
	}

	void Application::OnEvent(Event& e) {

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowClosedEvent>(BIND_EVENT_FN(Application::OnWindowClose));

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();) {
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}

	}


	void Application::Run() {

		while (m_Running) {

			RenderCommand::SetClearColor({ 0.2f, 0.2f, 0.2f, 1.0f });
			RenderCommand::Clear();
			
			Renderer::BeginScene();

			m_BlueShader->Bind();
			Renderer::Submit(m_SquareVA);

			m_Shader->Bind();
			Renderer::Submit(m_VertexArray);

			Renderer::EndScene();

			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();

			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();
			m_ImGuiLayer->End();
			m_Window->OnUpdate();
			
		}
	}

	bool Application::OnWindowClose(WindowClosedEvent& e) {
		m_Running = false;
		return true;
	}

}