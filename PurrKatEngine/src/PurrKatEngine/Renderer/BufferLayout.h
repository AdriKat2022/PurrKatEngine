#pragma once

#include "PurrKatEngine/Logs/InternalLog.h"

namespace PurrKatEngine
{
    enum class ShaderDataType : uint8_t
    {
        None,
        Bool,
        Float, Float2, Float3, Float4,
        Mat3, Mat4,
        Int, Int2, Int3, Int4,
    };
    
    static uint32_t ShaderDataTypeSize(ShaderDataType type)
    {
        switch (type)
        {
            case ShaderDataType::None: return 0;
            case ShaderDataType::Bool: return sizeof(bool);
            case ShaderDataType::Float: return sizeof(float);
            case ShaderDataType::Float2: return sizeof(float)*2;
            case ShaderDataType::Float3: return sizeof(float)*3;
            case ShaderDataType::Float4: return sizeof(float)*4;
            case ShaderDataType::Mat3: return sizeof(float)*3*3;
            case ShaderDataType::Mat4: return sizeof(float)*4*4;
            case ShaderDataType::Int: return sizeof(int);
            case ShaderDataType::Int2: return sizeof(int)*2;
            case ShaderDataType::Int3: return sizeof(int)*3;
            case ShaderDataType::Int4: return sizeof(int)*4;
        }

        PKE_CORE_ASSERT(false, "Unknown ShaderDataType.")
        return 0;
    }

    static uint32_t ShaderDataTypeCount(ShaderDataType type)
    {
        switch (type)
        {
            case ShaderDataType::None: return 0;
            case ShaderDataType::Bool: return 1;
            case ShaderDataType::Float: return 1;
            case ShaderDataType::Float2: return 2;
            case ShaderDataType::Float3: return 3;
            case ShaderDataType::Float4: return 4;
            case ShaderDataType::Mat3: return 3*3;
            case ShaderDataType::Mat4: return 4*4;
            case ShaderDataType::Int: return 1;
            case ShaderDataType::Int2: return 2;
            case ShaderDataType::Int3: return 3;
            case ShaderDataType::Int4: return 4;
        }
        
        PKE_CORE_ASSERT(false, "Unknown ShaderDataType.")
        return 0;
    }
    
    struct BufferElement
    {
        std::string name;
        uint32_t offset;
        uint32_t size;
        bool normalized;
        ShaderDataType type;

        BufferElement() {}
        BufferElement(ShaderDataType type, const std::string& name, bool normalized = false)
        : name(name), offset(0), size(ShaderDataTypeSize(type)), normalized(normalized), type(type) {}

        uint32_t GetElementCount() const { return ShaderDataTypeCount(type); }
    };
    
    class BufferLayout
    {
    public:

        BufferLayout(const std::initializer_list<BufferElement>& elements) : m_Elements(elements)
        {
            SetOffsetsAndSumStride();
        }

        const std::vector<BufferElement>& GetElements() const { return m_Elements; }
        uint32_t GetStride() const { return m_Stride; }
        
        std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
        std::vector<BufferElement>::iterator end() { return m_Elements.end(); }
        std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
        std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }
        
    private:
        void SetOffsetsAndSumStride()
        {
            uint32_t offset = 0;
            for (BufferElement& element : m_Elements)
            {
                element.offset = offset;
                offset += element.size;
            }
            m_Stride = offset;
        }
        
    private:
        std::vector<BufferElement> m_Elements;
        uint32_t m_Stride = 0;
    };
    
}
