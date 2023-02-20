#pragma once

#include "Shader.h"

#include <glm/glm.hpp>

namespace vkn
{
    class VkPass;
    class VkRenderer;
    class VkHardware;

    class VkShaderPipeline : public wv::Shader
    {
        friend class VkPass;
        friend class VkRenderer;
        friend class VkHardware;

    public:
        explicit VkShaderPipeline(const VkRenderer& renderer,
            const VkHardware& hardware,
            const std::string& vertexShader, 
            const std::string& fragmentShader);

        ~VkShaderPipeline() override;

        void Create();
        void Destroy();

        void Bind(VkCommandBuffer commandBuffer) const;

    private:
        /// @brief Helper function to load a shader module and its applicable create info into Vulkan
        /// @param shaderFileName String file name for the shader
        /// @return Vulkan shader module handle 
        VkShaderModule LoadShaderModule(const std::string& shaderFileName);

    private:
        const VkRenderer& c_VkRenderer;
        const VkHardware& c_VkHardware;
        const VkRenderPass& c_RenderPass;

        std::vector<VkPipelineShaderStageCreateInfo> m_ShaderStages = {};

        VkPipelineLayout m_PipelineLayout = VK_NULL_HANDLE;
        VkPipeline m_GraphicsPipeline = VK_NULL_HANDLE;

        VkDescriptorSetLayout m_DescriptorSetLayout = VK_NULL_HANDLE;
        VkDescriptorPool m_DescriptorPool = VK_NULL_HANDLE;
    };
}