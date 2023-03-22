#include "SurfEngine.h"

namespace wv
{
	surf_Cfg SurfEngine::s_SurfCfg;
}

wv::SurfEngine& wv::SurfEngine::GetInstance()
{
	static SurfEngine instance;
	return instance;
}

void wv::SurfEngine::Start()
{
	GetInstance().MStart();
}

void wv::SurfEngine::Destroy()
{
	GetInstance().MDestroy();
}

std::string wv::SurfEngine::InterpLine(const std::string& line)
{
	char* result = GetInstance().MInterpLine(line.c_str());

	// Copy and free the result for clients
	std::string strResult = std::string(result);
	surf_InterpFreeString(result);

	return strResult;
}

bool wv::SurfEngine::InterpFile(const std::string& filename)
{
	std::string filepath = core::FileSystem::GetSurfScriptPath(filename.c_str()).string();
	WAVE_ASSERT(!filepath.empty(), "Attempted to interpret a file over the surf API that wasn't found in the scripts directory")

	return GetInstance().MInterpFile(filepath.c_str());
}

void wv::SurfEngine::RegisterFunction(const std::string& functionId, surf_fun_t pFunction)
{
	GetInstance().MRegisterFunction(functionId.c_str(), pFunction);
}

void wv::SurfEngine::DeregisterFunction(const std::string& functionId)
{
	GetInstance().MDeregisterFunction(functionId.c_str());
}

void wv::SurfEngine::BindInt(const std::string& name, int i)
{
	GetInstance().MBindInt(name.c_str(), i);
}

void wv::SurfEngine::BindFlt(const std::string& name, float f)
{
	GetInstance().MBindFlt(name.c_str(), f);
}

void wv::SurfEngine::BindStr(const std::string& name, const std::string& s)
{
	GetInstance().MBindStr(name.c_str(), s.c_str());
}

void wv::SurfEngine::BindV2(const std::string& name, glm::vec2 v)
{
	GetInstance().MBindV2(name.c_str(), v.x, v.y);
}

void wv::SurfEngine::BindV3(const std::string& name, glm::vec3 v)
{
	GetInstance().MBindV3(name.c_str(), v.x, v.y, v.z);
}

void wv::SurfEngine::BindV4(const std::string& name, glm::vec4 v)
{
	GetInstance().MBindV4(name.c_str(), v.x, v.y, v.z, v.w);
}

int wv::SurfEngine::GetInt(const std::string& name)
{
	int value = 0;
	GetInstance().MGetInt(name.c_str(), &value);

	return value;
}

float wv::SurfEngine::GetFlt(const std::string& name)
{
	float value = 0.0f;
	GetInstance().MGetFlt(name.c_str(), &value);

	return value;
}

std::string wv::SurfEngine::GetStr(const std::string& name)
{
	char* value;
	GetInstance().MGetStr(name.c_str(), &value);

	// Copy and free the resultant value
	std::string strValue = std::string(value);
	surf_InterpFreeString(value);

	return strValue;
}

glm::vec2 wv::SurfEngine::GetV2(const std::string& name)
{
	surf_V2 value = {};
	GetInstance().MGetV2(name.c_str(), &value);

	return { value.x, value.y };
}

glm::vec3 wv::SurfEngine::GetV3(const std::string& name)
{
	surf_V3 value = {};
	GetInstance().MGetV3(name.c_str(), &value);

	return { value.x, value.y, value.z };
}

glm::vec4 wv::SurfEngine::GetV4(const std::string& name)
{
	surf_V4 value = {};
	GetInstance().MGetV4(name.c_str(), &value);

	return { value.x, value.y, value.z, value.w };
}

void wv::SurfEngine::MStart()
{
	core::Log(ELogType::Warn, "Starting surf bridge");

	// Open the surf bridge
	surf_ApiResult result = surf_StartBridge();
	WAVE_ASSERT(result != SURF_API_ERROR, "Failed to connect to the surf API, calls to the surf API and surf-dependent features will not work");

	std::string cfgSurf = core::FileSystem::GetSurfScriptPath("cfg.surf").string();

	// Provide surf a scripts directory to find a 'cfg.surf' file
	s_SurfCfg = surf_CfgLoad(cfgSurf.c_str());
	if (surf_CfgIsInvalid(&s_SurfCfg))
		core::Log(ELogType::Warn, "'cfg.surf' either wasn't found or has an invalid configuration");
}

void wv::SurfEngine::MDestroy()
{
	core::Log(ELogType::Warn, "Disconnecting the surf bridge");

	// Disconnect the bridge
	surf_ApiResult result = surf_DestroyBridge();
	WAVE_ASSERT(result != SURF_API_ERROR, "Failed to destroy surf bridge");
}

char* wv::SurfEngine::MInterpLine(const char* line)
{
	return surf_InterpLine(line);
}

bool wv::SurfEngine::MInterpFile(const char* filepath)
{
	return surf_InterpFile(filepath);
}

void wv::SurfEngine::MRegisterFunction(const char* functionId, surf_fun_t pFunction)
{
	surf_InterpRegisterSymbol(functionId, pFunction);
}

void wv::SurfEngine::MDeregisterFunction(const char* functionId)
{
	surf_InterpDeregisterSymbol(functionId);
}

void wv::SurfEngine::MBindInt(const char* name, int i)
{
	surf_InterpBindInt(name, i);
}

void wv::SurfEngine::MBindFlt(const char* name, float f)
{
	surf_InterpBindFlt(name, f);
}

void wv::SurfEngine::MBindStr(const char* name, const char* s)
{
	surf_InterpBindStr(name, s);
}

void wv::SurfEngine::MBindV2(const char* name, float f1, float f2)
{
	surf_InterpBindV2(name, f1, f2);
}

void wv::SurfEngine::MBindV3(const char* name, float f1, float f2, float f3)
{
	surf_InterpBindV3(name, f1, f2, f3);
}

void wv::SurfEngine::MBindV4(const char* name, float f1, float f2, float f3, float f4)
{
	surf_InterpBindV4(name, f1, f2, f3, f4);
}

void wv::SurfEngine::MGetInt(const char* name, int* out)
{
	bool result = surf_InterpGetInt(name, out);
	WAVE_ASSERT(result, "Failed to retrieve integer variable from surf");
}

void wv::SurfEngine::MGetFlt(const char* name, float* out)
{
	bool result = surf_InterpGetFlt(name, out);
	WAVE_ASSERT(result, "Failed to retrieve float variable from surf");
}

void wv::SurfEngine::MGetStr(const char* name, char** out)
{
	bool result = surf_InterpGetStr(name, out);
	WAVE_ASSERT(result, "Failed to retrieve string variable from surf");
}

void wv::SurfEngine::MGetV2(const char* name, surf_V2* out)
{
	bool result = surf_InterpGetV2(name, out);
	WAVE_ASSERT(result, "Failed to retrieve two float vector from surf");
}

void wv::SurfEngine::MGetV3(const char* name, surf_V3* out)
{
	bool result = surf_InterpGetV3(name, out);
	WAVE_ASSERT(result, "Failed to retrieve three float vector from surf");
}

void wv::SurfEngine::MGetV4(const char* name, surf_V4* out)
{
	bool result = surf_InterpGetV4(name, out);
	WAVE_ASSERT(result, "Failed to retrieve four float vector from surf");
}
