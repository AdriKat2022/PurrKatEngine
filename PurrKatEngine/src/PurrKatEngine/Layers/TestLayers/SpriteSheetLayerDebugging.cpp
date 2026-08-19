#include <pkepch.h>
#include "SpriteSheetLayerDebugging.h"

#include "PurrKatEngine/Profiling/Profiler.h"
#include "PurrKatEngine/Renderer/RenderCommand.h"
#include "PurrKatEngine/Renderer/Renderer2D/Renderer2D.h"
#include "PurrKatEngine/Utility/ImGuiUtility.h"

namespace PurrKatEngine
{
    SpriteSheetLayerDebugging::SpriteSheetLayerDebugging(const Ref<Texture2D>& spriteSheetTexture)
        : m_CameraController(16 / 9.0f, 1.0f, true),
          m_SpriteSheet(new SpriteSheet(spriteSheetTexture)) {}

    SpriteSheetLayerDebugging::SpriteSheetLayerDebugging(const std::string& path)
        : m_CameraController(16 / 9.0f, 1.0f, true),
          m_SpriteSheet(new SpriteSheet(path)) {}

    SpriteSheetLayerDebugging::SpriteSheetLayerDebugging(SpriteSheet* spriteSheet)
        : m_CameraController(16 / 9.0f, 1.0f, true),
          m_SpriteSheet(spriteSheet) {}

    void SpriteSheetLayerDebugging::OnAttach()
    {
        Layer::OnAttach();
        m_Sprites = m_SpriteSheet->GetSpriteArray();
        CenterCameraOnSpriteSheet();
    }

    void SpriteSheetLayerDebugging::OnDetach()
    {
        Layer::OnDetach();
    }

    void SpriteSheetLayerDebugging::OnUpdate()
    {
        PROFILE_FUNCTION();
        
        if (!m_KeepCameraCentered)
            m_CameraController.OnUpdate();
        
        RenderCommand::Clear();

        Renderer2D::BeginScene(m_CameraController.GetCamera(), false);
        
        float zoom = m_SpriteSheetZoom;

        {
            PROFILE_SCOPE("Pre-Rendering");
            
            const glm::ivec2 size = m_SpriteSheet->GetSpriteCount();
            
            for (int x = 0; x < size.x; x++)
            {
                for (int y = 0; y < size.y; y++)
                {
                    const size_t index = (size_t)x * size.y + y;

                    const glm::vec2 position{ (float)x * (1+(float)m_SpriteSheetSpacing.x/100), (float)y * (1+(float)m_SpriteSheetSpacing.y/100) };

                    if (index >= m_Sprites.size())
                        Renderer2D::DrawQuad(position, { zoom, zoom }, { 1, 0, 0, 1 });
                    else
                        Renderer2D::DrawQuad(position, { zoom, zoom }, m_Sprites[index]);
                }
            }
        }
        
        PROFILE_SCOPE("Rendering");

        Renderer2D::EndScene();
        
        if (m_KeepCameraCentered)
            CenterCameraOnSpriteSheet();
    }

    void SpriteSheetLayerDebugging::OnEvent(Event& event)
    {
        Layer::OnEvent(event);
        m_CameraController.OnEvent(event);
    }

