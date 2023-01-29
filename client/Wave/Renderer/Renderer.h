#pragma once

namespace wv
{
    /// @brief Abstract renderer with a particular API implementation
    class Renderer 
    {
    public:
        /// @brief Represents available graphics backends
        enum class GraphicsAPI
        {
            None = 0,
            Vulkan = 1
        };

    public:
        virtual ~Renderer() = default;

        virtual void Init() = 0;

        virtual void Clear() const = 0;

        virtual void ClearColor() const = 0;

		/// @brief Creates a renderer containing the implementation for the selected rendering API
		/// @return Unique pointer containing the constructed renderer 
		static wv::Unique<Renderer> CreateRendererWithAPISpec(wv::Renderer::GraphicsAPI api);
    };
}