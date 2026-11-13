#pragma once
#include <filesystem>
#include <vector>
#include <unordered_map>
#include "Texture.h"



namespace Jah {

	struct Glyph
	{
		char Character;
		Shared<Texture2D> Texture;
		glm::ivec2 Size;
		glm::ivec2 Bearing;
		uint32_t Advance;
	};



	class Font
	{

	public:
		
		Font() = default;
		Font(const Font&) = default;
		Font(const std::filesystem::path& filepath, float size);
		~Font() = default;

		
		const Glyph& GetGlyph(char c);

	private:
		void LoadFromFile();

	private:
		std::filesystem::path m_Filepath;
		float m_Size = 32;
		Shared<Texture2D> m_Texture;
		std::unordered_map<char, Glyph> m_Glyphs;

		std::vector<uint8_t> m_FontBuffer;
	};

}