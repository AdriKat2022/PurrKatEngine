#include "pkepch.h"
#include "YamlUtility.h"

#include <iomanip>


namespace PurrKatEngine
{
    YAML::Emitter& operator<<(YAML::Emitter& emitter, const glm::vec3& vec)
    {
        emitter << YAML::Flow;
        emitter << YAML::BeginSeq;
        emitter << vec.x << vec.y << vec.z;
        emitter << YAML::EndSeq;
        
        return emitter;
    }
    
    YAML::Emitter& operator<<(YAML::Emitter& emitter, const glm::vec4& vec)
    {
        emitter << YAML::Flow;
        emitter << YAML::BeginSeq;
        emitter << vec.x << vec.y << vec.z << vec.w;
        emitter << YAML::EndSeq;
        
        return emitter;
    }
}
