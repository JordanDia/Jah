#pragma once

#include "Camera.h"
#include "Texture.h"
#include "SubTexture2D.h"
#include "Scene/Components.h"
#include "Renderer/EditorCamera.h"

namespace Jah {

	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(Camera& camera, glm::mat4& transform);
		static void BeginScene(EditorCamera& camera);
		static void BeginScene(OrthographicCamera& camera);
		static void EndScene();
		static void Flush();
		static void FlushAndReset();

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Shared<Texture2D> texture);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Shared<Texture2D> texture);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Shared<Texture2D> texture, const glm::vec2& texCoordMin, const glm::vec2& texCoordMax);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Shared<Texture2D> texture, const glm::vec2& texCoordMin, const glm::vec2& texCoordMax);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Shared<SubTexture2D> subtexture);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Shared<SubTexture2D> subtexture);

		static void DrawQuad(const glm::mat4& transform, const glm::vec4& color);
		static void DrawQuad(const glm::mat4& transform, const Shared<Texture2D>& texture, const glm::vec2& texCoordMin, const glm::vec2& texCoordMax);
		static void DrawSprite(const glm::mat4& transform, SpriteRendererComponent& src);

		static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color);
		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color);
		static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Shared<Texture2D> texture);
		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Shared<Texture2D> texture);
		static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Shared<Texture2D> texture, const glm::vec2& texCoordMin, const glm::vec2& texCoordMax);
		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Shared<Texture2D> texture, const glm::vec2& texCoordMin, const glm::vec2& texCoordMax);
		static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Shared<SubTexture2D> subtexture);
		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Shared<SubTexture2D> subtexture);

		struct Statistics
		{
			uint32_t DrawCalls = 0;
			uint32_t QuadCount = 0;

			uint32_t GetTotalVertexCount() { return QuadCount * 4; }
			uint32_t GetTotalIndexCount() { return QuadCount * 6; }
		};
		static Statistics& GetStats();
		static void ResetStats();

	};

}