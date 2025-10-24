#include "Renderer2D.h"

#include "Renderer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Jah {

	struct QuadVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;
	};

	struct Renderer2DData
	{
		const uint32_t MaxQuads = 10000;
		const uint32_t MaxVertices = MaxQuads * 4;
		const uint32_t MaxIndices = MaxQuads * 6;

		Shared<VertexArray> QuadVertexArray;
		Shared<VertexBuffer> QuadVertexBuffer;
		Shared<Shader> TextureShader;
		Shared<Texture2D> WhiteTexture;

		uint32_t QuadIndexCount = 0;
		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr = nullptr;
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

		Data.TextureShader = std::make_shared<Jah::Shader>("Assets/Shaders/Texture.glsl");
		Data.TextureShader->Bind();
		Data.TextureShader->UploadUniformInt("u_Texture", 0);
	}

	void Renderer2D::Shutdown()
	{
		
	}

	void Renderer2D::BeginScene(OrthographicCamera& camera)
	{
		Data.TextureShader->Bind();
		Data.TextureShader->UploadUniformMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

		Data.QuadIndexCount = 0;
		Data.QuadVertexBufferPtr = Data.QuadVertexBufferBase;
	}

	void Renderer2D::EndScene()
	{
		uint32_t dataSize = uint32_t((uint8_t*)Data.QuadVertexBufferPtr - (uint8_t*)Data.QuadVertexBufferBase);
		Data.QuadVertexBuffer->SetData(Data.QuadVertexBufferBase, dataSize);

		Flush();
	}

	void Renderer2D::Flush()
	{
		Renderer::DrawIndexed(Data.QuadVertexArray, Data.QuadIndexCount);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0 }, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{

		Data.QuadVertexBufferPtr->Position = position;
		Data.QuadVertexBufferPtr->Color = color;
		Data.QuadVertexBufferPtr->TexCoord = { 0.0f, 0.0f };
		Data.QuadVertexBufferPtr++;

		Data.QuadVertexBufferPtr->Position = { position.x + size.x, position.y, 0.0f };
		Data.QuadVertexBufferPtr->Color = color;
		Data.QuadVertexBufferPtr->TexCoord = { 1.0f, 0.0f };
		Data.QuadVertexBufferPtr++;

		Data.QuadVertexBufferPtr->Position = { position.x + size.x, position.y + size.y, 0.0f };
		Data.QuadVertexBufferPtr->Color = color;
		Data.QuadVertexBufferPtr->TexCoord = { 1.0f, 1.0f };
		Data.QuadVertexBufferPtr++;

		Data.QuadVertexBufferPtr->Position = { position.x, position.y + size.y, 0.0f };
		Data.QuadVertexBufferPtr->Color = color;
		Data.QuadVertexBufferPtr->TexCoord = { 0.0f, 1.0f };
		Data.QuadVertexBufferPtr++;

		Data.QuadIndexCount += 6;

		/*Data.WhiteTexture->Bind();
		
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		Data.TextureShader->UploadUniformMat4("u_Transform", transform);

		Data.VertexArray->Bind();
		Renderer::DrawIndexed(Data.VertexArray);*/
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Shared<Texture2D> texture)
	{
		DrawQuad({ position.x, position.y, 0 }, size, texture);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Shared<Texture2D> texture)
	{
		Data.TextureShader->UploadUniformFloat4("u_Color", glm::vec4(1.0f));
		Data.TextureShader->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		Data.TextureShader->UploadUniformMat4("u_Transform", transform);

		texture->Bind();
		Data.QuadVertexArray->Bind();
		Renderer::DrawIndexed(Data.QuadVertexArray);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		DrawRotatedQuad({ position.x, position.y, 0 }, size, rotation, color);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		Data.TextureShader->UploadUniformFloat4("u_Color", color);
		Data.WhiteTexture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		Data.TextureShader->UploadUniformMat4("u_Transform", transform);

		Data.QuadVertexArray->Bind();
		Renderer::DrawIndexed(Data.QuadVertexArray);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Shared<Texture2D> texture)
	{
		DrawRotatedQuad({ position.x, position.y, 0 }, size, rotation, texture);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Shared<Texture2D> texture)
	{
		Data.TextureShader->UploadUniformFloat4("u_Color", glm::vec4(1.0f));
		Data.TextureShader->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		Data.TextureShader->UploadUniformMat4("u_Transform", transform);

		texture->Bind();
		Data.QuadVertexArray->Bind();
		Renderer::DrawIndexed(Data.QuadVertexArray);
	}

}