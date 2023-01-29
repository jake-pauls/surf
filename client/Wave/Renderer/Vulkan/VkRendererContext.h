#pragma once

#include "RendererContext.h"

#include "Window.h"

namespace vk
{
    class VkRendererContext final : public wv::RendererContext
    {
    public:
        VkRendererContext(wv::Window* window);
        ~VkRendererContext() override = default;

        void Init() override;

        SDL_WindowFlags GetContextSDLWindowFlags() override;

    private:
        wv::Window* m_Window = nullptr;
    };
}