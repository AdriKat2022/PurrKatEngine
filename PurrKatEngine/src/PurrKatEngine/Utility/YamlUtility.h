#pragma once
#include "yaml-cpp/yaml.h"

namespace PurrKatEngine
{
    // ---------------------- Serializers for glm types to YAML ----------------------
    YAML::Emitter& operator<<(YAML::Emitter& emitter, const glm::vec3& vec);
    YAML::Emitter& operator<<(YAML::Emitter& emitter, const glm::vec4& vec);
}

namespace YAML
{
    // ---------------------- Serializers for YAML types to glm ----------------------
    template <>
    struct convert<glm::vec3>
    {
        static Node encode(const glm::vec3& vec)
        {
            Node node;
            node.push_back(vec.x);
            node.push_back(vec.y);
            node.push_back(vec.z);
            return node;
        }
        
        static bool decode(const Node& node, glm::vec3& vec)
        {
            if (!node.IsSequence() || node.size() != 3)
                return false;

            vec.x = node[0].as<float>();
            vec.y = node[1].as<float>();
            vec.z = node[2].as<float>();
            return true;
        }
    };
    
    template <>
    struct convert<glm::vec4>
    {
        static Node encode(const glm::vec4& vec)
        {
            Node node;
            node.push_back(vec.x);
            node.push_back(vec.y);
            node.push_back(vec.z);
            return node;
        }
        
        static bool decode(const Node& node, glm::vec4& vec)
        {
            if (!node.IsSequence() || node.size() != 4)
                return false;

            vec.x = node[0].as<float>();
            vec.y = node[1].as<float>();
            vec.z = node[2].as<float>();
            vec.w = node[3].as<float>();
            return true;
        }
    };
}
