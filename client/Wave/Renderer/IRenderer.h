#pragma once

namespace wv
{
    class Window;

    /// @brief Abstract renderer with a particular API implementation
    class IRenderer 
    {
    public:
        /// @brief Represents available graphics backends
        enum class GraphicsAPI
        {
            None = 0,
            DirectX = 1,
            Vulkan = 2
        };

    private:
        const IRenderer& operator=(const IRenderer& other) = delete;

    public:
        virtual ~IRenderer() = default;

        virtual void Init() = 0;

        virtual void Teardown() = 0;

        virtual void Clear() const = 0;

        virtual void ClearColor() const = 0;

		/// @brief Creates a renderer containing the implementation for the selected rendering API
		/// @return Unique pointer containing the constructed renderer 
		static wv::Unique<IRenderer> CreateRendererWithGAPI(Window* window, IRenderer::GraphicsAPI gapi);
    };
}