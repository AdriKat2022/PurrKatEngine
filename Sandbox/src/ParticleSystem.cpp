#include "ParticleSystem.h"

#include <glm/ext/scalar_constants.hpp>
#include "PurrKatEngine.h"
#include "PurrKatEngine/Inputs/Time.h"
#include "PurrKatEngine/Renderer/OrthographicCamera.h"
#include "PurrKatEngine/Utility/Random.h"

void ParticleSystem::Emit(const ParticleProperties& props)
{
    if (m_ParticlePool.empty()) return;
    
    m_PoolIndex = (m_PoolIndex - 1) % m_ParticlePool.size();
    
    Particle& particle = m_ParticlePool[m_PoolIndex];
    
    if (!particle.Active) m_ActiveParticles++;
    
    particle.Active = true;
    particle.Position = props.Position;
    particle.Velocity = props.Velocity;
    particle.Rotation = 2 * glm::pi<float>() * PKE_RAND01();
    
    particle.Velocity = props.Velocity;
    particle.Velocity.x = props.VelocityVariation.x * PKE_RANDOM(-0.5f, 0.5f);
    particle.Velocity.y = props.VelocityVariation.y * PKE_RANDOM(-0.5f, 0.5f);
    
    particle.ColorBegin = props.ColorBegin;
    particle.ColorEnd = props.ColorEnd;
    
    particle.LifeTime = props.LifeTime;
    particle.LifeRemaining = props.LifeTime;
    particle.SizeBegin = props.SizeBegin;
    particle.SizeEnd = props.SizeEnd;
}

void ParticleSystem::OnRender(const PurrKatEngine::OrthographicCamera& camera) const
{
    if (m_ParticlePool.empty() || m_ActiveParticles == 0) return;
    
    PKE::Renderer2D::BeginScene(camera);
    
    for (const auto& particle : m_ParticlePool)
    {
        if (!particle.Active) continue;

        float life = particle.LifeRemaining / particle.LifeTime;

        glm::vec4 color = glm::mix(particle.ColorEnd, particle.ColorBegin, life);
        float size = std::lerp(particle.SizeEnd, particle.SizeBegin, life);

        PKE::Renderer2D::DrawRotatedQuad(particle.Position, {size, size}, particle.Rotation, nullptr, {1, 1}, color);
    }

    PKE::Renderer2D::EndScene();
}

void ParticleSystem::OnAttach()
{
    Layer::OnAttach();
}

void ParticleSystem::OnDetach()
{
    Layer::OnDetach();
}

void ParticleSystem::OnUpdate()
{
    Layer::OnUpdate();
    
    WATCH_VALUE(m_ActiveParticles);
    
    if (m_ParticlePool.empty() || m_ActiveParticles == 0) return;
    
    for (auto& particle : m_ParticlePool)
    {
        if (!particle.Active) continue;
        
        particle.LifeRemaining -= (float)PurrKatEngine::Time::deltaTime;
        
        if (particle.LifeRemaining <= 0.0f)
        {
            particle.Active = false;
            m_ActiveParticles--;
            continue;
        }
        
        particle.Position += particle.Velocity * (float)PurrKatEngine::Time::deltaTime;
        particle.Rotation += 0.01f * (float)PurrKatEngine::Time::deltaTime;
    }
}

void ParticleSystem::OnImGuiRender()
{
    Layer::OnImGuiRender();
}

void ParticleSystem::OnEvent(PurrKatEngine::Event& event)
{
    Layer::OnEvent(event);
}
