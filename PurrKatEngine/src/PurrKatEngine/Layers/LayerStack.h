#pragma once
#include "Layer.h"

namespace PurrKatEngine
{
    class PKE_API LayerStack
    {
    public:
        LayerStack();
        virtual ~LayerStack();

        // Overlays will **always** be on top of the layers.
        // m_LayerInsert tracks the end of the layers in the list and at the same time the beginning of overlays. 
        
        void PushLayer(Layer* layer);
        void PushOverlay(Layer* overlay);
        void PopLayer(Layer* layer);
        void PopOverlay(Layer* overlay);

        std::vector<Layer*>::iterator begin() { return m_Layers.begin(); }
        std::vector<Layer*>::iterator end() { return m_Layers.end(); }

    private:
        std::vector<Layer*> m_Layers;
        unsigned int m_LayerInsertIndex = 0;
    };
}
