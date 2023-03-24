#pragma once

#include <glm/glm.hpp>

namespace wv
{
    /// @brief Helper class for carrying values across implementations
	///		   This could be abstracted into an implementation or dissolved in a more specific use case of surf
    class SurfStore
    {
    public: 
		static void Update();

		// surf Bindings
		static glm::vec3 s_SurfAlbedo;
		static glm::vec3 s_SurfLightPosition;
		static glm::vec3 s_SurfLightColor;
		static float s_SurfMetallic;
		static float s_SurfRoughness;
		static float s_SurfAO;
    };
}