    void SpriteSheetLayerDebugging::OnImGuiRender()
    {
        Layer::OnImGuiRender();

        static bool open = true;
        if (ImGui::Begin("SpriteSheet Debugging", &open))
        {
            auto& opts = m_SpriteSheet->GetSpriteSheetOptions();

            // -------------------------------------------------------------------------
            // Sprite Sheet
            // -------------------------------------------------------------------------

            if (ImGui::CollapsingHeader("Sprite Sheet", ImGuiTreeNodeFlags_DefaultOpen))
            {
                ImGui::TextDisabled("Atlas");
                ImGui::Separator();
                
                static bool useCellCount = false;

                const int textureWidth = static_cast<int>(m_SpriteSheet->GetTexture()->GetWidth());
                const int textureHeight = static_cast<int>(m_SpriteSheet->GetTexture()->GetHeight());

                if (ImGui::Checkbox("Use Cell Count", &useCellCount))
                {
                    if (useCellCount)
                        opts.CellCount = m_SpriteSheet->GetSpriteCount();
                    else
                    {
                        opts.CellSize = {
                            std::round((textureWidth - opts.Padding.x * (opts.CellCount.x-1)) / opts.CellCount.x),
                            std::round((textureHeight - opts.Padding.y * (opts.CellCount.y-1))/ opts.CellCount.y)
                        };
                        
                        opts.CellCount = {0,0};
                    }
                    
                    PKE_CORE_WARN("Use Cell Count: {}", useCellCount ? "true" : "false");
                }

                bool rebuildSprites = false;

                if (useCellCount)
                {
                    rebuildSprites |= ImGuiUtility::DragIntControl("Cell Count X", opts.CellCount.x, 1, textureWidth, 1, 0.5f);
                    rebuildSprites |= ImGuiUtility::DragIntControl("Cell Count Y", opts.CellCount.y, 1, textureHeight, 1, 0.5f);
                    
                    if (rebuildSprites)
                        opts.CellSize = {
                            std::round((textureWidth - opts.Padding.x * (opts.CellCount.x-1)) / opts.CellCount.x),
                            std::round((textureHeight - opts.Padding.y * (opts.CellCount.y-1))/ opts.CellCount.y)
                        };
                }
                else
                {
                    rebuildSprites |= ImGuiUtility::SliderIntControl("Cell Size X", opts.CellSize.x, 1, textureWidth);
                    rebuildSprites |= ImGuiUtility::SliderIntControl("Cell Size Y", opts.CellSize.y, 1, textureHeight);
                }

                rebuildSprites |= ImGuiUtility::SliderIntControl("Padding X", opts.Padding.x, 0, 512);
                rebuildSprites |= ImGuiUtility::SliderIntControl("Padding Y", opts.Padding.y, 0, 512);

                if (rebuildSprites)
                    m_Sprites = m_SpriteSheet->GetSpriteArray();
                
                ImGui::Spacing();

                const glm::ivec2 spriteCount = m_SpriteSheet->GetSpriteCount();

                ImGui::Text("Grid");
                ImGui::SameLine();
                ImGui::TextDisabled("%d x %d", spriteCount.x, spriteCount.y);

                ImGui::Text("Sprites");
                ImGui::SameLine();
                ImGui::TextDisabled("%zu", m_Sprites.size());
            }

            // -------------------------------------------------------------------------
            // Preview
            // -------------------------------------------------------------------------

            if (ImGui::CollapsingHeader("Preview", ImGuiTreeNodeFlags_DefaultOpen))
            {
                ImGui::DragInt2("Spacing", (int*)&m_SpriteSheetSpacing, 1, -100, 1000, "%d%%");
                ImGui::DragFloat("Zoom", &m_SpriteSheetZoom, 0.01f, 0.1f, 10.0f, "%.2fx");
                
                ImGui::Spacing();
                ImGui::Checkbox("Keep Camera Centered", &m_KeepCameraCentered);
                if (!m_KeepCameraCentered)
                    ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.2f, 1.0f), "Use WASD to move the camera.");
                ImGui::Spacing();

                if (ImGui::Button("Reset Preview"))
                {
                    m_SpriteSheetSpacing = {0,0};
                    m_SpriteSheetZoom = 1.0f;
                }
                
                ImGui::SameLine();

                if (ImGui::Button("Reset Camera"))
                    CenterCameraOnSpriteSheet();
            }

            // -------------------------------------------------------------------------
            // Info
            // -------------------------------------------------------------------------

            if (ImGui::CollapsingHeader("Info", ImGuiTreeNodeFlags_DefaultOpen))
            {
                const glm::ivec2 spriteCount = m_SpriteSheet->GetSpriteCount();

                ImGui::Text("Cell Size");
                ImGui::SameLine();
                ImGui::TextDisabled("%d x %d",
                                    (int)opts.CellSize.x,
                                    (int)opts.CellSize.y);

                ImGui::Text("Padding");
                ImGui::SameLine();
                ImGui::TextDisabled("%d x %d",
                                    (int)opts.Padding.x,
                                    (int)opts.Padding.y);

                ImGui::Text("Grid");
                ImGui::SameLine();
                ImGui::TextDisabled("%d x %d", spriteCount.x, spriteCount.y);

                ImGui::Text("Valid Sprites");
                ImGui::SameLine();
                ImGui::TextDisabled("%zu", m_Sprites.size());

                const size_t expectedSprites = (size_t)spriteCount.x * (size_t)spriteCount.y;

                if (m_Sprites.size() != expectedSprites)
                {
                    ImGui::Spacing();
                    ImGui::TextColored(
                        ImVec4(1.0f, 0.5f, 0.2f, 1.0f),
                        "Warning: expected %zu sprites, got %zu.",
                        expectedSprites,
                        m_Sprites.size()
                    );
                }
            }
        }

        ImGui::End();
        
        ImGuiUtility::ShowApplicationInfoWindow(Application::Get());
        
        static bool profilerOpen = true;
        if (ImGui::Begin("Profiler", &profilerOpen, ImGuiWindowFlags_AlwaysAutoResize))
            Profiler::ShowProfiledResultsImGui();
        ImGui::End();
    }

    void SpriteSheetLayerDebugging::CenterCameraOnSpriteSheet()
    {
        auto spriteCount = m_SpriteSheet->GetSpriteCount();
        
        if (spriteCount.x <= 0 || spriteCount.y <= 0)
            return;

        const glm::vec2 spacing = m_SpriteSheetSpacing;

        const float stepX = 1.0f + spacing.x * 0.01f;
        const float stepY = 1.0f + spacing.y * 0.01f;

        const glm::vec2 center{
            (spriteCount.x - 1) * stepX * 0.5f,
            (spriteCount.y - 1) * stepY * 0.5f
        };

        m_CameraController.SetPosition({
            center.x,
            center.y,
            m_CameraController.GetPosition().z
        });
        
        m_CameraController.SetRotation(0.0f);
    }

}
