#pragma once

#include "Shader.h"

namespace vkn
{
    class VkPass;
    class VkRenderer;

    class VkShaderPipeline : public wv::Shader
    {
        friend class VkPass;
        friend class VkRenderer;

    public:
        explicit VkShaderPipeline(
            const VkRenderer& renderer,
            const VkDevice& device,
            const std::string& vertexShader, 
            const std::string& fragmentShader
        );

        ~VkShaderPipeline() override;

        void Create();
        void Destroy();

        void Bind();

        void Unbind() override;

    private:
        /// @brief Helper function to load a shader module and its applicable create info into Vulkan
        /// @param shaderFileName String file name for the shader
        /// @return Vulkan shader module handle 
        VkShaderModule LoadShaderModule(const std::string& shaderFileName);

    private:
        const VkRenderer& c_VkRenderer;
        const VkDevice& c_LogicalDevice;
        const VkRenderPass& c_RenderPass;

        std::vector<VkPipelineShaderStageCreateInfo> m_ShaderStages = {};

        VkPipelineLayout m_PipelineLayout = VK_NULL_HANDLE;
        VkPipeline m_GraphicsPipeline = VK_NULL_HANDLE;
    };
}