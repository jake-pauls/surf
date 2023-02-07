#pragma once

#include <vector>
#include <utility>

#include <vulkan/vulkan.hpp>

#include "RendererContext.h"
#include "Window.h"

namespace vkn
{
    class VkRendererContext final : public wv::RendererContext
    {
    public:
        explicit VkRendererContext(wv::Window* window);
        ~VkRendererContext() override = default;

        void Init() override;

        /// @brief Retrieves Vulkan-specific SDL window flags 
        SDL_WindowFlags GetContextSDLWindowFlags() const override;

        /// @brief Retrieves a vector containing the required Vulkan extensions for the current context
        /// @param vLayerExtensions Determines whether validation layer/debug extensions should be included in the list 
        /// @return Vector containing a string list of extension names
        std::vector<const char*> GetVulkanContextExtensions(bool vLayerExtensions = false) const;

        /// @brief Retrieves the Vulkan surface from SDL, SDL will allocate into the surface pointer
        /// @param instance Vulkan instance in use
        /// @param surface Allocated surface pointer
        void SetupSDLVulkanSurface(VkInstance instance, VkSurfaceKHR* surface) const;

        /// @brief Retrieves the Vulkan client dimensions for the current SDL windo
        VkExtent2D GetVulkanClientExtent() const;

    private:
        wv::Window* m_Window = nullptr;
    };
}