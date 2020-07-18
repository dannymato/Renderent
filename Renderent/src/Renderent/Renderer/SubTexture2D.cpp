#include "repch.h"
#include "SubTexture2D.h"

namespace Renderent {



	SubTexture2D::SubTexture2D(const Ref<Texture2D>& texture, const glm::vec2& min, const glm::vec2& max)
		: m_Texture(texture)
	{
		m_TextureCoords[0] = { min.x, min.y };
		m_TextureCoords[1] = { max.x, min.y };
		m_TextureCoords[2] = { max.x, max.y };
		m_TextureCoords[3] = { min.x, max.y };
	}

	Ref<SubTexture2D> SubTexture2D::CreateFromCoords(const Ref<Texture2D>& texture, const glm::vec2& coords, const glm::vec2& cellsize, const glm::vec2& spriteSize)
	{
		glm::vec2 min = { (coords.x * cellsize.x) / texture->GetWidth(), (coords.y * cellsize.y) / texture->GetHeight() };
		glm::vec2 max = { ((coords.x + spriteSize.x) * cellsize.x) / texture->GetWidth(), ((coords.y + spriteSize.y) * cellsize.y) / texture->GetHeight() };

		return CreateRef<SubTexture2D>(texture, min, max);
	}

}