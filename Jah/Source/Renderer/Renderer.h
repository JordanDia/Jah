#pragma once

#include <memory>
#include <glm/glm.hpp>
#include "Shader.h"
#include "VertexArray.h"
#include "Camera.h"
#include "Texture.h"

namespace Jah {

	class Renderer
	{
	public:

		static void Init();
		static void Shutdown();

		static void OnWindowResize(uint32_t width, uint32_t height);

		static void BeginScene(OrthographicCamera& camera);
		static void BeginScene(const glm::mat4& viewProjectionMatrix);
		static void EndScene();

		static void Submit(const Shared<Shader>& shader,
			const Shared<VertexArray>& vertexArray,
			const glm::mat4& transform = glm::mat4(1.0f),
			const Shared<Texture2D>& texture = nullptr
		);
		static void SetClearColor(const glm::vec4& color);
		static void Clear();

		static void DrawIndexed(const Shared<VertexArray>& vertexArray, uint32_t indexCount = 0);
		static void DrawLines(const Shared<VertexArray>& vertexArray, uint32_t vertexCount = 0);

		static void SetLineWidth(float width);

	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};

		static SceneData* m_SceneData;
	};

}