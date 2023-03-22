#ifndef Cfg_h
#define Cfg_h

#include "surf/Define.h"

/// @brief Represents the selectable graphics APIs for surf clients
typedef enum surf_Gapi
{
	SURF_GAPI_NIL,
	SURF_GAPI_VULKAN,
	SURF_GAPI_DIRECTX,
	SURF_GAPI_OPENGL
} surf_Gapi;

/// @brief Represents the selectable shader languages for surf clients
typedef enum surf_Slang
{
	SURF_SLANG_NIL,
	SURF_SLANG_GLSL,
	SURF_SLANG_HLSL
} surf_Slang;

/// @brief Represents the selectable rendering methods for surf clients 
typedef enum surf_Method
{
	SURF_METHOD_NIL,
	SURF_METHOD_FORWARD,
	SURF_METHOD_DEFERRED
} surf_Method;

/// @brief Represents a surf configuration
typedef struct surf_Cfg
{
	surf_Gapi Gapi;
	surf_Slang Slang;
	surf_Method Method;
} surf_Cfg;

/// @brief Checks if a 'cfg.surf' file has been loaded and is valid
/// @return True if a 'cfg.surf' file has been loadeed and is valid, false if not
int surf_CfgIsValidLoaded();

/// @brief Searches a directory of surf scripts non-recursively for a 'cfg.surf' file
/// @param surfDir Directory containing surf scripts to search
/// @return True if loading the config succeeded, false if it failed
surf_Cfg surf_CfgLoad(const char* surfDir);

/// @brief Retrieves the 'cfg_gapi' variable from the surf interpreter
/// @return Enum representing the config's provided graphics api
surf_Gapi surf_CfgGetGapi();

/// @brief Retrieves the 'cfg_slang' variable from the surf interpreter
/// @return Enum representing the config's provided shader language
surf_Slang surf_CfgGetSlang();

/// @brief Retrieves the 'cfg_method' variable from the surf interpreter 
/// @return Enum representing the config's provided rendering method
surf_Method surf_CfgGetMethod();

/// @brief Checks if a provided surf configuration is invalid or nil (all required configuration variables are provided)
/// @return True if the config is nil, false if the config has an invalid configuration field 
int surf_CfgIsInvalid(const surf_Cfg* cfg);

#endif /* Cfg_h */