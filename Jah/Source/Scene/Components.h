#pragma once

#include <glm/glm.hpp>

namespace Jah {


	struct TransformComponent
	{
		glm::vec3 Position	{ 0.0f, 0.0f, 0.0f };
		glm::vec2 Size		{ 1.0f, 1.0f };
		float Rotation =	0.0f;

		TransformComponent() = default;
		TransformComponent(const glm::vec3& position)
			: Position(position) {}
		TransformComponent(const glm::vec3& position, const glm::vec2& scale, float rotation)
			: Position(position), Rotation(rotation), Size(scale) {}
	};

	struct SpriteRendererComponent
	{

		glm::vec4 Color		{ 0.3f, 0.4f, 1.0f, 1.0f };

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const glm::vec4 color)
			: Color(color) {}
	};
}