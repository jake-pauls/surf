#include "surf/Cfg.h"
#include "surf/Define.h"
#include "surf/Interp.h"

/// @brief Static variable to determine is a valid cfg.surf file has been loaded via the interpreter
static int s_CfgIsValidLoaded = SURF_FALSE;

int surf_CfgIsValidLoaded()
{
	return s_CfgIsValidLoaded;
}

surf_Cfg surf_CfgLoad(const char* surfDir)
{
	const char* cfgFileName = "cfg.surf";

	// Define a null configuration
	surf_Cfg cfg = { SURF_GAPI_NIL, SURF_SLANG_NIL, SURF_METHOD_NIL };

	char* buffer;
	const char* fmt = "%s/%s";
	int _ = ASPRINTF(&buffer, fmt, surfDir, cfgFileName);

	int result = surf_InterpFile(buffer);
	if (!result)
	{
		SURF_API_CLIENT_LOG("No cfg.surf file was found in: %s", surfDir);
		return cfg;
	}

	free(buffer);

	// Retrieve the config
	cfg.Gapi = surf_CfgGetGapi();
	cfg.Slang = surf_CfgGetSlang();
	cfg.Method = surf_CfgGetMethod();

	if (surf_CfgIsNil(&cfg))
	{
		SURF_API_CLIENT_LOG("One or more of the provided values in cfg.surf was nil, the API will not perform as intended until they're filled out");
	}
	else
	{
		s_CfgIsValidLoaded = SURF_TRUE;
	}

	return cfg;
}

surf_Gapi surf_CfgGetGapi()
{
	char* buffer;
	int result = surf_InterpGetStr("cfg_gapi", &buffer);
	if (!result)
		return SURF_GAPI_NIL;

	// Lowercase the buffer to ensure case-insensitivity 
	StringToLower(&buffer);

	if (strcmp(buffer, "vulkan") == 0)
		return SURF_GAPI_VULKAN;

	if (strcmp(buffer, "directx") == 0)
		return SURF_GAPI_DIRECTX;

	if (strcmp(buffer, "opengl") == 0)
		return SURF_GAPI_OPENGL;

	surf_InterpFreeString(buffer);

	return SURF_GAPI_NIL;
}

surf_Slang surf_CfgGetSlang()
{
	char* buffer;
	int result = surf_InterpGetStr("cfg_slang", &buffer);
	if (!result)
		return SURF_SLANG_NIL;

	// Lowercase the buffer to ensure case-insensitivity 
	StringToLower(&buffer);

	if (strcmp(buffer, "glsl") == 0)
		return SURF_SLANG_GLSL;

	if (strcmp(buffer, "hlsl") == 0)
		return SURF_SLANG_HLSL;

	return SURF_SLANG_NIL;
}

surf_Method surf_CfgGetMethod()
{
	char* buffer;
	int result = surf_InterpGetStr("cfg_method", &buffer);
	if (!result)
		return SURF_GAPI_NIL;

	// Lowercase the buffer to ensure case-insensitivity 
	StringToLower(&buffer);

	if (strcmp(buffer, "forward") == 0)
		return SURF_METHOD_FORWARD;

	if (strcmp(buffer, "deferred") == 0)
		return SURF_METHOD_DEFERRED;

	return SURF_METHOD_NIL;
}

int surf_CfgIsNil(const surf_Cfg* cfg)
{
	return cfg->Gapi == SURF_GAPI_NIL 
		|| cfg->Slang == SURF_SLANG_NIL 
		|| cfg->Method == SURF_METHOD_NIL;
}
