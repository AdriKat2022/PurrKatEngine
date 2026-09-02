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
        glDeleteVertexArrays(1, &m_RendererID);
    }

    void OpenGLVertexArray::Bind() const
    {
        glBindVertexArray(m_RendererID);
    }

    void OpenGLVertexArray::Unbind() const
    {
        glBindVertexArray(0);
    }

    void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer)
    {
        PKE_CORE_ASSERT(!vertexBuffer->GetLayout().GetElements().empty(), "Vertex Buffer has no layout.")
        
        glBindVertexArray(m_RendererID);
        vertexBuffer->Bind();
        
        uint32_t index = 0;
        
        const auto& bufferLayout = vertexBuffer->GetLayout();

        for (const auto& element : bufferLayout)
        {
            switch (element.type)
            {
                case ShaderDataType::None:
                case ShaderDataType::Bool:
                case ShaderDataType::Int:
                case ShaderDataType::Int2:
                case ShaderDataType::Int3:
                case ShaderDataType::Int4:
                    glEnableVertexAttribArray(index);
                    glVertexAttribIPointer(index,
                        (GLint)element.GetElementCount(),
                        ShaderDataTypeToOpenGL(element.type),
                        (GLsizei)bufferLayout.GetStride(),
                        (const void*)element.offset);
                    index++;
                    break;
                case ShaderDataType::Float:
                case ShaderDataType::Float2:
                case ShaderDataType::Float3:
                case ShaderDataType::Float4:
                    glEnableVertexAttribArray(index);
                    glVertexAttribPointer(index,
                        (GLint)element.GetElementCount(),
                        ShaderDataTypeToOpenGL(element.type),
                        element.normalized ? GL_TRUE : GL_FALSE,
                        (GLsizei)bufferLayout.GetStride(),
                        (const void*)element.offset);
                    index++;
                    break;
                case ShaderDataType::Mat3:
                case ShaderDataType::Mat4:
                    uint32_t count = element.GetElementCount();
                    for (uint32_t i = 0; i < count; i++)
                    {
                        glEnableVertexAttribArray(index);
                        glVertexAttribPointer(index,
                            (GLint)count,
                            ShaderDataTypeToOpenGL(element.type),
                            element.normalized ? GL_TRUE : GL_FALSE,
                            (GLsizei)bufferLayout.GetStride(),
                            (const void*)(element.offset + sizeof(float) * count * i));
                        glVertexAttribDivisor(index, 1);
                        index++;
                    }
                    break;
            }
        }

        m_VertexBuffers.push_back(vertexBuffer);
    }

    void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
    {
        glBindVertexArray(m_RendererID);
        indexBuffer->Bind();

        m_IndexBuffer = indexBuffer;
    }
}
