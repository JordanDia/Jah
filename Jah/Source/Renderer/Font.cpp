#include "jahpch.h"
#include "Font.h"

#include <ft2build.h>
#include FT_FREETYPE_H

namespace Jah {
	

	Font::Font(const std::filesystem::path& filepath, float size)
		: m_Filepath(filepath), m_Size(size)
	{
		std::ifstream file(filepath, std::ios::binary | std::ios::ate);
		JAH_ASSERT(file, "Failed to open filepath: {}", filepath.string());

		auto fileSize = file.tellg();
		m_FontBuffer.resize(fileSize);
		file.seekg(0, std::ios::beg);
		file.read((char*)m_FontBuffer.data(), fileSize);

		LoadFromFile();
	}

	void Font::LoadFromFile()
	{
		FT_Library ft;
		if (FT_Init_FreeType(&ft))
		{
			JAH_ASSERT(false, "Failed to init FreeType!");
			return;
		}

		FT_Face face;
		if (FT_New_Face(ft, m_Filepath.string().c_str(), 0, &face))
		{
			JAH_ASSERT(false, "Failed to init FreeType!");
			return;
		}
		
		FT_Set_Pixel_Sizes(face, 0, static_cast<uint32_t>(m_Size));
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		for (unsigned char c = 32; c < 127; c++)
		{
			if (FT_Load_Char(face, c, FT_LOAD_RENDER))
				continue;

			auto& glyph = face->glyph;

			Shared<Texture2D> texture = CreateShared<Texture2D>(
				glyph->bitmap.width,
				glyph->bitmap.rows,
				TextureFormat::Red
			);

			uint32_t size = glyph->bitmap.width * glyph->bitmap.rows;
			texture->SetData(glyph->bitmap.buffer, size);

			Glyph character;
			character.Texture = texture;
			character.Size = { glyph->bitmap.width, glyph->bitmap.rows };
			character.Bearing = { glyph->bitmap_left, glyph->bitmap_top };
			character.Advance = glyph->advance.x >> 6;

			m_Glyphs.emplace(c, character);
		}

		FT_Done_Face(face);
		FT_Done_FreeType(ft);	
	}

	const Glyph& Font::GetGlyph(char c)
	{
		auto it = m_Glyphs.find(c);
		JAH_ASSERT(it != m_Glyphs.end(), "Glyph {} not found!", c);
		return it->second;
	}

}