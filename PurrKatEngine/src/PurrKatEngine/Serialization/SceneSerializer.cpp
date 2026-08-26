#include "pkepch.h"
#include "SceneSerializer.h"

#include <fstream>
#include "PurrKatEngine/Logs/InternalLog.h"
#include "PurrKatEngine/Scene/Components.h"
#include "PurrKatEngine/Scene/Scene.h"
#include "PurrKatEngine/Utility/YamlUtility.h"

namespace PurrKatEngine
{
    SceneSerializer::SceneSerializer(const Ref<Scene>& scene)
        : m_Scene(scene)
    {
        
    }

    void SceneSerializer::Serialize(const std::string& filepath) const
    {
        YAML::Emitter emitter;
        emitter << YAML::BeginMap;
        emitter << YAML::Key << "Scene" << YAML::Value << "Untitled";
        emitter << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
        
        m_Scene->m_Registry.view<entt::entity>().each([&](auto entityId)
        {
            Entity entity = {entityId, m_Scene.get()};
            
            SerializeEntity(emitter, entity);
        });
        
        emitter << YAML::EndSeq;
        emitter << YAML::EndMap;
        
        std::ofstream fout(filepath);
        fout << emitter.c_str();
    }

    void SceneSerializer::SerializeRuntime(const std::string& filepath)
    {
        
    }

    bool SceneSerializer::Deserialize(const std::string& filepath) const
    {
        std::ifstream stream(filepath);
        std::stringstream strStream;
        strStream << stream.rdbuf();
        
        YAML::Node data = YAML::Load(strStream.str());
        if (!data["Scene"])
        {
            PKE_CORE_ERROR("Failed to load scene from file '{}'. The data may be in the wrong format.", filepath);
            return false;
        }
        
        std::string sceneName = data["Scene"].as<std::string>();
        PKE_CORE_INFO("Deserializing scene '{}'", sceneName);
        
        // Deserialize entities
        if (YAML::Node entities = data["Entities"])
        {
            for (auto entityNode : entities)
            {
                DeserializeEntity(entityNode);
            }
        }
        
        return false;
    }

    bool SceneSerializer::DeserializeRuntime(const std::string& filepath)
    {
        return false;
    }
    
    void SceneSerializer::DeserializeEntity(YAML::detail::iterator_base<YAML::detail::iterator_value>::value_type entityNode) const
    {
        uint64_t entityId = entityNode["Entity"].as<uint64_t>(); // We should have a uuid here, but for now we will just use the entityId as a placeholder.
        std::string tag = entityNode["Tag"].as<std::string>();
        
        PKE_CORE_DEBUG("Deserializing entity with ID {} ({})", entityId, tag);
        
        // TODO: RECREATE the entity with the same ID as before.
        Entity entity = m_Scene->CreateEntity(tag);
                
        if (YAML::Node transformNode = entityNode["Transform"])
        {
            auto& transform = entity.GetComponent<TransformComponent>();
            transform.Position = transformNode["Position"].as<glm::vec3>();
            transform.Rotation = transformNode["Rotation"].as<glm::vec3>();
            transform.Scale = transformNode["Scale"].as<glm::vec3>();
        }
                
        if (YAML::Node spriteNode = entityNode["Sprite"])
        {
            auto& sprite = entity.AddComponent<SpriteComponent>();
            sprite.Color = spriteNode["Color"].as<glm::vec4>();
        }
                
        if (YAML::Node cameraNode = entityNode["Camera"])
        {
            auto& cameraComp = entity.AddComponent<CameraComponent>();
            // Deserialize camera properties here
            cameraComp.Camera.SetProjectionType((SceneCamera::ProjectionType)cameraNode["ProjectionType"].as<int>());
            cameraComp.Camera.SetPerspectivePov(cameraNode["PerspectiveFOV"].as<float>());
            cameraComp.Camera.SetPerspectiveNearClip(cameraNode["PerspectiveNear"].as<float>());
            cameraComp.Camera.SetPerspectiveFarClip(cameraNode["PerspectiveFar"].as<float>());
            cameraComp.Camera.SetOrthographicSize(cameraNode["OrthographicSize"].as<float>());
            cameraComp.Camera.SetOrthographicNearClip(cameraNode["OrthographicNear"].as<float>());
            cameraComp.Camera.SetOrthographicFarClip(cameraNode["OrthographicFar"].as<float>());
        }
    }

    void SceneSerializer::SerializeEntity(YAML::Emitter& emitter, Entity entity)
    {
        emitter << YAML::BeginMap;
        emitter << YAML::Key << "Entity" << YAML::Value << (uint32_t)entity;
            
        if (entity.HasComponent<TagComponent>())
        {
            auto& tag = entity.GetComponent<TagComponent>().Tag;
            emitter << YAML::Key << "Tag" << YAML::Value << tag;
        }
            
        if (entity.HasComponent<TransformComponent>())
        {
            auto& transform = entity.GetComponent<TransformComponent>();
            emitter << YAML::Key << "Transform" << YAML::Value << YAML::BeginMap;
            emitter << YAML::Key << "Position" << YAML::Value << transform.Position;
            emitter << YAML::Key << "Rotation" << YAML::Value << transform.Rotation;
            emitter << YAML::Key << "Scale" << YAML::Value << transform.Scale;
            emitter << YAML::EndMap;
        }
            
        if (entity.HasComponent<SpriteComponent>())
        {
            auto& sprite = entity.GetComponent<SpriteComponent>();
            emitter << YAML::Key << "Sprite" << YAML::Value << YAML::BeginMap;
            emitter << YAML::Key << "Color" << YAML::Value << sprite.Color;
            emitter << YAML::EndMap;
        }
            
        if (entity.HasComponent<CameraComponent>())
        {
            auto& camera = entity.GetComponent<CameraComponent>();
            emitter << YAML::Key << "Camera" << YAML::Value << YAML::BeginMap;
            // Serialize camera properties here
            emitter << YAML::Key << "ProjectionType" << YAML::Value << (int)camera.Camera.GetProjectionType();
            emitter << YAML::Key << "PerspectiveFOV" << YAML::Value << camera.Camera.GetPerspectivePov();
            emitter << YAML::Key << "PerspectiveNear" << YAML::Value << camera.Camera.GetPerspectiveNearClip();
            emitter << YAML::Key << "PerspectiveFar" << YAML::Value << camera.Camera.GetPerspectiveFarClip();
            emitter << YAML::Key << "OrthographicSize" << YAML::Value << camera.Camera.GetOrthographicSize();
            emitter << YAML::Key << "OrthographicNear" << YAML::Value << camera.Camera.GetOrthographicNearClip();
            emitter << YAML::Key << "OrthographicFar" << YAML::Value << camera.Camera.GetOrthographicFarClip();
            emitter << YAML::EndMap;
        }
            
        emitter << YAML::EndMap; // End of Entity
    }
}
