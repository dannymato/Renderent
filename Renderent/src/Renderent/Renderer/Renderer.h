#pragma once

#include "Renderent/Renderer/Camera.h"
#include "Renderent/Renderer/RenderCommand.h"
#include "Renderent/Renderer/Shader.h"

namespace Renderent{

	class Renderer {
	public:

		static void Init();
		static void OnWindowResized(uint32_t width, uint32_t height);

		static void BeginScene(OrthographicCamera& camera);
		static void EndScene();

		static void Submit(const Ref<VertexArray>& vertexArray,
			const Ref<Shader>& shader,
			const glm::mat4& transform = glm::mat4(1.0f));

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};

		static SceneData* m_sceneData;
	};

}