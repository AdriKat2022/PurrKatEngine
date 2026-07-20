#pragma once

#include <fstream>
#include "Timer.h"
#include "../imgui/imgui.h" // WHY

// TODO: Move to utility macro or something
#define CONCAT_IMPL(x, y) x##y
#define CONCAT(x, y) CONCAT_IMPL(x, y)

#define PROFILE_SCOPE(name)     ::PurrKatEngine::Timer CONCAT(timer, __LINE__)(name, [](const ProfileResults& profile) { Profiler::AddProfileResult(profile); })
#define PROFILE_FUNCTION()      PROFILE_SCOPE(__FUNCTION__)

#define PROFILE_DISPLAY()       Profiler::DisplayProfiledResults()
#define PROFILE_CLEAR()         Profiler::EndFrame()

#define PKE_ENABLE_PROFILING 1
#if PKE_ENABLE_PROFILING
#define PROFILE_SESSION_BEGIN(name, filepath)   ::PurrKatEngine::Profiler::Get()->BeginSession(name, filepath)
#define PROFILE_SESSION_END()                   ::PurrKatEngine::Profiler::Get()->EndSession()
#else
#define PROFILE_SESSION_BEGIN(name)
#define PROFILE_SESSION_END()
#endif


namespace PurrKatEngine
{
    inline int PROFILE_SMOOTHING = 30;
    
    struct ProfileResults
    {
        const char* Name;
        long long StartTime = 0;
        float Duration = 0;
        int ThreadId = 0;
        
        int CurrentSmooth = 0; // Should be private
        
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
        inline static bool s_EnableLiveProfiling = false;
        inline static bool s_EnableFileProfiling = true;
        
        inline static std::vector<ProfileResults> s_ProfileResults = std::vector<ProfileResults>();
        
        void BeginSession(const std::string& name, const std::string& filepath = "results.json")
        {
            PKE_CORE_ASSERT(!m_SessionInitialized, "Tried to begin session '{}' while session '{}' was still active.", name, m_Name);
            m_Name = name;
            m_OutputFile.open(filepath);
            m_SessionInitialized = true;
            WriteHeader();
        }
        
        void EndSession()
        {
            WriteFooter();
            m_OutputFile.close();
            m_ProfileCount = 0;
            m_SessionInitialized = false;
        }
        
        void WriteProfile(const ProfileResults& profileResults)
        {
            if (m_ProfileCount++ > 0) m_OutputFile << ",";
            
            std::string name = profileResults.Name;
            std::ranges::replace(name, '"', '\'');
            
            m_OutputFile << "{";
            m_OutputFile << R"("cat":"function",)";
            m_OutputFile << "\"dur\":" << profileResults.Duration * 1000 << ",";
            m_OutputFile << R"("name":")" << name << "\",";
            m_OutputFile << R"("ph":"X",)";
            m_OutputFile << "\"pid\":0,";
            m_OutputFile << "\"tid\":" << profileResults.ThreadId << ",";
            m_OutputFile << "\"ts\":" << profileResults.StartTime;
            m_OutputFile << "}";
            m_OutputFile.flush();
        }
        
        static Profiler* Get()
        {
            if (s_CurrentSession == nullptr) s_CurrentSession = new Profiler();
            
            return s_CurrentSession;
        };
        
        static void AddProfileResult(const ProfileResults& profileResults, int index = -1)
        {
            if (s_EnableFileProfiling && s_CurrentSession)
            {
                s_CurrentSession->WriteProfile(profileResults);
            }
            
            if (!s_EnableLiveProfiling) return;
            
            // Insert the profile results where items after it have a >= index, and items before have a <= index.
            int i = 0;
            
            while (i < s_ProfileResults.size())
            {
                if (profileResults.Name == s_ProfileResults[i].Name)
                {
                    s_ProfileResults[i].AddValue(profileResults.Duration);
                    return;
                }
                i++;
            }
            
            s_ProfileResults.push_back(profileResults);
        }
        
        static void ClearProfilerData() { s_ProfileResults.clear(); }
        
        static void DisplayProfiledResults()
        {
            ImGui::Checkbox("Enable Live Profiling", &s_EnableLiveProfiling);
            ImGui::Checkbox("Enable File Profiling", &s_EnableFileProfiling);
            
            if (s_EnableFileProfiling)
            {
                if (s_CurrentSession)   ImGui::TextColored(ImVec4(1, 0, 0, 1), "RECORDING (session '%s')", s_CurrentSession->m_Name);
                else                    ImGui::TextColored(ImVec4(0.4f, 0.4f, 0.4f, 1), "There is no session to record to.");
            }
            
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
        
    private:
        void WriteHeader()
        {
            m_OutputFile << "{\"otherData\": {}, \"traceEvents\":[";
            m_OutputFile.flush();
        }
        
        void WriteFooter()
        {
            m_OutputFile << "]}";
            m_OutputFile.flush();
        }
        
    private:
        inline static Profiler* s_CurrentSession = nullptr;
        
        bool m_SessionInitialized = false;
        bool m_Recording = false;
        long int m_ProfileCount = 0;
        std::string m_Name;
        std::ofstream m_OutputFile;
    };
}
