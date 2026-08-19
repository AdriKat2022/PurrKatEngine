#include "Sandbox2DMapTiling.h"

using namespace PurrKatEngine;

static const char* s_MapTiles =
    "GGGGGGGGGGGGGGGGGGGGGGGG"
    "GGGGGGGGGGGGGGGGGGGGGGGG"
    "GGGGGGGGGDDDDGGGGGGGGGGG"
    "GGGGGGGDDDDDDDDDGGGGGGGG"
    "GGGGDDDDDGGGGDDDDDDDGGGG"
    "GGGGGGDDDDDDDDDDDDDDGGGG"
    "GGGGDDDDDDDDDDDDDDDGGGGG"
    "GGGGDDDDDDDDDDDDDGGGGGGG"
    "GGGGGGGGGGGGGGGGGGGGGGGG"
    "GGGGGGGGGGGGGGGGGGGGGGGG";

Sandbox2DMapTiling::Sandbox2DMapTiling()
{
    m_CameraController.EnableZoom = true;
    
    m_GrassSpriteSheet.SetTexture(Texture2D::CreateRef("assets/textures/TileSets/Grass.png", { .Filter = Texture2D::FilterType::Nearest }));
    m_GrassSpriteSheet.SetSpriteSheetOptions({ .CellCount = {11, 7}});
    
    m_DirtSpriteSheet.SetTexture(Texture2D::CreateRef("assets/textures/TileSets/Tilled_Dirt_Wide.png", { .Filter = Texture2D::FilterType::Nearest }));
    m_DirtSpriteSheet.SetSpriteSheetOptions({ .CellSize = {11, 7}});
    
    m_TileMap['G'] = m_GrassSpriteSheet.GetSprite({1, 1});
    m_TileMap['D'] = m_DirtSpriteSheet.GetSprite({1, 1});
    
}

void Sandbox2DMapTiling::OnAttach()
{
    Layer::OnAttach();
}

void Sandbox2DMapTiling::OnDetach()
{
    Layer::OnDetach();
}

void Sandbox2DMapTiling::OnUpdate()
{
    Layer::OnUpdate();
    m_CameraController.OnUpdate();
    
    RenderCommand::Clear();
    
    Renderer2D::BeginScene(m_CameraController.GetCamera(), false);

    constexpr glm::ivec2 size = {24, 10};
    
    for (int j = 0; j < size.y; j++)
    {
        for (int i = 0; i < size.x; i++)
        {
            Renderer2D::DrawQuad({(float)i - size.x/2.0f, - (float)j + size.y/2.0f}, {1, 1}, m_TileMap[s_MapTiles[i + j * (int)size.x]]);
        }
    }
    
    Renderer2D::EndScene();
}

void Sandbox2DMapTiling::OnImGuiRender()
{
    Layer::OnImGuiRender();
    ImGuiUtility::ShowOrthographicCameraInfos(m_CameraController);
    ImGuiUtility::ShowApplicationInfoWindow(Application::Get());
}

void Sandbox2DMapTiling::OnEvent(Event& event)
{
    Layer::OnEvent(event);
    m_CameraController.OnEvent(event);
}
