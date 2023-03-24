#include "SurfStore.h"

#include "SurfEngine.h"

namespace wv
{
	glm::vec3 SurfStore::s_SurfAlbedo = {};
	glm::vec3 SurfStore::s_SurfLightPosition = {};
	glm::vec3 SurfStore::s_SurfLightColor = {};
	float SurfStore::s_SurfMetallic = 0.0f;
	float SurfStore::s_SurfRoughness = 0.0f;
	float SurfStore::s_SurfAO = 0.0f;
}

void wv::SurfStore::Update()
{
	// Load PBR surf script and retrieve it's variables
	bool pbrSurfUpdates = wv::SurfEngine::InterpFile("pbr.surf");
	if (pbrSurfUpdates)
	{
		s_SurfAlbedo = wv::SurfEngine::GetV3("pbr_albedo");
		s_SurfMetallic = wv::SurfEngine::GetFlt("pbr_metallic");
		s_SurfRoughness = wv::SurfEngine::GetFlt("pbr_roughness");
		s_SurfAO = wv::SurfEngine::GetFlt("pbr_ao");
		s_SurfLightPosition = wv::SurfEngine::GetV3("pbr_light_position");
		s_SurfLightColor = wv::SurfEngine::GetV3("pbr_light_color");
	}

	// Run C function callback surf script - functions are registered in 'VkRenderer'
	wv::SurfEngine::InterpFile("fun.surf");
}

