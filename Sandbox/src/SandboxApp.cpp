#include <Renderent.h>
#include "Renderent/Platform/OpenGL/OpenGLShader.h"

#include <imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class ExampleLayer : public Renderent::Layer {
public:
	ExampleLayer() :
		Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(0.0f) {
	
		m_VertexArray.reset(Renderent::VertexArray::Create());

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5,  0.0f, 0.2f, 0.8f, 0.8f, 1.0f,
			 0.0f,  0.5,  0.0f, 0.8f, 0.8f, 0.2f, 1.0f,
		};

		Renderent::Ref<Renderent::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(Renderent::VertexBuffer::Create(sizeof(vertices), vertices));

		Renderent::BufferLayout layout = {
			{Renderent::ShaderDataType::Float3, "a_Position"},
			{Renderent::ShaderDataType::Float4, "a_Color" }
		};

		vertexBuffer->SetLayout(layout);

		m_VertexArray->AddVertexBuffer(vertexBuffer);


		uint32_t indices[3] = { 0, 1, 2 };

		Renderent::Ref<Renderent::IndexBuffer> indexBuffer;
		indexBuffer.reset(Renderent::IndexBuffer::Create(3, indices));
		m_VertexArray->AddIndexBuffer(indexBuffer);

		m_SquareVA.reset(Renderent::VertexArray::Create());

		float squareVertices[5 * 4] = {
			-0.5f, -0.5f,  0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f,  0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
		};

		Renderent::Ref<Renderent::VertexBuffer> squareVB;
		squareVB.reset(Renderent::VertexBuffer::Create(sizeof(squareVertices), squareVertices));

		squareVB->SetLayout({
			{Renderent::ShaderDataType::Float3, "a_Position"},
			{Renderent::ShaderDataType::Float2, "a_TexCoord"}
			});

		m_SquareVA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		Renderent::Ref<Renderent::IndexBuffer> square_IB;
		square_IB.reset(Renderent::IndexBuffer::Create(6, squareIndices));
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

		m_Shader.reset(Renderent::Shader::Create(vertexSrc, fragmentSrc));

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

		m_FlatColorShader.reset(Renderent::Shader::Create(flatColorShaderVertexSrc, flatColorShaderFragmentSrc2));

		m_TextureShader.reset(Renderent::Shader::Create("assets/shader/Texture.glsl"));
		
		
		m_Texture = Renderent::Texture2D::Create("Checkerboard.png");
		m_ChernoLogoTexture = Renderent::Texture2D::Create("ChernoLogo.png");


		std::dynamic_pointer_cast<Renderent::OpenGLShader>(m_TextureShader)->Bind();
		std::dynamic_pointer_cast<Renderent::OpenGLShader>(m_TextureShader)->UploadUniformInt(0, "u_Texture");
	}

	void OnUpdate(Renderent::Timestep timestep) override {

		if (Renderent::Input::IsKeyPressed(RE_KEY_LEFT))
			m_CameraPosition.x -= m_CameraMoveSpeed * timestep;
		else if (Renderent::Input::IsKeyPressed(RE_KEY_RIGHT))
			m_CameraPosition.x += m_CameraMoveSpeed * timestep;

		if (Renderent::Input::IsKeyPressed(RE_KEY_UP))
			m_CameraPosition.y += m_CameraMoveSpeed * timestep;
		else if (Renderent::Input::IsKeyPressed(RE_KEY_DOWN))
			m_CameraPosition.y -= m_CameraMoveSpeed * timestep;

		if (Renderent::Input::IsKeyPressed(RE_KEY_A))
			m_CameraRotation += m_CameraRotationSpeed * timestep;
		else if (Renderent::Input::IsKeyPressed(RE_KEY_D))
			m_CameraRotation -= m_CameraRotationSpeed * timestep;


		Renderent::RenderCommand::SetClearColor({ 0.2f, 0.2f, 0.2f, 1.0f });
		Renderent::RenderCommand::Clear();

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);

		Renderent::Renderer::BeginScene(m_Camera);

		
		static glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		m_FlatColorShader->Bind();
		std::dynamic_pointer_cast<Renderent::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat4(m_SquaresColor, "u_Color");

		for (int y = 0; y < 10; y++) {
			for (int x = 0; x < 10; x++) {
				glm::vec3 pos(x * 0.11f,y * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;

				Renderent::Renderer::Submit(m_SquareVA, m_FlatColorShader, transform);
			}
		}

		m_Texture->Bind();
		Renderent::Renderer::Submit(m_SquareVA, m_TextureShader, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));


		m_ChernoLogoTexture->Bind();
		Renderent::Renderer::Submit(m_SquareVA, m_TextureShader, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));



		//Triangle
		//Renderent::Renderer::Submit(m_VertexArray, m_Shader, triangleTransform);

		Renderent::Renderer::EndScene();

	}

	void OnEvent(Renderent::Event& event) override {

	}

	virtual void OnImGuiRender() override {

		ImGui::Begin("Settings");
		ImGui::ColorPicker4("Square Color", glm::value_ptr(m_SquaresColor));
		ImGui::End();
	}


private:
	Renderent::Ref<Renderent::Shader> m_Shader;
	Renderent::Ref<Renderent::VertexArray> m_VertexArray;

	Renderent::Ref<Renderent::VertexArray> m_SquareVA;
	Renderent::Ref<Renderent::Shader> m_FlatColorShader, m_TextureShader;

	Renderent::OrthographicCamera m_Camera;
	glm::vec3 m_CameraPosition;

	float m_CameraRotation = 0.0f;

	float m_CameraMoveSpeed = 2.0f;
	float m_CameraRotationSpeed = 180.0f;

	glm::vec4 m_SquaresColor = { 0.0f, 0.0f, 0.0f, 1.0f };

	Renderent::Ref<Renderent::Texture2D> m_Texture, m_ChernoLogoTexture;


};

class Sandbox : public Renderent::Application {
public:
	Sandbox() {
		PushLayer(new ExampleLayer());
	}

	Sandbox(const Renderent::WindowProps& props)
	: Renderent::Application(props) {
		PushLayer(new ExampleLayer());
	}

	~Sandbox() {

	}
};


Renderent::Application* Renderent::CreateApplication() {
	return new Sandbox(Renderent::WindowProps("Sandbox App", 1920, 1080, true));
}