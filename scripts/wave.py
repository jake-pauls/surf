import os
import sys
import platform
import subprocess


# Basic directories
script_dir = os.path.dirname(os.path.realpath(__file__))
surf_dir = os.path.abspath(os.path.join(script_dir, os.pardir)) 
build_dir = os.path.join(surf_dir, 'build')


# Use shell for subprocesses executed on windows
is_win32 = platform.system() == 'Windows'


def log(msg: str) -> None:
    """
    Simple logging function
    
    Parameters
    ----------
    msg : str
        Message to be logged
    """
    print(f"[surf.py] {msg}")


def main() -> None:
    """
    Simple build script for the project that drops the CMake build into a directory
    Build configuration can be passed either as 'Debug' or 'Release' 

    ex: python wave.py debug
    """
    # Check if a valid build configuration was passed
    # Argument is 'debug' by default 
    build_cfg = "debug"
    if len(sys.argv) > 1:
        build_cfg = sys.argv[1].lower()

    build_cfgs = ["debug", "release"]
    if build_cfg not in build_cfgs:
        log("No build configuration passed, using 'Debug'")

    # Check if the build directory exists
    if not os.path.exists("./build"):
        log("Creating build directory")
        os.makedirs("./build")

    os.chdir(build_dir)

    # Construct a CMake argument based on the passed configuration
    cfg = "DEBUG"
    if build_cfg == "release":
        cfg = "RELEASE"

    log(f"Using CMake to create build artifacts in {cfg}")
    cmake_cmd = subprocess.run(["cmake", f"-DCMAKE_BUILD_TYPE={cfg}", ".."], shell=is_win32)

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

    log("surf!")


if __name__ == '__main__':
    main()
