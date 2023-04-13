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

	// Run profiling
	bool g_Profile = false;
	bool g_P1 = true;
	bool g_P2 = true;
	bool g_P3 = true;
	bool g_P4 = true;
	bool g_P5 = true;
	bool g_P6 = true;
	bool g_P7 = true;
}

void wv::SurfStore::Update()
{	
	if (g_Profile)
	{
		if (g_P1)
		{
			SURF_PROFILE_START_STEP("interp file", "interpreting 1 line surf file");
			//SURF_PROFILE_START_STEP("interp file", "non-networked interp file");
			wv::SurfEngine::InterpFile("1.surf");
			//SURF_PROFILE_END_STEP("interp file", "non-networked interp file");
			SURF_PROFILE_END_STEP("interp file", "interpreting 1 line surf file");
			g_P1 = false;
		}

		if (g_P2)
		{
			SURF_PROFILE_START_STEP("interp file", "interpreting 10 line surf file");
			//SURF_PROFILE_START_STEP("interp file", "non-networked interp file");
			wv::SurfEngine::InterpFile("10.surf");
			//SURF_PROFILE_END_STEP("interp file", "non-networked interp file");
			SURF_PROFILE_END_STEP("interp file", "interpreting 10 line surf file");
			g_P2 = false;
		}

		if (g_P3)
		{
			SURF_PROFILE_START_STEP("interp file", "interpreting 25 line surf file");
			//SURF_PROFILE_START_STEP("interp file", "non-networked interp file");
			wv::SurfEngine::InterpFile("25.surf");
			//SURF_PROFILE_END_STEP("interp file", "non-networked interp file");
			SURF_PROFILE_END_STEP("interp file", "interpreting 25 line surf file");
			g_P3 = false;
		}

		if (g_P4)
		{
			SURF_PROFILE_START_STEP("interp file", "interpreting 50 line surf file");
			//SURF_PROFILE_START_STEP("interp file", "non-networked interp file");
			wv::SurfEngine::InterpFile("50.surf");
			//SURF_PROFILE_END_STEP("interp file", "non-networked interp file");
			SURF_PROFILE_END_STEP("interp file", "interpreting 50 line surf file");
			g_P4 = false;
		}

		if (g_P5)
		{
			SURF_PROFILE_START_STEP("interp file", "interpreting 100 line surf file");
			//SURF_PROFILE_START_STEP("interp file", "non-networked interp file");
			wv::SurfEngine::InterpFile("100.surf");
			//SURF_PROFILE_END_STEP("interp file", "non-networked interp file");
			SURF_PROFILE_END_STEP("interp file", "interpreting 100 line surf file");
			g_P5 = false;
		}

		if (g_P6)
		{
			SURF_PROFILE_START_STEP("interp file", "interpreting 1000 line surf file");
			//SURF_PROFILE_START_STEP("interp file", "non-networked interp file");
			wv::SurfEngine::InterpFile("1000.surf");
			//SURF_PROFILE_END_STEP("interp file", "non-networked interp file");
			SURF_PROFILE_END_STEP("interp file", "interpreting 1000 line surf file");
			g_P6 = false;
		}

		if (g_P7)
		{
			SURF_PROFILE_START_STEP("interp file", "interpreting 10000 line surf file");
			//SURF_PROFILE_START_STEP("interp file", "non-networked interp file");
			wv::SurfEngine::InterpFile("10000.surf");
			//SURF_PROFILE_END_STEP("interp file", "non-networked interp file");
			SURF_PROFILE_END_STEP("interp file", "interpreting 10000 line surf file");
			g_P7 = false;
		}
	}

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

