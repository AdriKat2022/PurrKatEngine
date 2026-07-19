#pragma once

#define CONCAT_IMPL(x, y) x##y
#define CONCAT(x, y) CONCAT_IMPL(x, y)

#define PROFILE_SCOPE(name)     Timer CONCAT(timer, __LINE__)(name, [&](ProfileResults profile) { Profiler::AddProfileResult(profile); })
#define PROFILE_DISPLAY()       Profiler::DisplayProfiledResults()
#define PROFILE_CLEAR()         Profiler::EndFrame()

namespace PurrKatEngine
{
    inline int PROFILE_SMOOTHING = 30;
    
    struct ProfileResults
    {
        const char* Name;
        float Duration = 0;
        int CurrentSmooth = 0;
        
        void AddValue(float value)
        {
            if (CurrentSmooth < PROFILE_SMOOTHING) { CurrentSmooth++; }
            else CurrentSmooth = PROFILE_SMOOTHING;
            
            Duration = (Duration * (float)(CurrentSmooth - 1) + value) / (float)CurrentSmooth;
        }
    };
    
    class Profiler
    {
    public:
        inline static bool s_EnableProfiling = false;
        inline static std::vector<ProfileResults> s_ProfileResults = std::vector<ProfileResults>();
        
        static void AddProfileResult(const ProfileResults& profileResults, int index = -1)
        {
            if (!s_EnableProfiling) return;
            
            // Insert the profile results where items after it have a >= index, and items before have a <= index.
            auto it = s_ProfileResults.begin();
            
            while (it != s_ProfileResults.end())
            {
                if (profileResults.Name == it->Name)
                {
                    it->AddValue(profileResults.Duration);
                    return;
                }
                it++;
            }
            
            s_ProfileResults.push_back(profileResults);
        }
        
        static void ClearProfilerData() { s_ProfileResults.clear(); }
        
        static void DisplayProfiledResults()
        {
            ImGui::Checkbox("Enable Profiling", &s_EnableProfiling);
            
            ImGui::DragInt("Value Smoothing", &PROFILE_SMOOTHING, 1, 1, 250);
            
            ImGui::BeginTable("Profiling", 2);
            ImGui::TableSetupColumn("");
            ImGui::TableSetupColumn("Duration (ms)");
            ImGui::TableHeadersRow();
            for (const auto& result : s_ProfileResults)
            {
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text(result.Name);
                ImGui::TableSetColumnIndex(1);
                ImGui::Text("%.3f", result.Duration);
            }
            ImGui::EndTable();
        }
    };
}
