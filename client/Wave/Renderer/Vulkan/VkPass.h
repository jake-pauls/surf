#pragma once

#include <vulkan/vulkan.h>

namespace vkn
{
    class VkRenderer;
    class VkHardware;
    class VkSwapChain;
    class VkShaderPipeline;

    class VkPass
    {
        friend class VkRenderer;
        friend class VkHardware;
        friend class VkSwapChain;
        friend class VkShaderPipeline;

    public:
        explicit VkPass(const VkDevice& device, const VkSwapChain& swapChain);
        ~VkPass();        

        void Create();
        void Destroy();

    private:
        const VkDevice& c_LogicalDevice;
        const VkSwapChain& c_VkSwapChain;

        VkRenderPass m_RenderPass = VK_NULL_HANDLE;
    };
}