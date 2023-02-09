#pragma once

#include <filesystem>

namespace core
{
    using Path = std::filesystem::path;

    class FileSystem
    {
    public:
        /// @brief Wrapper to retrieve current root directory
        /// @hack CMake is currently configured to drop the solution deep in the 'build' directory 
        static inline Path GetRootDirectory() 
        { 
#if defined(WIN32)
            return std::filesystem::current_path(); 
#elif defined(__linux__)
            return std::filesystem::canonical("/proc/self/exe").parent_path();
#endif
        }

        /// @brief Wrapper to retrieve path to the shader directory
        static inline Path GetShaderDirectory() { return GetRootDirectory() / "Shaders"; }
    };
}