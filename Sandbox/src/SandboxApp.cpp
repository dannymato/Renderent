#include "Sandbox2D.h"

#include <Renderent.h>
#include <Renderent/Core/Entrypoint.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

class ExampleLayer : public Renderent::Layer {
public:
	ExampleLayer() :
		Layer("Example"), m_CameraController(1920.0f / 1080.0f, true), m_CameraPosition(0.0f) {
	
		m_VertexArray = Renderent::VertexArray::Create();

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5,  0.0f, 0.2f, 0.8f, 0.8f, 1.0f,
			 0.0f,  0.5,  0.0f, 0.8f, 0.8f, 0.2f, 1.0f,
		};

		Renderent::Ref<Renderent::VertexBuffer> vertexBuffer;
		vertexBuffer = Renderent::VertexBuffer::Create(sizeof(vertices), vertices);

		Renderent::BufferLayout layout = {
			{Renderent::ShaderDataType::Float3, "a_Position"},
			{Renderent::ShaderDataType::Float4, "a_Color" }
		};

		vertexBuffer->SetLayout(layout);

		m_VertexArray->AddVertexBuffer(vertexBuffer);


		uint32_t indices[3] = { 0, 1, 2 };

		Renderent::Ref<Renderent::IndexBuffer> indexBuffer;
		indexBuffer = Renderent::IndexBuffer::Create(3, indices);
		m_VertexArray->AddIndexBuffer(indexBuffer);

		m_SquareVA = Renderent::VertexArray::Create();

		float squareVertices[5 * 4] = {
			-0.5f, -0.5f,  0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f,  0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
		};

		Renderent::Ref<Renderent::VertexBuffer> squareVB;
		squareVB = Renderent::VertexBuffer::Create(sizeof(squareVertices), squareVertices);

		squareVB->SetLayout({
			{Renderent::ShaderDataType::Float3, "a_Position"},
			{Renderent::ShaderDataType::Float2, "a_TexCoord"}
			});

		m_SquareVA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		Renderent::Ref<Renderent::IndexBuffer> square_IB;
		square_IB =Renderent::IndexBuffer::Create(6, squareIndices);
		m_SquareVA->AddIndexBuffer(square_IB);

		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec4 v_Color;

			void main() {
				
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1);
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

		m_TriangleShader = Renderent::Shader::Create("VertexPosColor", vertexSrc, fragmentSrc);

		std::string flatColorShaderVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			void main() {
				
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1);

			}


		)";

		std::string flatColorShaderFragmentSrc2 = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			uniform vec4 u_Color;

			void main() {
				
				color = u_Color;

			}


		)";

		m_FlatColorShader = Renderent::Shader::Create("Flat Color"
			, flatColorShaderVertexSrc, flatColorShaderFragmentSrc2);

		auto textureShader = m_ShaderLibrary.Load("assets/shader/Texture.glsl");
		
		
		m_Texture = Renderent::Texture2D::Create("assets/textures/Checkerboard.png");
		m_ChernoLogoTexture = Renderent::Texture2D::Create("ChernoLogo.png");


		textureShader->Bind();
		textureShader->SetInt(0, "u_Texture");
	}

	void OnUpdate(Renderent::Timestep timestep) override {

		
		m_CameraController.OnUpdate(timestep);

		Renderent::RenderCommand::SetClearColor({ 0.2f, 0.2f, 0.2f, 1.0f });
		Renderent::RenderCommand::Clear();

		Renderent::Renderer::BeginScene(m_CameraController.GetCamera());

		
		static glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		m_FlatColorShader->Bind();
		m_FlatColorShader->SetFloat4(m_SquaresColor, "u_Color");

		for (int y = 0; y < 10; y++) {
			for (int x = 0; x < 10; x++) {
				glm::vec3 pos(x * 0.11f,y * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;

				Renderent::Renderer::Submit(m_SquareVA, m_FlatColorShader, transform);
			}
		}

		auto textureShader = m_ShaderLibrary.Get("Texture");

		m_Texture->Bind();
		Renderent::Renderer::Submit(m_SquareVA, textureShader, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));


		m_ChernoLogoTexture->Bind();
		Renderent::Renderer::Submit(m_SquareVA, textureShader, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));



		//Triangle
		//Renderent::Renderer::Submit(m_VertexArray, m_Shader, triangleTransform);

		Renderent::Renderer::EndScene();

	}

	void OnEvent(Renderent::Event& event) override {
		m_CameraController.OnEvent(event);

	}

	virtual void OnImGuiRender() override {

		ImGui::Begin("Settings");
		ImGui::ColorPicker4("Square Color", glm::value_ptr(m_SquaresColor));
		ImGui::End();
	}


private:
	Renderent::ShaderLibrary m_ShaderLibrary;

	Renderent::Ref<Renderent::Shader> m_TriangleShader;
	Renderent::Ref<Renderent::VertexArray> m_VertexArray;

	Renderent::Ref<Renderent::VertexArray> m_SquareVA;
	Renderent::Ref<Renderent::Shader> m_FlatColorShader;

	Renderent::OrthographicCameraController m_CameraController;
	glm::vec3 m_CameraPosition;

	float m_CameraRotation = 0.0f;

	float m_CameraMoveSpeed = 2.0f;
	float m_CameraRotationSpeed = 180.0f;

	glm::vec4 m_SquaresColor = { 0.0f, 0.0f, 0.0f, 1.0f };

	Renderent::Ref<Renderent::Texture2D> m_Texture, m_ChernoLogoTexture;


};

class Sandbox : public Renderent::Application {
public:
	Sandbox():Sandbox(Renderent::WindowProps()) {

	}

	Sandbox(const Renderent::WindowProps& props)
	: Renderent::Application(props) {
		//PushLayer(new ExampleLayer());
		PushLayer(new Sandbox2D());
	}

	~Sandbox() {

	}
};


Renderent::Application* Renderent::CreateApplication() {
	return new Sandbox(Renderent::WindowProps("Sandbox App", 1920, 1080, true));
}