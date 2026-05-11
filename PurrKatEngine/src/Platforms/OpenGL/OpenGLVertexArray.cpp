#include "pkepch.h"
#include "OpenGLVertexArray.h"

#include "glad/glad.h"

namespace PurrKatEngine
{
    // TO MOVE ELSEWHERE
    static GLenum ShaderDataTypeToOpenGL(ShaderDataType type)
    {
        switch (type)
        {
            case ShaderDataType::None: return GL_NONE;
            case ShaderDataType::Bool: return GL_BOOL;
            case ShaderDataType::Float: return GL_FLOAT;
            case ShaderDataType::Float2: return GL_FLOAT;
            case ShaderDataType::Float3: return GL_FLOAT;
            case ShaderDataType::Float4: return GL_FLOAT;
            case ShaderDataType::Mat3: return GL_FLOAT;
            case ShaderDataType::Mat4: return GL_FLOAT;
            case ShaderDataType::Int: return GL_INT;
            case ShaderDataType::Int2: return GL_INT;
            case ShaderDataType::Int3: return GL_INT;
            case ShaderDataType::Int4: return GL_INT;
        }

        PKE_CORE_ASSERT(false, "Unknown ShaderDataType.")
        return GL_NONE;
    }
    
    OpenGLVertexArray::OpenGLVertexArray()
    {
        glCreateVertexArrays(1, &m_RendererID);
    }

    OpenGLVertexArray::~OpenGLVertexArray()
    {
    }

    void OpenGLVertexArray::Bind() const
    {
        glBindVertexArray(m_RendererID);
    }

    void OpenGLVertexArray::Unbind() const
    {
        glBindVertexArray(0);
    }

    void OpenGLVertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)
    {
        PKE_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has no layout.")
        
        glBindVertexArray(m_RendererID); // Bind to OpenGL (we could call our Bind() definition, but it's better to keep things untangled in this case.
        vertexBuffer->Bind();
        
        uint32_t index = 0;
        
        const auto& bufferLayout = vertexBuffer->GetLayout();

        for (const auto& element : bufferLayout)
        {
            glEnableVertexAttribArray(index);
            glVertexAttribPointer(index,
                element.GetElementCount(),
                ShaderDataTypeToOpenGL(element.type),
                element.normalized ? GL_TRUE : GL_FALSE,
                bufferLayout.GetStride(),
                (const void*)element.offset);
            index++;
        }

        m_VertexBuffers.push_back(vertexBuffer);
    }

    void OpenGLVertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
    {
        glBindVertexArray(m_RendererID);
        indexBuffer->Bind();

        m_IndexBuffer = indexBuffer;
    }
}
