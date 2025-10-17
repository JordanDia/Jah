#include "Renderer.h"

#include <glad/glad.h>

namespace Jah {

	Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData;

	void Renderer::Init()
	{
		
	}

	void Renderer::Shutdown()
	{

	}

	void Renderer::BeginScene(OrthographicCamera& camera)
	{
		m_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	}

	void Renderer::EndScene()
	{

	}

	void Renderer::Submit(const std::shared_ptr<Shader>& shader,
		const std::shared_ptr<VertexArray>& vertexArray,
		const glm::mat4& transform,
		const Shared<Texture2D>& texture
	)
	{
		shader->Bind();
		shader->UploadUniformMat4("u_ViewProjection", m_SceneData->ViewProjectionMatrix);
		shader->UploadUniformMat4("u_Transform", transform);

		if (texture)
		{
			texture->Bind(0);
			shader->UploadUniformInt("u_Texture", 0);
		}

		vertexArray->Bind();
		glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
	}

	void Renderer::SetClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void Renderer::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT);
	}

}