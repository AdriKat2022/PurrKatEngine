#pragma once
#include "PurrKatEngine.h"

class ExampleSandboxLayer : public PurrKatEngine::ImGuiLayer
{
public:
    glm::vec4 MainColor = {1.f, 1.f, 1.f, 1.f};
    
    ExampleSandboxLayer();
    
    void OnTestActionInvoked();
    void OnImGuiRender() override;
    void OnUpdate() override;
    void OnEvent(PKE::Event& e) override;

private:
    PKE::Ref<PKE::Texture2D> m_RazowskiTexture;
    PKE::Ref<PKE::Texture2D> m_LoveTexture;
    PKE::Ref<PKE::Texture2D> m_CppTexture;
    
    PKE::ShaderLibrary m_ShaderLibrary;
    
    PKE::OrthographicCameraController m_CameraController;

    PKE::Ref<PKE::VertexArray> m_TriangleVertexArray;
    PKE::Ref<PKE::VertexArray> m_SquareVertexArray;

    PKE::Transform m_SquareTransform;
};
