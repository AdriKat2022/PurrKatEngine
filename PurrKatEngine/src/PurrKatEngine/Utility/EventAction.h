#pragma once

namespace PurrKatEngine
{
    template<typename... Args>
    class EventAction
    {
    public:
        using Handler = std::function<void(Args...)>;
        using ListenerReference = int;
        
        struct ActionObject
        {
            Handler ActionHandler;
            int Id;
        };
        
        explicit EventAction() = default;
        explicit EventAction(const Handler& defautHandler) { AddListener(defautHandler); }

        ListenerReference AddListener(Handler handler)
        {
            int id = m_CurrentId++;
            m_Listeners.push_back(ActionObject{std::move(handler), id});
            return id;
        }
        
        bool RemoveListener(ListenerReference id)
        {
            return 0 < std::erase_if(m_Listeners, [id](const ActionObject& obj) { return obj.Id == id; });
        }
        
        void ClearListeners()
        {
            m_Listeners.clear();
        }
        
        void Invoke(Args... args) const
        {
            for (const auto& listener : m_Listeners)
            {
                listener.ActionHandler(args...);
            }
        }

        int operator+=(const Handler& handler) { return AddListener(handler); }
        bool operator-=(ListenerReference id) { return RemoveListener(id); }
        
    private:
        std::vector<ActionObject> m_Listeners;
        int m_CurrentId = 0;
    };
}
