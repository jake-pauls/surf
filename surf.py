import os
import platform
import subprocess


# Basic directories
surf_dir = os.path.dirname(os.path.realpath(__file__))
build_dir = os.path.join(surf_dir, 'build')


# Use shell for subprocesses executed on windows
is_win32 = platform.system() == 'Windows'


"""
Simple logging function
"""
def log(msg):
    print(f"[surf.py] {msg}")


def main():
    if not os.path.exists("./build"):
        log("Creating build directory")
        os.makedirs("./build")

    os.chdir(build_dir)

    log("Using CMake to create build artifacts")
    cmake_cmd = subprocess.run(["cmake", ".."], shell=is_win32)

    if cmake_cmd.returncode == 0:
        log("Successfully built cmake artifacts")
    else:
        log("Failed to build cmake artifacts")

    if is_win32:
        log("Open the Visual Studio solution with\n\t./build/surf.sln")
    else:
        log("Use 'make' to run the project")

    log("surf!")



if __name__ == '__main__':
    main()
