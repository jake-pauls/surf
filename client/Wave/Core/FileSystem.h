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
            return std::filesystem::current_path()
                .parent_path()
                .parent_path()
                .parent_path() / "client" / "Wave"; 
        }

        /// @brief Wrapper to retrieve path to the shader directory
        static inline Path GetShaderDirectory() { return GetRootDirectory() / "Renderer" / "Shaders"; }

        /// @brief Wrapper to retrieve path to the SPIR-V bin directory 
        static inline Path GetSPIRVDirectory() { return GetShaderDirectory() / "spv"; }
    };
}