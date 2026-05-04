#pragma once

#include "pkepch.h"

namespace PurrKatEngine
{
    class Shader
    {
    public:
        Shader() = default;
        virtual ~Shader() = default;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;
    };
}
