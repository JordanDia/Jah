#pragma once

#include <memory>
#include <glm/glm.hpp>
#include "Shader.h"
#include "VertexArray.h"
#include "Camera.h"

namespace Jah {

	class Renderer
	{
	public:

		static void Init();
		static void Shutdown();

		static void BeginScene(OrthographicCamera& camera);
		static void EndScene();

		static void Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f));
		static void SetClearColor(const glm::vec4& color);
		static void Clear();

	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};

		static SceneData* m_SceneData;
	};

}