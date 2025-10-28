#include "jahpch.h"
#include "SubTexture2D.h"

namespace Jah {



	SubTexture2D::SubTexture2D(const Shared<Texture2D>& texture, const glm::vec2& min, const glm::vec2& max)
		: m_Texture(texture)
	{
		m_TexCoords[0] = { min.x, min.y };
		m_TexCoords[1] = { max.x, min.y };
		m_TexCoords[2] = { max.x, max.y };
		m_TexCoords[3] = { min.x, max.y };
	}

	Shared<SubTexture2D> SubTexture2D::CreateFromCoords(const Shared<Texture2D>& texture, const glm::vec2& coords, const glm::vec2& spriteSize)
	{
		uint32_t sheetWidth = texture->GetWidth();
		uint32_t sheetHeight = texture->GetHeight();

		glm::vec2 min = { (coords.x * spriteSize.x) / sheetWidth, (coords.y * spriteSize.y) / sheetHeight };
		glm::vec2 max = { ((coords.x + 1) * spriteSize.x) / sheetWidth, ((coords.y + 1) * spriteSize.y) / sheetHeight };

		return std::make_shared<SubTexture2D>(texture, min, max);
	}

}