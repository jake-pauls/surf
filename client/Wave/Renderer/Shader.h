#pragma once

#include <string>

namespace wv 
{
    class Shader 
    {
    public:
        explicit Shader(const std::string& vertexShader, const std::string& fragmentShader)
            : m_VertexShaderFile(vertexShader.c_str())
            , m_FragmentShaderFile(fragmentShader.c_str())
        {
        }

        virtual ~Shader() = default;

        virtual void Bind() = 0;
        virtual void Unbind() = 0;

    protected:
        const char* m_VertexShaderFile = nullptr;
        const char* m_FragmentShaderFile = nullptr;
    };
}