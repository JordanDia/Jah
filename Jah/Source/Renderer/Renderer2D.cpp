#include "jahpch.h"
#include "Renderer2D.h"

#include "Renderer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

#include <glm/gtc/matrix_transform.hpp>
#include <array>
#include "Core/Log.h"

namespace Jah {

	struct QuadVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;
		float TexIndex;
	};

	struct Renderer2DData
	{
		static constexpr uint32_t MaxQuads = 20000;
		static constexpr uint32_t MaxVertices = MaxQuads * 4;
		static constexpr uint32_t MaxIndices = MaxQuads * 6;
		static constexpr uint32_t MaxTextureSlots = 32;

		Shared<VertexArray> QuadVertexArray;
		Shared<VertexBuffer> QuadVertexBuffer;
		Shared<Shader> TextureShader;
		Shared<Texture2D> WhiteTexture;

		uint32_t QuadIndexCount = 0;
		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr = nullptr;

		std::array<Shared<Texture2D>, MaxTextureSlots> TextureSlots;
		uint32_t TextureSlotIndex = 1;

		glm::vec4 QuadVertexPositions[4]{};

		Renderer2D::Statistics Stats;
	};

	static Renderer2DData Data;

	void Renderer2D::Init()
	{
		Data.QuadVertexArray = std::make_shared<Jah::VertexArray>();

		Data.QuadVertexBuffer = std::make_shared<VertexBuffer>(Data.MaxVertices * sizeof(QuadVertex));
		Data.QuadVertexBuffer->SetLayout({
			{ Jah::ShaderDataType::Float3, "a_Position" },
			{ Jah::ShaderDataType::Float4, "a_Color" },
			{ Jah::ShaderDataType::Float2, "a_TexCoord" },
			{ Jah::ShaderDataType::Float, "a_TexIndex" },
		});
		Data.QuadVertexArray->AddVertexBuffer(Data.QuadVertexBuffer);

		Data.QuadVertexBufferBase = new QuadVertex[Data.MaxVertices];

		uint32_t* quadIndices = new uint32_t[Data.MaxIndices];

		uint32_t offset = 0;
		for (uint32_t i = 0; i < Data.MaxIndices; i += 6)
		{
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;

			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;

			offset += 4;
		}

		std::shared_ptr<Jah::IndexBuffer> quadIB = std::make_shared<IndexBuffer>(quadIndices, Data.MaxIndices);
		Data.QuadVertexArray->SetIndexBuffer(quadIB);
		delete[] quadIndices;


		Data.WhiteTexture = std::make_shared<Texture2D>(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		Data.WhiteTexture->SetData(&whiteTextureData, sizeof(whiteTextureData));

		int32_t samplers[Data.MaxTextureSlots];
		for (uint32_t i = 0; i < Data.MaxTextureSlots; i++)
			samplers[i] = i;

		Data.TextureShader = std::make_shared<Jah::Shader>("Assets/Shaders/Texture.glsl");
		Data.TextureShader->Bind();
		Data.TextureShader->UploadUniformIntArray("u_Textures", samplers, Data.MaxTextureSlots);
		Data.TextureSlots[0] = Data.WhiteTexture;

		Data.QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		Data.QuadVertexPositions[1] = {  0.5f, -0.5f, 0.0f, 1.0f };
		Data.QuadVertexPositions[2] = {  0.5f,  0.5f, 0.0f, 1.0f };
		Data.QuadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };
	}

	void Renderer2D::Shutdown()
	{
		
	}

	void Renderer2D::BeginScene(Camera& camera, glm::mat4& transform)
	{
		glm::mat4 viewProj = camera.GetProjection() * glm::inverse(transform);

		Data.TextureShader->Bind();
		Data.TextureShader->UploadUniformMat4("u_ViewProjection", viewProj);

		Data.QuadIndexCount = 0;
		Data.QuadVertexBufferPtr = Data.QuadVertexBufferBase;

		Data.TextureSlotIndex = 1;
	}

	void Renderer2D::BeginScene(OrthographicCamera& camera)
	{
		Data.TextureShader->Bind();
		Data.TextureShader->UploadUniformMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

		Data.QuadIndexCount = 0;
		Data.QuadVertexBufferPtr = Data.QuadVertexBufferBase;

		Data.TextureSlotIndex = 1;
	}

	

	void Renderer2D::BeginScene(EditorCamera& camera)
	{

		glm::mat4 viewProj = camera.GetViewProjection();

		Data.TextureShader->Bind();
		Data.TextureShader->UploadUniformMat4("u_ViewProjection", viewProj);

		Data.QuadIndexCount = 0;
		Data.QuadVertexBufferPtr = Data.QuadVertexBufferBase;

		Data.TextureSlotIndex = 1;
	}

	void Renderer2D::EndScene()
	{
		uint32_t dataSize = uint32_t((uint8_t*)Data.QuadVertexBufferPtr - (uint8_t*)Data.QuadVertexBufferBase);
		Data.QuadVertexBuffer->SetData(Data.QuadVertexBufferBase, dataSize);

		Flush();
	}

	void Renderer2D::Flush()
	{
		if (Data.QuadIndexCount) {

			for (uint32_t i = 0; i < Data.TextureSlotIndex; i++)
			{
				Data.TextureSlots[i]->Bind(i);
			}

			Renderer::DrawIndexed(Data.QuadVertexArray, Data.QuadIndexCount);

			Data.Stats.DrawCalls++;
		}
	}

	void Renderer2D::FlushAndReset()
	{
		EndScene();
		Data.QuadIndexCount = 0;
		Data.QuadVertexBufferPtr = Data.QuadVertexBufferBase;
		Data.TextureSlotIndex = 1;
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0 }, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		if (Data.QuadIndexCount >= Renderer2DData::MaxIndices)
			FlushAndReset();

		const float whiteTextureIndex = 0.0f;

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		for (int i = 0; i < 4; i++)
		{
			Data.QuadVertexBufferPtr->Position = transform * Data.QuadVertexPositions[i];
			Data.QuadVertexBufferPtr->Color = color;

			glm::vec2 texCoord = { 0.0f, 0.0 };
			if (i == 1)
				texCoord = { 1.0f, 0.0f };
			else if (i == 2)
				texCoord = { 1.0f, 1.0f };
			else if (i == 3)
				texCoord = { 0.0f, 1.0f };
			
			Data.QuadVertexBufferPtr->TexCoord = texCoord;
			Data.QuadVertexBufferPtr->TexIndex = whiteTextureIndex;
			Data.QuadVertexBufferPtr++;

		}

		Data.QuadIndexCount += 6;

		Data.Stats.QuadCount++;
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Shared<Texture2D> texture)
	{
		DrawQuad({ position.x, position.y, 0 }, size, texture);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Shared<Texture2D> texture)
	{
		if (Data.QuadIndexCount >= Renderer2DData::MaxIndices)
			FlushAndReset();

		constexpr glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };

		float textureIndex = 0.0f;

		for (uint32_t i = 1; i < Data.TextureSlotIndex; i++)
		{
			if (*Data.TextureSlots[i] == *texture)
			{
				textureIndex = i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			textureIndex = (float)Data.TextureSlotIndex;
			Data.TextureSlots[Data.TextureSlotIndex] = texture;
			Data.TextureSlotIndex++;
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });


		for (int i = 0; i < 4; i++)
		{
			Data.QuadVertexBufferPtr->Position = transform * Data.QuadVertexPositions[i];
			Data.QuadVertexBufferPtr->Color = color;

			glm::vec2 texCoord = { 0.0f, 0.0 };
			if (i == 1)
				texCoord = { 1.0f, 0.0f };
			else if (i == 2)
				texCoord = { 1.0f, 1.0f };
			else if (i == 3)
				texCoord = { 0.0f, 1.0f };

			Data.QuadVertexBufferPtr->TexCoord = texCoord;
			Data.QuadVertexBufferPtr->TexIndex = textureIndex;
			Data.QuadVertexBufferPtr++;

		}

		Data.QuadIndexCount += 6;

		Data.Stats.QuadCount++;
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Shared<SubTexture2D> subtexture)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, subtexture);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Shared<SubTexture2D> subtexture)
	{
		if (Data.QuadIndexCount >= Renderer2DData::MaxIndices)
			FlushAndReset();

		constexpr uint32_t quadVertexCount = 4;
		const glm::vec2* texCoords = subtexture->GetTexCoords();
		const Shared<Texture2D> texture = subtexture->GetTexture();

		constexpr glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };

		float textureIndex = 0.0f;

		for (uint32_t i = 1; i < Data.TextureSlotIndex; i++)
		{
			if (*Data.TextureSlots[i] == *texture)
			{
				textureIndex = i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			textureIndex = (float)Data.TextureSlotIndex;
			Data.TextureSlots[Data.TextureSlotIndex] = texture;
			Data.TextureSlotIndex++;
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		for (int i = 0; i < 4; i++)
		{
			Data.QuadVertexBufferPtr->Position = transform * Data.QuadVertexPositions[i];
			Data.QuadVertexBufferPtr->Color = color;
			Data.QuadVertexBufferPtr->TexCoord = texCoords[i];
			Data.QuadVertexBufferPtr->TexIndex = textureIndex;
			Data.QuadVertexBufferPtr++;
		}

		Data.QuadIndexCount += 6;

		Data.Stats.QuadCount++;
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Shared<Texture2D> texture, const glm::vec2& texCoordMin, const glm::vec2& texCoordMax)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, texture, texCoordMin, texCoordMax);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Shared<Texture2D> texture, const glm::vec2& texCoordMin, const glm::vec2& texCoordMax)
	{
		if (Data.QuadIndexCount >= Renderer2DData::MaxIndices)
			FlushAndReset();

		constexpr uint32_t quadVertexCount = 4;

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		glm::vec2 texCoords[] = {
			{ texCoordMin.x, texCoordMin.y },
			{ texCoordMax.x, texCoordMin.y },
			{ texCoordMax.x, texCoordMax.y },
			{ texCoordMin.x, texCoordMax.y },
		};

	

		constexpr glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };

		float textureIndex = 0.0f;

		for (uint32_t i = 1; i < Data.TextureSlotIndex; i++)
		{
			if (*Data.TextureSlots[i] == *texture)
			{
				textureIndex = i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			textureIndex = (float)Data.TextureSlotIndex;
			Data.TextureSlots[Data.TextureSlotIndex] = texture;
			Data.TextureSlotIndex++;
		}

	

		for (int i = 0; i < 4; i++)
		{
			Data.QuadVertexBufferPtr->Position = transform * Data.QuadVertexPositions[i];
			Data.QuadVertexBufferPtr->Color = color;
			Data.QuadVertexBufferPtr->TexCoord = texCoords[i];
			Data.QuadVertexBufferPtr->TexIndex = textureIndex;
			Data.QuadVertexBufferPtr++;
		}

		Data.QuadIndexCount += 6;

		Data.Stats.QuadCount++;
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color)
	{
		if (Data.QuadIndexCount >= Renderer2DData::MaxIndices)
			FlushAndReset();

		constexpr uint32_t quadVertexCount = 4;
		constexpr glm::vec2 textureCoords[] = {
			{ 0.0f, 0.0f },
			{ 1.0f, 0.0f,},
			{ 1.0f, 1.0f },
			{0.0f, 1.0f}
		};
	
		const float whiteTextureIndex = 0.0f;


		for (int i = 0; i < 4; i++)
		{
			Data.QuadVertexBufferPtr->Position = transform * Data.QuadVertexPositions[i];
			Data.QuadVertexBufferPtr->Color = color;
			Data.QuadVertexBufferPtr->TexCoord = textureCoords[i];
			Data.QuadVertexBufferPtr->TexIndex = whiteTextureIndex;
			Data.QuadVertexBufferPtr++;

		}
		Data.QuadIndexCount += 6;

		Data.Stats.QuadCount++;
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const Shared<Texture2D>& texture, const glm::vec2& texCoordMin, const glm::vec2& texCoordMax)
	{
		if (Data.QuadIndexCount >= Renderer2DData::MaxIndices)
			FlushAndReset();

		constexpr uint32_t quadVertexCount = 4;

		glm::vec2 textureCoords[] = {
			{ texCoordMin.x, texCoordMin.y },
			{ texCoordMax.x, texCoordMin.y },
			{ texCoordMax.x, texCoordMax.y },
			{ texCoordMin.x, texCoordMax.y },
		};

		constexpr glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
		float textureIndex = 0.0f;
		for (uint32_t i = 1; i < Data.TextureSlotIndex; i++)
		{
			if (*Data.TextureSlots[i] == *texture)
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			if (Data.QuadIndexCount >= Renderer2DData::MaxIndices)
				FlushAndReset();

			textureIndex = (float)Data.TextureSlotIndex;
			Data.TextureSlots[Data.TextureSlotIndex] = texture;
			Data.TextureSlotIndex++;
		}

		for (int i = 0; i < 4; i++)
		{
			Data.QuadVertexBufferPtr->Position = transform * Data.QuadVertexPositions[i];
			Data.QuadVertexBufferPtr->Color = color;
			Data.QuadVertexBufferPtr->TexCoord = textureCoords[i];
			Data.QuadVertexBufferPtr->TexIndex = textureIndex;
			Data.QuadVertexBufferPtr++;

		}
		Data.QuadIndexCount += 6;

		Data.Stats.QuadCount++;
	}

	void Renderer2D::DrawSprite(const glm::mat4& transform, SpriteRendererComponent& src)
	{
		if (src.Texture)
			DrawQuad(transform, src.Texture, src.TexCoordMin, src.TexCoordMax);
		else
			DrawQuad(transform, src.Color);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		DrawRotatedQuad({ position.x, position.y, 0 }, size, rotation, color);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		if (Data.QuadIndexCount >= Renderer2DData::MaxIndices)
			FlushAndReset();

		const float whiteTextureIndex = 0.0f;

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		for (int i = 0; i < 4; i++)
		{
			Data.QuadVertexBufferPtr->Position = transform * Data.QuadVertexPositions[i];
			Data.QuadVertexBufferPtr->Color = color;

			glm::vec2 texCoord = { 0.0f, 0.0 };
			if (i == 1)
				texCoord = { 1.0f, 0.0f };
			else if (i == 2)
				texCoord = { 1.0f, 1.0f };
			else if (i == 3)
				texCoord = { 0.0f, 1.0f };

			Data.QuadVertexBufferPtr->TexCoord = texCoord;
			Data.QuadVertexBufferPtr->TexIndex = whiteTextureIndex;
			Data.QuadVertexBufferPtr++;

		}
		Data.QuadIndexCount += 6;

		Data.Stats.QuadCount++;
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Shared<Texture2D> texture)
	{
		DrawRotatedQuad({ position.x, position.y, 0 }, size, rotation, texture);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Shared<Texture2D> texture)
	{
		if (Data.QuadIndexCount >= Renderer2DData::MaxIndices)
			FlushAndReset();

		constexpr glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };

		float textureIndex = 0.0f;

		for (uint32_t i = 1; i < Data.TextureSlotIndex; i++)
		{
			if (*Data.TextureSlots[i] == *texture)
			{
				textureIndex = i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			textureIndex = (float)Data.TextureSlotIndex;
			Data.TextureSlots[Data.TextureSlotIndex] = texture;
			Data.TextureSlotIndex++;
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		for (int i = 0; i < 4; i++)
		{
			Data.QuadVertexBufferPtr->Position = transform * Data.QuadVertexPositions[i];
			Data.QuadVertexBufferPtr->Color = color;

			glm::vec2 texCoord = { 0.0f, 0.0 };
			if (i == 1)
				texCoord = { 1.0f, 0.0f };
			else if (i == 2)
				texCoord = { 1.0f, 1.0f };
			else if (i == 3)
				texCoord = { 0.0f, 1.0f };

			Data.QuadVertexBufferPtr->TexCoord = texCoord;
			Data.QuadVertexBufferPtr->TexIndex = textureIndex;
			Data.QuadVertexBufferPtr++;

		}
		Data.QuadIndexCount += 6;

		Data.Stats.QuadCount++;
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Shared<SubTexture2D> subtexture)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, subtexture);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Shared<SubTexture2D> subtexture)
	{
		if (Data.QuadIndexCount >= Renderer2DData::MaxIndices)
			FlushAndReset();

		constexpr glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };

		constexpr uint32_t quadVertexCount = 4;
		const glm::vec2* texCoords = subtexture->GetTexCoords();
		const Shared<Texture2D> texture = subtexture->GetTexture();

		float textureIndex = 0.0f;

		for (uint32_t i = 1; i < Data.TextureSlotIndex; i++)
		{
			if (*Data.TextureSlots[i] == *texture)
			{
				textureIndex = i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			textureIndex = (float)Data.TextureSlotIndex;
			Data.TextureSlots[Data.TextureSlotIndex] = texture;
			Data.TextureSlotIndex++;
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		for (int i = 0; i < 4; i++)
		{
			Data.QuadVertexBufferPtr->Position = transform * Data.QuadVertexPositions[i];
			Data.QuadVertexBufferPtr->Color = color;
			Data.QuadVertexBufferPtr->TexCoord = texCoords[i];
			Data.QuadVertexBufferPtr->TexIndex = textureIndex;
			Data.QuadVertexBufferPtr++;

		}
		Data.QuadIndexCount += 6;

		Data.Stats.QuadCount++;
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Shared<Texture2D> texture, const glm::vec2& texCoordMin, const glm::vec2& texCoordMax)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, texture, texCoordMin, texCoordMax);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Shared<Texture2D> texture, const glm::vec2& texCoordMin, const glm::vec2& texCoordMax)
	{
		if (Data.QuadIndexCount >= Renderer2DData::MaxIndices)
			FlushAndReset();

		constexpr uint32_t quadVertexCount = 4;

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		glm::vec2 texCoords[] = {
			{ texCoordMin.x, texCoordMin.y },
			{ texCoordMax.x, texCoordMin.y },
			{ texCoordMax.x, texCoordMax.y },
			{ texCoordMin.x, texCoordMax.y },
		};



		constexpr glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };

		float textureIndex = 0.0f;

		for (uint32_t i = 1; i < Data.TextureSlotIndex; i++)
		{
			if (*Data.TextureSlots[i] == *texture)
			{
				textureIndex = i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			textureIndex = (float)Data.TextureSlotIndex;
			Data.TextureSlots[Data.TextureSlotIndex] = texture;
			Data.TextureSlotIndex++;
		}

		for (int i = 0; i < 4; i++)
		{
			Data.QuadVertexBufferPtr->Position = transform * Data.QuadVertexPositions[i];
			Data.QuadVertexBufferPtr->Color = color;
			Data.QuadVertexBufferPtr->TexCoord = texCoords[i];
			Data.QuadVertexBufferPtr->TexIndex = textureIndex;
			Data.QuadVertexBufferPtr++;
		}

		Data.QuadIndexCount += 6;

		Data.Stats.QuadCount++;
	}

	void Renderer2D::ResetStats()
	{
		memset(&Data.Stats, 0, sizeof(Statistics));
	}

	Renderer2D::Statistics& Renderer2D::GetStats()
	{
		return Data.Stats;
	}

}