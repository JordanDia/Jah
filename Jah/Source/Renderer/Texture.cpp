#include "Texture.h"

#include "stb_image.h"
#include <glad/glad.h>

namespace Jah {

	Texture2D::Texture2D(uint32_t width, uint32_t height, TextureFormat format)
		: m_Width(width), m_Height(height)
	{
		switch (format)
		{
			case TextureFormat::Red:
			{
				m_InternalFormat = GL_RED;
				m_DataFormat = GL_RED;
				break;
			}
			case TextureFormat::RGB:
			{
				m_InternalFormat = GL_RGB8;
				m_DataFormat = GL_RGB;
				break;
			}
			case TextureFormat::RGBA:
			{
				m_InternalFormat = GL_RGBA8;
				m_DataFormat = GL_RGBA;
				break;
			}
		}

		//m_InternalFormat = GL_RGBA8;
		//m_DataFormat = GL_RGBA;

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	Texture2D::Texture2D(uint32_t rendererID, uint32_t width, uint32_t height, TextureFormat format)
		: m_RendererID(rendererID), m_Width(width), m_Height(height), m_OwnsTexture(false)
	{
		m_InternalFormat = GL_RGBA8;
		m_DataFormat = GL_RGBA;
	}

	Texture2D::Texture2D(const std::string& path)
		: m_Path(path)
	{
		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		JAH_ASSERT(data, "Failed to load image!");
		m_Width = width;
		m_Height = height;

		GLenum internalFormat = 0;
		GLenum dataFormat = 0;

		if (channels == 4)
		{
			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
		}
		else if (channels == 3)
		{
			internalFormat = GL_RGB8;
			dataFormat = GL_RGB;
		}

		JAH_ASSERT(internalFormat & dataFormat, "Unsupported image format!");

		m_InternalFormat = internalFormat;
		m_DataFormat = dataFormat;

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, internalFormat, m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);
	}

	Texture2D::~Texture2D()
	{
		if (m_OwnsTexture)
			glDeleteTextures(1, &m_RendererID);
	}

	void Texture2D::SetData(void* data, uint32_t size)
	{
		uint32_t bpp = 4;

		switch (m_DataFormat)
		{
			case GL_RED: bpp = 1; break;
			case GL_RGB: bpp = 3; break;
			case GL_RGBA: bpp = 4; break;
		}

		JAH_ASSERT(size == m_Width * m_Height * bpp, "Data must be entire texture!");
		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
	}

	void Texture2D::Bind(uint32_t slot) const
	{
		glBindTextureUnit(slot, m_RendererID);
	}

}