#include "repch.h"
#include "Renderer2D.h"

#include "Renderent/Renderer/RenderCommand.h"
#include "Renderent/Renderer/Shader.h"
#include "Renderent/Renderer/VertexArray.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Renderent {

	struct QuadVertex {
		glm::vec3 position;
		glm::vec2 texCoord;
		glm::vec4 color;
		float texIndex;
		float texScale;
	};

	struct Renderer2DStorage {

		Ref<VertexArray> QuadVertexArray;
		Ref<Shader> TextureShader;
		Ref<Texture2D> WhiteTexture;
		Ref<VertexBuffer> QuadVertexBuffer;

		const uint32_t MaxQuads = 10000;
		const uint32_t MaxVertices = MaxQuads * 4;
		const uint32_t MaxIndices = MaxQuads * 6;
		static const uint32_t MaxTextureSlots = 32; // TODO: Renderer Capabilities

		uint32_t QuadIndexCount = 0;

		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr = nullptr;

		std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;
		uint32_t TextureSlotIndex = 1;

		glm::vec4 QuadVertexPositions[4];

		Renderer2D::Statistics Stats;
	};

	static Renderer2DStorage s_Data;

	void Renderer2D::Init() {
		RE_PROFILE_FUNCTION();


		s_Data.QuadVertexArray = Renderent::VertexArray::Create();

		s_Data.QuadVertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(QuadVertex));

		s_Data.QuadVertexBuffer->SetLayout({
			{ShaderDataType::Float3, "a_Position"},
			{ShaderDataType::Float2, "a_TexCoord"},
			{ShaderDataType::Float4, "a_Color"},
			{ShaderDataType::Float, "a_TexIndex"},
			{ShaderDataType::Float, "a_TexScale"}
		});

		s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer);

		s_Data.QuadVertexBufferBase = new QuadVertex[s_Data.MaxVertices];

		uint32_t* quadIndices = new uint32_t[s_Data.MaxIndices];

		uint32_t offset = 0;
		for (uint32_t i = 0; i < s_Data.MaxIndices; i += 6) {
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;
			
			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;

			offset += 4;

		}

		Ref<IndexBuffer> quadIB = IndexBuffer::Create(s_Data.MaxIndices, quadIndices);
		s_Data.QuadVertexArray->AddIndexBuffer(quadIB);
		delete[] quadIndices;


		s_Data.WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_Data.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		int32_t samplers[s_Data.MaxTextureSlots];
		for (uint32_t i = 0; i < s_Data.MaxTextureSlots; i++) {
			samplers[i] = i;
		}

		s_Data.TextureShader = Shader::Create("assets/shader/Texture.glsl");
		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetIntArray(samplers, s_Data.MaxTextureSlots, "u_Textures");

		s_Data.TextureSlots[0] = s_Data.WhiteTexture;

		s_Data.QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		s_Data.QuadVertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
		s_Data.QuadVertexPositions[2] = { 0.5f, 0.5f, 0.0f, 1.0f };
		s_Data.QuadVertexPositions[3] = { -0.5f, 0.5f, 0.0f, 1.0f };

	}
	void Renderer2D::Shutdown() {
		RE_PROFILE_FUNCTION();
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera) {
		RE_PROFILE_FUNCTION();
		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetMat4(camera.GetViewProjectionMatrix(), "u_ViewProjection");

		s_Data.QuadIndexCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

		s_Data.TextureSlotIndex = 1;

	}
	void Renderer2D::EndScene() {
		RE_PROFILE_FUNCTION();
		
		uint32_t dataSize = (uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase;
		s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBufferBase, dataSize);

		Flush();
	}

	void Renderer2D::Flush() {

		for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++) {
			s_Data.TextureSlots[i]->Bind(i);
		}

		RenderCommand::DrawIndexed(s_Data.QuadVertexArray, s_Data.QuadIndexCount);
		s_Data.Stats.DrawCalls++;
	}

	void Renderer2D::FlushandReset() {
		EndScene();

		s_Data.QuadIndexCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

		s_Data.TextureSlotIndex = 1;
	}

	// Primitives

	// Quad with Color
	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color) {
		DrawQuad({ position.x, position.y, 0.0f }, size, color);
	}

	
	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color) {

		RE_PROFILE_FUNCTION();

		if (s_Data.QuadIndexCount >= s_Data.MaxIndices) {
			FlushandReset();
		}
		
		float texIndex = 0.0f;
		float texScale = 1.0f;

		constexpr glm::vec2 textureCoords[] = {
			{0.0f, 0.0f},
			{1.0f, 0.0f},
			{1.0f, 1.0f},
			{0.0f, 1.0f}
		};

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position);
		transform = glm::scale(transform, { size.x, size.y, 1.0f });

		for (int i = 0; i < 4; i++) {
			s_Data.QuadVertexBufferPtr->position = transform * s_Data.QuadVertexPositions[i];
			s_Data.QuadVertexBufferPtr->color = color;
			s_Data.QuadVertexBufferPtr->texCoord = textureCoords[i];
			s_Data.QuadVertexBufferPtr->texIndex = texIndex;
			s_Data.QuadVertexBufferPtr->texScale = texScale;
			s_Data.QuadVertexBufferPtr++;

		}

		s_Data.QuadIndexCount += 6;

		s_Data.Stats.QuadCount++;
	}


	// Quad with texture
	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float textureScale) {
		DrawQuad({ position.x, position.y, 0.0f }, size, texture, textureScale);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float textureScale) {
		
		RE_PROFILE_FUNCTION();

		if (s_Data.QuadIndexCount >= s_Data.MaxIndices) {
			FlushandReset();
		}

	
		constexpr glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };

		float textureIndex = 0.0f;

		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++) {
			if (*s_Data.TextureSlots[i].get() == *texture.get()) {
				textureIndex = (float) i;
				break;
			}
		}

		constexpr glm::vec2 textureCoords[] = {
			{0.0f, 0.0f},
			{1.0f, 0.0f},
			{1.0f, 1.0f},
			{0.0f, 1.0f}
		};

		if (textureIndex == 0.0f) {
			textureIndex = (float)s_Data.TextureSlotIndex;
			s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
			s_Data.TextureSlotIndex++;
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position);
		transform = glm::scale(transform, { size.x, size.y, 1.0f });

		for (int i = 0; i < 4; i++) {
			s_Data.QuadVertexBufferPtr->position = transform * s_Data.QuadVertexPositions[i];
			s_Data.QuadVertexBufferPtr->color = color;
			s_Data.QuadVertexBufferPtr->texCoord = textureCoords[i];
			s_Data.QuadVertexBufferPtr->texIndex = textureIndex;
			s_Data.QuadVertexBufferPtr->texScale = textureScale;
			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.QuadIndexCount += 6;

		s_Data.Stats.QuadCount++;
	
	}

	// Quad with subtexture
	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<SubTexture2D>& subtexture, float textureScale) {
		DrawQuad({ position.x, position.y, 0.0f }, size, subtexture, textureScale);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<SubTexture2D>& subtexture, float textureScale) {

		RE_PROFILE_FUNCTION();

		if (s_Data.QuadIndexCount >= s_Data.MaxIndices) {
			FlushandReset();
		}

		constexpr glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
		const glm::vec2* textureCoords = subtexture->GetTextureCoords();
		const Ref<Texture2D> texture = subtexture->GetTexture();

		float textureIndex = 0.0f;

		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++) {
			if (*s_Data.TextureSlots[i].get() == *texture.get()) {
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f) {
			textureIndex = (float)s_Data.TextureSlotIndex;
			s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
			s_Data.TextureSlotIndex++;
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position);
		transform = glm::scale(transform, { size.x, size.y, 1.0f });

		for (int i = 0; i < 4; i++) {
			s_Data.QuadVertexBufferPtr->position = transform * s_Data.QuadVertexPositions[i];
			s_Data.QuadVertexBufferPtr->color = color;
			s_Data.QuadVertexBufferPtr->texCoord = textureCoords[i];
			s_Data.QuadVertexBufferPtr->texIndex = textureIndex;
			s_Data.QuadVertexBufferPtr->texScale = textureScale;
			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.QuadIndexCount += 6;

		s_Data.Stats.QuadCount++;
	}

	// Quad with texture and tint color

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& tintColor, float textureScale)
	{

		DrawQuad({ position.x, position.y, 0.0f }, size, texture, tintColor, textureScale);

	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& tintColor, float textureScale)
	{
		RE_PROFILE_FUNCTION();

		if (s_Data.QuadIndexCount >= s_Data.MaxIndices) {
			FlushandReset();
		}

		glm::vec2 halfSize = size / 2.0f;

		float textureIndex = 0.0f;

		constexpr glm::vec2 textureCoords[] = {
			{0.0f, 0.0f},
			{1.0f, 0.0f},
			{1.0f, 1.0f},
			{0.0f, 1.0f}
		};

		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++) {
			if (*s_Data.TextureSlots[i].get() == *texture.get()) {
				textureIndex = (float)i;
				break;
			}
		}
		if (textureIndex == 0.0f) {
			textureIndex = (float)s_Data.TextureSlotIndex;
			s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
			s_Data.TextureSlotIndex++;
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position);
		transform = glm::scale(transform, { size.x, size.y, 1.0f });

		for (int i = 0; i < 4; i++) {
			s_Data.QuadVertexBufferPtr->position = transform * s_Data.QuadVertexPositions[i];
			s_Data.QuadVertexBufferPtr->color = tintColor;
			s_Data.QuadVertexBufferPtr->texCoord = textureCoords[i];
			s_Data.QuadVertexBufferPtr->texIndex = textureIndex;
			s_Data.QuadVertexBufferPtr->texScale = textureScale;
			s_Data.QuadVertexBufferPtr++;
		}
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadIndexCount += 6;

		s_Data.Stats.QuadCount++;

	}

	// Quad with subtexture and tint color
	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<SubTexture2D>& subtexture, const glm::vec4& tintColor, float textureScale) {
		DrawQuad({ position.x, position.y, 0.0f }, size, subtexture, tintColor, textureScale);
	}
	
	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<SubTexture2D>& subtexture, const glm::vec4& tintColor, float textureScale) {
		RE_PROFILE_FUNCTION();

		if (s_Data.QuadIndexCount >= s_Data.MaxIndices) {
			FlushandReset();
		}

		const glm::vec2* textureCoords = subtexture->GetTextureCoords();
		const Ref<Texture2D> texture = subtexture->GetTexture();
		float textureIndex = 0.0f;

		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++) {
			if (*s_Data.TextureSlots[i].get() == *texture.get()) {
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f) {
			textureIndex = (float)s_Data.TextureSlotIndex;
			s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
			s_Data.TextureSlotIndex++;
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position);
		transform = glm::scale(transform, { size.x, size.y, 1.0f });

		for (int i = 0; i < 4; i++) {
			s_Data.QuadVertexBufferPtr->position = transform * s_Data.QuadVertexPositions[i];
			s_Data.QuadVertexBufferPtr->color = tintColor;
			s_Data.QuadVertexBufferPtr->texCoord = textureCoords[i];
			s_Data.QuadVertexBufferPtr->texIndex = textureIndex;
			s_Data.QuadVertexBufferPtr->texScale = textureScale;
			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.QuadIndexCount += 6;

		s_Data.Stats.QuadCount++;
	}

	// Rotated Quad with a color
	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4 color)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, color);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4 color)
	{
		RE_PROFILE_FUNCTION();

		if (s_Data.QuadIndexCount >= s_Data.MaxIndices) {
			FlushandReset();
		}

		float textureIndex = 0.0f;
		float textureScale = 1.0f;

		constexpr glm::vec2 textureCoords[] = {
			{0.0f, 0.0f},
			{1.0f, 0.0f},
			{1.0f, 1.0f},
			{0.0f, 1.0f}
		};

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position);
		transform = glm::rotate(transform, rotation, { 0.0f, 0.0f, 1.0f });
		transform = glm::scale(transform, { size.x, size.y, 1.0f });

		for (int i = 0; i < 4; i++) {
			s_Data.QuadVertexBufferPtr->position = transform * s_Data.QuadVertexPositions[i];
			s_Data.QuadVertexBufferPtr->color = color;
			s_Data.QuadVertexBufferPtr->texCoord = textureCoords[i];
			s_Data.QuadVertexBufferPtr->texIndex = textureIndex;
			s_Data.QuadVertexBufferPtr->texScale = textureScale;
			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.QuadIndexCount += 6;

		s_Data.Stats.QuadCount++;
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float textureScale)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, texture, textureScale);
	}
	
	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float textureScale)
	{

		RE_PROFILE_FUNCTION();

		if (s_Data.QuadIndexCount >= s_Data.MaxIndices) {
			FlushandReset();
		}

		constexpr glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };

		float textureIndex = 0.0f;

		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++) {
			if (*s_Data.TextureSlots[i].get() == *texture.get()) {
				textureIndex = (float)i;
				break;
			}
		}
		if (textureIndex == 0.0f) {
			textureIndex = (float)s_Data.TextureSlotIndex;
			s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
			s_Data.TextureSlotIndex++;
		}

		constexpr glm::vec2 textureCoords[] = {
			{0.0f, 0.0f},
			{1.0f, 0.0f},
			{1.0f, 1.0f},
			{0.0f, 1.0f}
		};

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position);
		transform = glm::rotate(transform, rotation, { 0.0f, 0.0f, 1.0f });
		transform = glm::scale(transform, { size.x, size.y, 1.0f });

		for (int i = 0; i < 4; i++) {
			s_Data.QuadVertexBufferPtr->position = transform * s_Data.QuadVertexPositions[i];
			s_Data.QuadVertexBufferPtr->color = color;
			s_Data.QuadVertexBufferPtr->texCoord = textureCoords[i];
			s_Data.QuadVertexBufferPtr->texIndex = textureIndex;
			s_Data.QuadVertexBufferPtr->texScale = textureScale;
			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.QuadIndexCount += 6;

		s_Data.Stats.QuadCount++;
	}

	// Rotated quad with a subtexture
	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<SubTexture2D>& subtexture, float textureScale) {
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, subtexture, textureScale);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<SubTexture2D>& subtexture, float textureScale) {
		RE_PROFILE_FUNCTION();
		
		if (s_Data.QuadIndexCount >= s_Data.MaxIndices) {
			FlushandReset();
		}

		constexpr glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };

		float textureIndex = 0.0f;

		const glm::vec2* textureCoords = subtexture->GetTextureCoords();
		const Ref<Texture2D> texture = subtexture->GetTexture();

		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++) {
			if (*s_Data.TextureSlots[i].get() == *texture.get()) {
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f) {
			textureIndex = (float)s_Data.TextureSlotIndex;
			s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
			s_Data.TextureSlotIndex++;
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position);
		transform = glm::rotate(transform, rotation, { 0.0f, 1.0f, 0.0f });
		transform = glm::scale(transform, { size.x, size.y, 1.0f });

		for (int i = 0; i < 4; i++) {
			s_Data.QuadVertexBufferPtr->position = transform * s_Data.QuadVertexPositions[i];
			s_Data.QuadVertexBufferPtr->color = color;
			s_Data.QuadVertexBufferPtr->texCoord = textureCoords[i];
			s_Data.QuadVertexBufferPtr->texIndex = textureIndex;
			s_Data.QuadVertexBufferPtr->texScale = textureScale;
			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.QuadIndexCount += 6;

		s_Data.Stats.QuadCount++;
	}

	// Rotated quad with texture and tint color
	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, const glm::vec4 color, float textureScale)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, texture, color, textureScale);
	}
	
	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, const glm::vec4 color, float textureScale)
	{
		RE_PROFILE_FUNCTION();

		if (s_Data.QuadIndexCount >= s_Data.MaxIndices) {
			FlushandReset();
		}

		constexpr glm::vec2 textureCoords[]{
			{0.0f, 0.0f},
			{1.0f, 0.0f},
			{1.0f, 1.0f},
			{0.0f, 1.0f}
		};

		float textureIndex = 0.0f;

		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++) {
			if (*s_Data.TextureSlots[i].get() == *texture.get()) {
				textureIndex = (float)i;
				break;
			}
		}
		if (textureIndex == 0.0f) {
			textureIndex = (float)s_Data.TextureSlotIndex;
			s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
			s_Data.TextureSlotIndex++;
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position);
		transform = glm::rotate(transform, rotation, { 0.0f, 0.0f, 1.0f });
		transform = glm::scale(transform, { size.x, size.y, 1.0f });

		for (int i = 0; i < 4; i++) {
			s_Data.QuadVertexBufferPtr->position = transform * s_Data.QuadVertexPositions[i];
			s_Data.QuadVertexBufferPtr->color = color;
			s_Data.QuadVertexBufferPtr->texCoord = textureCoords[i];
			s_Data.QuadVertexBufferPtr->texIndex = textureIndex;
			s_Data.QuadVertexBufferPtr->texScale = textureScale;
			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.QuadIndexCount += 6;

		s_Data.Stats.QuadCount++;
	}

	// Rotated quad with subtexture and tint color
	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<SubTexture2D>& subtexture, const glm::vec4 color, float textureScale)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, subtexture, color, textureScale);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<SubTexture2D>& subtexture, const glm::vec4 color, float textureScale) {
		
		RE_PROFILE_FUNCTION();

		if (s_Data.QuadIndexCount >= s_Data.MaxIndices) {
			FlushandReset();
		}

		float textureIndex = 0.0f;

		const glm::vec2* textureCoords = subtexture->GetTextureCoords();
		const Ref<Texture2D> texture = subtexture->GetTexture();

		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++) {
			if (*s_Data.TextureSlots[i].get() == *texture.get()) {
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f) {
			textureIndex = (float)s_Data.TextureSlotIndex;
			s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
			s_Data.TextureSlotIndex++;
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position);
		transform = glm::rotate(transform, rotation, { 0.0f, 1.0f, 0.0f });
		transform = glm::scale(transform, { size.x, size.y, 1.0f });

		for (int i = 0; i < 4; i++) {
			s_Data.QuadVertexBufferPtr->position = transform * s_Data.QuadVertexPositions[i];
			s_Data.QuadVertexBufferPtr->color = color;
			s_Data.QuadVertexBufferPtr->texCoord = textureCoords[i];
			s_Data.QuadVertexBufferPtr->texIndex = textureIndex;
			s_Data.QuadVertexBufferPtr->texScale = textureScale;
			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.QuadIndexCount += 6;

		s_Data.Stats.QuadCount++;
	}

	void Renderer2D::ResetStats()
	{
		memset(&s_Data.Stats, 0, sizeof(Statistics));
	}

	Renderer2D::Statistics Renderer2D::GetStats()
	{
		return s_Data.Stats;
	}

	
	
}