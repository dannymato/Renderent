#include "repch.h"
#include "Renderer.h"

#include "Renderent/Renderer/Renderer2D.h"

namespace Renderent {

	Renderer::SceneData* Renderer::m_sceneData = new Renderer::SceneData;

	void Renderer::BeginScene(OrthographicCamera& camera)
	{
		m_sceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::Submit(const Ref<VertexArray>& vertexArray,
		const Ref<Shader>& shader, const glm::mat4& transform)
	{
		shader->Bind();
		shader->SetMat4(m_sceneData->ViewProjectionMatrix, "u_ViewProjection");
		shader->SetMat4(transform, "u_Transform");
		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}

	void Renderer::Init() {
		RE_PROFILE_FUNCTION();

		RenderCommand::Init();
		Renderer2D::Init();

	}

	void Renderer::OnWindowResized(uint32_t width, uint32_t height)
	{
		RenderCommand::SetViewport(0, 0, width, height);
	}

}