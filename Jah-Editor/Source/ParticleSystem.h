#pragma once

#include <Jah.h>


namespace Jah {

	struct ParticleProps
	{
		glm::vec2 Position;

		glm::vec2 Velocity;
		glm::vec2 VelocityVariation;

		glm::vec4 ColorBegin;
		glm::vec4 ColorEnd;

		float Rotation = 0.0f;
	

		float SizeBegin = 0.0f;
		float SizeEnd = 0.0f;
		float SizeVariation = 0.0f;

		float Lifetime = 1.0f;
	};

	struct Particle
	{
		glm::vec2 Position;
		glm::vec2 Velocity;
		glm::vec4 ColorBegin;
		glm::vec4 ColorEnd;

		float Rotation = 0.0f;
		float RotationSpeed = 0.0f;

		float SizeBegin = 0.0f;
		float SizeEnd = 0.0f;

		float Lifetime = 1.0f;
		float LifeRemaining = 1.0f;

		bool Active = false;
	};

	class ParticleSystem
	{
	public:
		ParticleSystem(uint32_t maxParticles = 10000);

		void OnUpdate(Timestep ts);
		void OnRender();
		void Emit(const ParticleProps& particleProps);

	private:
		std::vector<Particle> m_ParticlePool;
		uint32_t m_PoolIndex = 0;
	};

}