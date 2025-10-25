#pragma once

#include <cstdint>
#include <string>
#include "Core/Core.h"

#include <glad/glad.h>

namespace Jah {

	class Texture
	{
	public:
		virtual ~Texture() = default;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual void Bind(uint32_t slot = 0) const = 0;
	};

	class Texture2D : public Texture
	{
	public:
		Texture2D(const std::string& path);
		Texture2D(uint32_t width, uint32_t height);
		~Texture2D();

		uint32_t GetWidth() const override { return m_Width; }
		uint32_t GetHeight() const override { return m_Height; }

		void SetData(void* data, uint32_t size);

		void Bind(uint32_t slot = 0) const override;

		bool operator==(const Texture2D& other) const
		{
			return m_RendererID == other.m_RendererID;
		}

	private:
		std::string m_Path;
		uint32_t m_Width = 0;
		uint32_t m_Height = 0;
		uint32_t m_RendererID = 0;
		GLenum m_InternalFormat;
		GLenum m_DataFormat;
	};

}