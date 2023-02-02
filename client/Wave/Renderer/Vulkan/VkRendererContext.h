#pragma once

#include <vector>

#include "IRendererContext.h"
#include "Window.h"

namespace vkn
{
    class VkRendererContext final : public wv::IRendererContext
    {
    public:
        explicit VkRendererContext(wv::Window* window);
        ~VkRendererContext() override = default;

        void Init() override;

        /// @brief Retrieves Vulkan-specific SDL window flags 
        SDL_WindowFlags GetContextSDLWindowFlags() override;

        /// @brief Retrieves a vector containing the required Vulkan extensions for the current context
        /// @param vLayerExtensions Determines whether validation layer/debug extensions should be included in the list 
        /// @return Vector containing a string list of extension names
        std::vector<const char*> GetVulkanContextExtensions(bool vLayerExtensions = false) const;

    private:
        wv::Window* m_Window = nullptr;
    };
}