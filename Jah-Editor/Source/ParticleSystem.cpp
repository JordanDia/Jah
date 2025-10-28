#include "ParticleSystem.h"
#include <glm/glm.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/compatibility.hpp>

namespace Jah {

	ParticleSystem::ParticleSystem(uint32_t maxParticles /*= 10000*/)
		: m_PoolIndex(maxParticles - 1)
	{
		m_ParticlePool.resize(maxParticles);
	}

	void ParticleSystem::OnUpdate(Timestep ts)
	{
		for (auto& particle : m_ParticlePool)
		{
			if (!particle.Active)
				continue;

			if (particle.LifeRemaining <= 0.0f)
			{
				particle.Active = false;
				continue;
			}

			particle.LifeRemaining -= ts;
			particle.Position += particle.Velocity * (float)ts;
			particle.Rotation += particle.RotationSpeed * ts;
		}
	}

	void ParticleSystem::OnRender()
	{
		for (auto& particle : m_ParticlePool)
		{
			if (!particle.Active)
				continue;

			float life = 1.0f - particle.LifeRemaining / particle.Lifetime;
			glm::vec4 color = glm::lerp(particle.ColorBegin, particle.ColorEnd, life);
			float size = glm::lerp(particle.SizeBegin, particle.SizeEnd, life);

			Renderer2D::DrawRotatedQuad(particle.Position, { size, size }, particle.Rotation, color);
		}
	}

	void ParticleSystem::Emit(const ParticleProps& props)
	{
		Particle& particle = m_ParticlePool[m_PoolIndex];
		particle.Active = true;
		particle.Position = props.Position;
		particle.Rotation = ((float)(rand() % 100) / 100.0f) * 2.0f * 3.1415926f;
		particle.RotationSpeed = (rand() / (float)RAND_MAX - 0.5f) * 400.0f;

		particle.Velocity = props.Velocity;
		particle.Velocity.x += props.VelocityVariation.x * (rand() / (float)RAND_MAX - 0.5f);
		particle.Velocity.y += props.VelocityVariation.y * (rand() / (float)RAND_MAX - 0.5f);

		particle.ColorBegin = props.ColorBegin;
		particle.ColorEnd = props.ColorEnd;

		particle.Lifetime = props.Lifetime;
		particle.LifeRemaining = props.Lifetime;
		particle.SizeBegin = props.SizeBegin + props.SizeVariation * (rand() / (float)RAND_MAX - 0.5f);
		particle.SizeEnd = props.SizeEnd;

		m_PoolIndex = --m_PoolIndex % m_ParticlePool.size();
	}

}