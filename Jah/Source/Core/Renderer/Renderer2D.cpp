#include "Renderer2D.h"

#include "Renderer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Jah {

	struct Renderer2DStorage
	{
		Shared<VertexArray> VertexArray;
		Shared<Shader> TextureShader;
		Shared<Texture2D> WhiteTexture;
	};

	static Renderer2DStorage* s_Data = nullptr;

	void Renderer2D::Init()
	{
		s_Data = new Renderer2DStorage();

		s_Data->VertexArray = std::make_shared<Jah::VertexArray>();

		float squareVertices[4 * 5] = {
				-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
				 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
				 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
				-0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
		};
		std::shared_ptr<Jah::VertexBuffer> squareVB = std::make_shared<Jah::VertexBuffer>(squareVertices, sizeof(squareVertices));
		Jah::BufferLayout squareLayout = {
			{ Jah::ShaderDataType::Float3, "a_Position" },
			{ Jah::ShaderDataType::Float2, "a_TexCoord" },
		};
		squareVB->SetLayout(squareLayout);
		s_Data->VertexArray->AddVertexBuffer(squareVB);

		unsigned int squareIndices[6] = {
			0, 1, 2,
			2, 3, 0
		};
		std::shared_ptr<Jah::IndexBuffer> squareIB = std::make_shared<Jah::IndexBuffer>(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
		s_Data->VertexArray->SetIndexBuffer(squareIB);

		s_Data->WhiteTexture = std::make_shared<Texture2D>(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_Data->WhiteTexture->SetData(&whiteTextureData, sizeof(whiteTextureData));

		s_Data->TextureShader = std::make_shared<Jah::Shader>("Assets/Shaders/Texture.glsl");
		s_Data->TextureShader->Bind();
		s_Data->TextureShader->UploadUniformInt("u_Texture", 0);
	}

	void Renderer2D::Shutdown()
	{
		delete s_Data;
	}

	void Renderer2D::BeginScene(OrthographicCamera& camera)
	{
		s_Data->TextureShader->Bind();
		s_Data->TextureShader->UploadUniformMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
	}

	void Renderer2D::EndScene()
	{

	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2 size, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0 }, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2 size, const glm::vec4& color)
	{
		s_Data->TextureShader->UploadUniformFloat4("u_Color", color);
		s_Data->WhiteTexture->Bind();
		
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_Data->TextureShader->UploadUniformMat4("u_Transform", transform);

		s_Data->VertexArray->Bind();
		Renderer::DrawIndexed(s_Data->VertexArray);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2 size, const Shared<Texture2D> texture)
	{
		DrawQuad({ position.x, position.y, 0 }, size, texture);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2 size, const Shared<Texture2D> texture)
	{
		s_Data->TextureShader->UploadUniformFloat4("u_Color", glm::vec4(1.0f));
		s_Data->TextureShader->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_Data->TextureShader->UploadUniformMat4("u_Transform", transform);

		texture->Bind();
		s_Data->VertexArray->Bind();
		Renderer::DrawIndexed(s_Data->VertexArray);
	}

}