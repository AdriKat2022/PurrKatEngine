#pragma once

#include <chrono>
#include "PurrKatEngine/Logs/Log.h"

namespace PurrKatEngine
{
    template<typename Fn = std::nullptr_t>
    class Timer
    {
    public:
        Timer(const char* name, float* outResult, bool printResult) : m_Name(name), m_PrintResult(printResult), m_OutResult(outResult)
        {
            m_StartTime = std::chrono::high_resolution_clock::now();
        }
        
        Timer(const char* name, Fn callbackFunc) : m_Name(name), m_CallbackFunc(callbackFunc)
        {
            m_StartTime = std::chrono::high_resolution_clock::now();
        }
        
        ~Timer()
        {
            if (!m_Stopped) Stop();
        }

        void Stop()
        {
            auto endTime = std::chrono::high_resolution_clock::now();

            m_Stopped = true;

            long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTime).time_since_epoch().count();
            long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTime).time_since_epoch().count();

            // Put in milliseconds.
            float duration = (end - start) * 0.001f;

            if (m_OutResult)
            {
                *m_OutResult = duration;
            }
            if (m_PrintResult)
            {
                PKE_LOG_INFO("Timer: {} took {} µs", m_Name, duration);
            }
            if constexpr (!std::is_same_v<Fn, std::nullptr_t>)
            {
                int threadId = std::hash<std::thread::id>{}(std::this_thread::get_id()); // Trick to transform the thread id to an integer.
                
                // Only calling if the type is NOT nullptr.
                m_CallbackFunc({m_Name, start, duration, threadId});
            }
        }
        
    private:
        bool m_Stopped = false;
        std::chrono::high_resolution_clock::time_point m_StartTime;
        
        const char* m_Name;
        bool m_PrintResult = false;
        float* m_OutResult = nullptr;
        Fn m_CallbackFunc = nullptr;
    };
}
