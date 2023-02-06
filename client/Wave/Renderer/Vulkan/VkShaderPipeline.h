#pragma once

#include "Shader.h"

namespace vkn
{
    class VkHardware;

    class VkShaderPipeline : public wv::Shader
    {
        friend class VkHardware;

    public:
        explicit VkShaderPipeline(const VkHardware& hardware, const std::string& vertexShader, const std::string& fragmentShader);
        ~VkShaderPipeline() override;

        void Bind() override;
        void Unbind() override;

    private:
        /// @brief Helper function to load a shader module and its applicable create info into Vulkan
        /// @param shaderFileName String file name for the shader
        /// @return Vulkan shader module handle 
        VkShaderModule LoadShaderModule(const std::string& shaderFileName);

    private:
		const VkHardware& c_VkHardware;

        VkShaderModule m_VertexShaderModule = VK_NULL_HANDLE;
        VkShaderModule m_FragmentShaderModule = VK_NULL_HANDLE;
    };
}