#pragma once
#include "PurrKatEngine/Scene/Entity.h"
#include "yaml-cpp/emitter.h"
#include "yaml-cpp/node/iterator.h"

namespace PurrKatEngine
{
    class Scene;

    // SceneSerializer is responsible for serializing and deserializing a Scene object to and from a file.
    // If deserializing, it will also be responsible for allocating the Scene object and its entities.
    class SceneSerializer
    {
    public:
        SceneSerializer(const Ref<Scene>& scene);

        void Serialize(const std::string& filepath) const;
        void SerializeRuntime(const std::string& filepath);

        bool Deserialize(const std::string& filepath) const;
        bool DeserializeRuntime(const std::string& filepath);
        
    private:
        void DeserializeEntity(YAML::detail::iterator_base<YAML::detail::iterator_value>::value_type entityNode) const;
        static void SerializeEntity(YAML::Emitter& emitter, Entity entity);
        
    private:
        Ref<Scene> m_Scene = nullptr;
    };
}
