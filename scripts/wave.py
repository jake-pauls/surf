import os
import sys
import subprocess

from utils import log, cmake_build_dir, is_win32, use_shell


def build_wave() -> None:
    """Builds the 'wave' client project"""
    # Check if a valid build configuration was passed
    # Argument is 'debug' by default 
    build_cfg = "debug"
    if len(sys.argv) > 2:
        build_cfg = sys.argv[1].lower()

    build_cfgs = ["debug", "release"]
    if build_cfg not in build_cfgs:
        log("No build configuration passed, using 'Debug'")
    
    log (f"Building wave in {build_cfg}")

    # Check if the build directory exists
    if not os.path.exists("./build"):
        log("Creating build directory")
        os.makedirs("./build")

    os.chdir(cmake_build_dir)

    # Construct a CMake argument based on the passed configuration
    cfg = "DEBUG"
    if build_cfg == "release":
        cfg = "RELEASE"

    log(f"Using CMake to create build artifacts in {cfg}")
    cmake_cmd = subprocess.run(["cmake", f"-DCMAKE_BUILD_TYPE={cfg}", ".."], shell=use_shell)

    # Ensure subprocess ran properly
    if cmake_cmd.returncode == 0:
        log("Successfully built cmake artifacts")
    else:
        log("Failed to build cmake artifacts")

    # Platform-specific execution information
    if is_win32:
        log("Open the Visual Studio solution with\n\t./build/surf.sln")
    else:
        log("Use 'make' to run the project")

