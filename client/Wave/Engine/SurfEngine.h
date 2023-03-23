#pragma once

#include <string>

#include <surf/surf.h>
#include <glm/glm.hpp>

namespace wv
{
	/// @brief Example singleton wrapper for the surf API in C++
	class SurfEngine
	{
	private:
		SurfEngine()
		{
		}

	public:
		SurfEngine(SurfEngine& other) = delete;
		SurfEngine(SurfEngine&& other) = delete;

		/// @brief Retrieves an instance of the SurfEngine singleton
		static SurfEngine& GetInstance();

		/// @brief Initializes the surf bridge and connects to the API 
		static void Start();

		/// @brief Destroys the surf bridge and disconnects from the API
		static void Destroy();

		/// @brief Interprets a single line of surf over the API 
		/// @return Returns the result of the surf line in the interpreter 
		static std::string InterpLine(const std::string& line);

		/// @brief Interprets a file line-by-line over the surf API 
		/// @param filename Name of a script in the 'core::FileSystem::SurfScriptDirectory'
		/// @return True if the file was interpreted, false otherwise
		/// @note The internal method 'surf_InterpFile' maintains a hash table of files and when they've been updated.
		///		  As such, this can be placed in update/render loops at one's discretion.
		///		  Obviously, the current version of surf (on a server) will incur some update delays for changes made to scripts in real-time.
		///		  With this in mind, it is still super cool to test/play around with. :)
		static bool InterpFile(const std::string& filename);

		/// @brief 'InterpFile' except it submits to the unmanaged surf interpreter function, meaning it won't be checked for modifications before sending
		/// @param filename Name of a script in the 'core::FileSystem::SurfScriptDirectory'
		/// @return True if the file was interpreted, false otherwise
		static bool UnmanagedInterpFile(const std::string& filename);

		/// @brief Registers a function pointer in the surf API to become available for a 'ref' callback
		static void RegisterFunction(const std::string& functionId, surf_fun_t pFunction);

		/// @brief Deregisters a function pointer in the surf API
		static void DeregisterFunction(const std::string& functionId);

		/// @brief Binds an integer variable in the current surf environment over the API
		static void BindInt(const std::string& name, int i);

		/// @brief Binds an float variable in the current surf environment over the API
		static void BindFlt(const std::string& name, float f);

		/// @brief Binds a string variable in the current surf environment over the API
		static void BindStr(const std::string& name, const std::string& s);

		/// @brief Binds a two float vector as a variable in the current surf environment over the API
		static void BindV2(const std::string& name, glm::vec2 v);

		/// @brief Binds a three float vector as a variable in the current surf environment over the API
		static void BindV3(const std::string& name, glm::vec3 v);

		/// @brief Binds a four float vector as a variable in the current surf environment over the API
		static void BindV4(const std::string& name, glm::vec4 v);

		/// @brief Retrieves an integer variable from the current surf environment over the API
		static int GetInt(const std::string& name);

		/// @brief Retrieves a float variable from the current surf environment over the API
		static float GetFlt(const std::string& name);

		/// @brief Retrieves a string variable from the current surf environment over the API
		static std::string GetStr(const std::string& name);

		/// @brief Retrieves a two float vector from the current surf environment over the API
		static glm::vec2 GetV2(const std::string& name);

		/// @brief Retrieves a three float vector from the current surf environment over the API
		static glm::vec3 GetV3(const std::string& name);

		/// @brief Retrieves a four float vector from the current surf environment over the API
		static glm::vec4 GetV4(const std::string& name);

		/// @brief Retrieves the configuration struct set by the surf API
		static constexpr surf_Cfg GetSurfCfg() { return s_SurfCfg; }

	private:
		void MStart();
		void MDestroy();

		char* MInterpLine(const char* line);
		bool MInterpFile(const char* filepath);
		bool MUnmanagedInterpFile(const char* filepath);

		void MRegisterFunction(const char* functionId, surf_fun_t pFunction);
		void MDeregisterFunction(const char* functionId);

		void MBindInt(const char* name, int i);
		void MBindFlt(const char* name, float f);
		void MBindStr(const char* name, const char* s);
		void MBindV2(const char* name, float f1, float f2);
		void MBindV3(const char* name, float f1, float f2, float f3);
		void MBindV4(const char* name, float f1, float f2, float f3, float f4);

		void MGetInt(const char* name, int* out);
		void MGetFlt(const char* name, float* out);
		void MGetStr(const char* name, char** out);
		void MGetV2(const char* name, surf_V2* out);
		void MGetV3(const char* name, surf_V3* out);
		void MGetV4(const char* name, surf_V4* out);

	private:
		static surf_Cfg s_SurfCfg;
	};
}