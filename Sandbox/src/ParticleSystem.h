#pragma once
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include "PurrKatEngine/Layers/Layer.h"
#include "PurrKatEngine/Renderer/OrthographicCamera.h"

struct ParticleProperties
{
    glm::vec2 Position;
    glm::vec2 Velocity, VelocityVariation;
    glm::vec4 ColorBegin, ColorEnd;
    float SizeBegin, SizeEnd, SizeVariation;
    float LifeTime;
};

class ParticleSystem : public PurrKatEngine::Layer
{
public:
    explicit ParticleSystem(size_t maxParticleCount = 500) { SetMaxParticleCount(maxParticleCount); }
    
    void SetMaxParticleCount(size_t maxParticleCount) { m_ParticlePool.resize(maxParticleCount); }
    size_t GetMaxParticleCount() const { return m_ParticlePool.size(); }
    
    void Emit(const ParticleProperties& props);
    void OnRender(const PurrKatEngine::OrthographicCamera& camera) const;
    
    void OnAttach() override;
    void OnDetach() override;
    void OnUpdate() override;
    void OnImGuiRender() override;
    void OnEvent(PurrKatEngine::Event& event) override;
    
private:
    struct Particle
    {
        glm::vec2 Position;
        glm::vec2 Velocity;
        glm::vec4 ColorBegin, ColorEnd;
        
        float Rotation = 0;
        float SizeBegin, SizeEnd;
        
        float LifeTime = 1;
        float LifeRemaining = 0;
        
        bool Active = false;
    };
    
    
    std::vector<Particle> m_ParticlePool;
    uint32_t m_PoolIndex = 0;
    uint32_t m_ActiveParticles = 0;
};
