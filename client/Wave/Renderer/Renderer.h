#pragma once

namespace wv
{
    class Window;

    /// @brief Abstract renderer with a particular API implementation
    class Renderer 
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
        const Renderer& operator=(const Renderer& other) = delete;

    public:
        virtual ~Renderer() = default;

        virtual void Init() = 0;
        
        virtual void Draw() = 0;

        virtual void Teardown() = 0;

        virtual void Clear() const = 0;

        virtual void ClearColor() const = 0;

		/// @brief Creates a renderer containing the implementation for the selected rendering API
		/// @return Unique pointer containing the constructed renderer 
		static core::Unique<Renderer> CreateRendererWithGAPI(Window* window, Renderer::GraphicsAPI gapi);
    };
}