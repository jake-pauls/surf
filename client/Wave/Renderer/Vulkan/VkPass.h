#pragma once

#include <vulkan/vulkan.h>

namespace vkn
{
    class VkRenderer;
    class VkShaderPipeline;

    class VkPass
    {
        friend class VkRenderer;
        friend class VkShaderPipeline;

    public:
        explicit VkPass(const VkRenderer& renderer);
        ~VkPass();        

        void Create();
        void Destroy();

    private:
        const VkRenderer& c_VkRenderer;

        VkRenderPass m_RenderPass = VK_NULL_HANDLE;
    };
}