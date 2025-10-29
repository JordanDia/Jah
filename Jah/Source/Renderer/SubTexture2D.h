#pragma once

#include <glm/glm.hpp>
#include <memory>
#include "Texture.h"

namespace Jah {

	class SubTexture2D
	{
	public:
		SubTexture2D(const Shared<Texture2D>& texture, const glm::vec2& min, const glm::vec2& max);

		const Shared<Texture2D> GetTexture() const { return m_Texture; }
		const glm::vec2* GetTexCoords() const { return m_TexCoords; }

		static Shared<SubTexture2D> CreateFromCoords(const Shared<Texture2D>& texture, const glm::vec2& coords, const glm::vec2& spriteSize);

	private:
		Shared<Texture2D> m_Texture;
		glm::vec2 m_TexCoords[4]{};
	};

}