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
		glm::vec3 Position = {};
		glm::vec4 Color = {};
		glm::vec2 TexCoord = {};
		float TexIndex = 0;

		int EntityID = 0;
	};

	struct CircleVertex
	{
		glm::vec3 WorldPosition = {};
		glm::vec3 LocalPosition = {};
		glm::vec4 Color = {};
		float Thickness = 0.0f;
		float Fade = 0.0f;

		int EntityID = 0;
	};

	struct LineVertex
	{
		glm::vec3 Position = {};
		glm::vec4 Color = {};

		int EntityID = 0;
	};

	struct Renderer2DData
	{
		static constexpr uint32_t MaxQuads = 20000;
		static constexpr uint32_t MaxVertices = MaxQuads * 4;
		static constexpr uint32_t MaxIndices = MaxQuads * 6;
		static constexpr uint32_t MaxTextureSlots = 32;

		Shared<VertexArray> QuadVertexArray;
		Shared<VertexBuffer> QuadVertexBuffer;
		Shared<Shader> QuadShader;
		Shared<Texture2D> WhiteTexture;

		Shared<VertexArray> CircleVertexArray;
		Shared<VertexBuffer> CircleVertexBuffer;
		Shared<Shader> CircleShader;

		Shared<VertexArray> LineVertexArray;
		Shared<VertexBuffer> LineVertexBuffer;
		Shared<Shader> LineShader;
		float LineWidth = 2.0f;

		uint32_t QuadIndexCount = 0;
		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr = nullptr;

		uint32_t CircleIndexCount = 0;
		CircleVertex* CircleVertexBufferBase = nullptr;
		CircleVertex* CircleVertexBufferPtr = nullptr;

		uint32_t LineVertexCount = 0;
		LineVertex* LineVertexBufferBase = nullptr;
		LineVertex* LineVertexBufferPtr = nullptr;

		std::array<Shared<Texture2D>, MaxTextureSlots> TextureSlots;
		uint32_t TextureSlotIndex = 1;

		glm::vec4 QuadVertexPositions[4]{};

		Renderer2D::Statistics Stats;
	};

	static Renderer2DData Data;

	void Renderer2D::Init()
	{
		Data.QuadVertexArray = CreateShared<VertexArray>();

		Data.QuadVertexBuffer = CreateShared<VertexBuffer>(Data.MaxVertices * sizeof(QuadVertex));
		Data.QuadVertexBuffer->SetLayout({
			{ ShaderDataType::Float3,	"a_Position"	},
			{ ShaderDataType::Float4,	"a_Color"		},
			{ ShaderDataType::Float2,	"a_TexCoord"	},
			{ ShaderDataType::Float,	"a_TexIndex"	},
			{ ShaderDataType::Int,		"a_EntityID"	},
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

		std::shared_ptr<IndexBuffer> quadIB = CreateShared<IndexBuffer>(quadIndices, Data.MaxIndices);
		Data.QuadVertexArray->SetIndexBuffer(quadIB);
		delete[] quadIndices;


		// Circles
		Data.CircleVertexArray = CreateShared<VertexArray>();

		Data.CircleVertexBuffer = CreateShared<VertexBuffer>(Data.MaxVertices * sizeof(CircleVertex));
		Data.CircleVertexBuffer->SetLayout({
			{ ShaderDataType::Float3,	"a_WorldPosition"	},
			{ ShaderDataType::Float3,	"a_LocalPosition"	},
			{ ShaderDataType::Float4,	"a_Color"		},
			{ ShaderDataType::Float,	"a_Thickness"	},
			{ ShaderDataType::Float,	"a_Fade"		},
			{ ShaderDataType::Int,		"a_EntityID"	},
		});
		Data.CircleVertexArray->AddVertexBuffer(Data.CircleVertexBuffer);
		Data.CircleVertexArray->SetIndexBuffer(quadIB); // Use quad IB
		Data.CircleVertexBufferBase = new CircleVertex[Data.MaxVertices];


		// Lines
		Data.LineVertexArray = CreateShared<VertexArray>();

		Data.LineVertexBuffer = CreateShared<VertexBuffer>(Data.MaxVertices * sizeof(LineVertex));
		Data.LineVertexBuffer->SetLayout({
			{ ShaderDataType::Float3,	"a_Position"	},
			{ ShaderDataType::Float4,	"a_Color"		},
			{ ShaderDataType::Int,		"a_EntityID"	},
		});
		Data.LineVertexArray->AddVertexBuffer(Data.LineVertexBuffer);
		Data.LineVertexBufferBase = new LineVertex[Data.MaxVertices];

		// Textures
		Data.WhiteTexture = CreateShared<Texture2D>(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		Data.WhiteTexture->SetData(&whiteTextureData, sizeof(whiteTextureData));

		int32_t samplers[Data.MaxTextureSlots];
		for (uint32_t i = 0; i < Data.MaxTextureSlots; i++)
			samplers[i] = i;

		Data.QuadShader = CreateShared<Shader>("Assets/Shaders/Renderer2D_Quad.glsl");
		Data.CircleShader = CreateShared<Shader>("Assets/Shaders/Renderer2D_Circle.glsl");
		Data.LineShader = CreateShared<Shader>("Assets/Shaders/Renderer2D_Line.glsl");

		Data.QuadShader->Bind();
		Data.QuadShader->UploadUniformIntArray("u_Textures", samplers, Data.MaxTextureSlots);
		Data.TextureSlots[0] = Data.WhiteTexture;

		Data.QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		Data.QuadVertexPositions[1] = {  0.5f, -0.5f, 0.0f, 1.0f };
		Data.QuadVertexPositions[2] = {  0.5f,  0.5f, 0.0f, 1.0f };
		Data.QuadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };
	}

	void Renderer2D::Shutdown()
	{
		delete[] Data.QuadVertexBufferBase;
		delete[] Data.CircleVertexBufferBase;
	}

	void Renderer2D::BeginScene(Camera& camera, glm::mat4& transform)
	{
		glm::mat4 viewProj = camera.GetProjection() * glm::inverse(transform);

		Data.QuadShader->Bind();
		Data.QuadShader->UploadUniformMat4("u_ViewProjection", viewProj);

		Data.CircleShader->Bind();
		Data.CircleShader->UploadUniformMat4("u_ViewProjection", viewProj);

		Data.LineShader->Bind();
		Data.LineShader->UploadUniformMat4("u_ViewProjection", viewProj);

		Data.QuadIndexCount = 0;
		Data.QuadVertexBufferPtr = Data.QuadVertexBufferBase;

		Data.CircleIndexCount = 0;
		Data.CircleVertexBufferPtr = Data.CircleVertexBufferBase;

		Data.LineVertexCount = 0;
		Data.LineVertexBufferPtr = Data.LineVertexBufferBase;

		Data.TextureSlotIndex = 1;
	}

	void Renderer2D::BeginScene(OrthographicCamera& camera)
	{
		Data.QuadShader->Bind();
		Data.QuadShader->UploadUniformMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

		Data.CircleShader->Bind();
		Data.CircleShader->UploadUniformMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

		Data.LineShader->Bind();
		Data.LineShader->UploadUniformMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

		Data.QuadIndexCount = 0;
		Data.QuadVertexBufferPtr = Data.QuadVertexBufferBase;

		Data.TextureSlotIndex = 1;
	}

	

	void Renderer2D::BeginScene(EditorCamera& camera)
	{

		glm::mat4 viewProj = camera.GetViewProjection();

		Data.QuadShader->Bind();
		Data.QuadShader->UploadUniformMat4("u_ViewProjection", viewProj);

		Data.CircleShader->Bind();
		Data.CircleShader->UploadUniformMat4("u_ViewProjection", viewProj);

		Data.LineShader->Bind();
		Data.LineShader->UploadUniformMat4("u_ViewProjection", viewProj);

		StartBatch();
	}

	void Renderer2D::EndScene()
	{
		Flush();
	}

	void Renderer2D::StartBatch()
	{
		Data.QuadIndexCount = 0;
		Data.QuadVertexBufferPtr = Data.QuadVertexBufferBase;

		Data.CircleIndexCount = 0;
		Data.CircleVertexBufferPtr = Data.CircleVertexBufferBase;

		Data.LineVertexCount = 0;
		Data.LineVertexBufferPtr = Data.LineVertexBufferBase;

		Data.TextureSlotIndex = 1;
	}

	void Renderer2D::NextBatch()
	{
		Flush();
		StartBatch();
	}

	void Renderer2D::Flush()
	{
		if (Data.QuadIndexCount)
		{
			uint32_t dataSize = uint32_t((uint8_t*)Data.QuadVertexBufferPtr - (uint8_t*)Data.QuadVertexBufferBase);
			Data.QuadVertexBuffer->SetData(Data.QuadVertexBufferBase, dataSize);

			for (uint32_t i = 0; i < Data.TextureSlotIndex; i++)
				Data.TextureSlots[i]->Bind(i);
			
			Data.QuadShader->Bind();
			Renderer::DrawIndexed(Data.QuadVertexArray, Data.QuadIndexCount);
			Data.Stats.DrawCalls++;
		}

		if (Data.CircleIndexCount)
		{
			uint32_t dataSize = uint32_t((uint8_t*)Data.CircleVertexBufferPtr - (uint8_t*)Data.CircleVertexBufferBase);
			Data.CircleVertexBuffer->SetData(Data.CircleVertexBufferBase, dataSize);

			Data.CircleShader->Bind();
			Renderer::DrawIndexed(Data.CircleVertexArray, Data.CircleIndexCount);
			Data.Stats.DrawCalls++;
		}

		if (Data.LineVertexCount)
		{
			uint32_t dataSize = uint32_t((uint8_t*)Data.LineVertexBufferPtr - (uint8_t*)Data.LineVertexBufferBase);
			Data.LineVertexBuffer->SetData(Data.LineVertexBufferBase, dataSize);

			Data.LineShader->Bind();
			Renderer::SetLineWidth(Data.LineWidth);
			Renderer::DrawLines(Data.LineVertexArray, Data.LineVertexCount);
			Data.Stats.DrawCalls++;
		}
	}
	

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0 }, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		if (Data.QuadIndexCount >= Renderer2DData::MaxIndices)
			NextBatch();

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
			NextBatch();

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
			NextBatch();

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
			NextBatch();

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

	void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color, int entityID)
	{
		if (Data.QuadIndexCount >= Renderer2DData::MaxIndices)
			NextBatch();

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
			Data.QuadVertexBufferPtr->EntityID = entityID;
			Data.QuadVertexBufferPtr++;

		}
		Data.QuadIndexCount += 6;

		Data.Stats.QuadCount++;
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const Shared<Texture2D>& texture, const glm::vec2& texCoordMin, const glm::vec2& texCoordMax, int entityID)
	{
		if (Data.QuadIndexCount >= Renderer2DData::MaxIndices)
			NextBatch();

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
				NextBatch();

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
			Data.QuadVertexBufferPtr->EntityID = entityID;
			Data.QuadVertexBufferPtr++;

		}
		Data.QuadIndexCount += 6;

		Data.Stats.QuadCount++;
	}

	void Renderer2D::DrawCircle(const glm::mat4& transform, const glm::vec4& color, float thickness /*= 1.0f*/, float fade /*= 0.005f*/, int entityID /*= -1*/)
	{
		if (Data.CircleIndexCount >= Renderer2DData::MaxIndices)
			NextBatch();

		for (int i = 0; i < 4; i++)
		{
			Data.CircleVertexBufferPtr->WorldPosition = transform * Data.QuadVertexPositions[i];
			Data.CircleVertexBufferPtr->LocalPosition = Data.QuadVertexPositions[i] * 2.0f;
			Data.CircleVertexBufferPtr->Color = color;
			Data.CircleVertexBufferPtr->Thickness = thickness;
			Data.CircleVertexBufferPtr->Fade = fade;
			Data.CircleVertexBufferPtr->EntityID = entityID;
			Data.CircleVertexBufferPtr++;

		}
		Data.CircleIndexCount += 6;

		Data.Stats.QuadCount++;
	}

	void Renderer2D::DrawLine(const glm::vec3& p0, const glm::vec3& p1, const glm::vec4& color, int entityID)
	{
		Data.LineVertexBufferPtr->Position = p0;
		Data.LineVertexBufferPtr->Color = color;
		Data.LineVertexBufferPtr->EntityID = entityID;
		Data.LineVertexBufferPtr++;

		Data.LineVertexBufferPtr->Position = p1;
		Data.LineVertexBufferPtr->Color = color;
		Data.LineVertexBufferPtr->EntityID = entityID;
		Data.LineVertexBufferPtr++;

		Data.LineVertexCount += 2;
	}

	void Renderer2D::DrawRect(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, int entityID /*= -1*/)
	{
		glm::vec3 p0 = glm::vec3(position.x - size.x / 2, position.y - size.y / 2, 0);
		glm::vec3 p1 = glm::vec3(position.x + size.x / 2, position.y - size.y / 2, 0);
		glm::vec3 p2 = glm::vec3(position.x + size.x / 2, position.y + size.y / 2, 0);
		glm::vec3 p3 = glm::vec3(position.x - size.x / 2, position.y + size.y / 2, 0);

		DrawLine(p0, p1, color, entityID);
		DrawLine(p1, p2, color, entityID);
		DrawLine(p2, p3, color, entityID);
		DrawLine(p3, p0, color, entityID);
	}

	void Renderer2D::DrawRect(const glm::mat4& transform, const glm::vec4& color, int entityID /*= -1*/)
	{
		glm::vec3 lineVertices[4];
		for (size_t i = 0; i < 4; i++)
			lineVertices[i] = transform * Data.QuadVertexPositions[i];

		DrawLine(lineVertices[0], lineVertices[1], color, entityID);
		DrawLine(lineVertices[1], lineVertices[2], color, entityID);
		DrawLine(lineVertices[2], lineVertices[3], color, entityID);
		DrawLine(lineVertices[3], lineVertices[0], color, entityID);
	}

	void Renderer2D::DrawSprite(const glm::mat4& transform, SpriteRendererComponent& src, int entityID)
	{
		if (src.Texture)
			DrawQuad(transform, src.Texture, src.TexCoordMin, src.TexCoordMax, entityID);
		else
			DrawQuad(transform, src.Color, entityID);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		DrawRotatedQuad({ position.x, position.y, 0 }, size, rotation, color);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		if (Data.QuadIndexCount >= Renderer2DData::MaxIndices)
			NextBatch();

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
			NextBatch();

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
			NextBatch();

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
			NextBatch();

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

	float Renderer2D::GetLineWidth()
	{
		return Data.LineWidth;
	}

	void Renderer2D::SetLineWidth(float width)
	{
		Renderer::SetLineWidth(width);
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