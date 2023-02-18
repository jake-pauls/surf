<p align="center">
    <img src=".github/assets/surf-logo.png" alt="surf">
</p>
<p align="center">
    <b>Surfing</b> the graphics pipeline.
</p>

### Setup (Windows)
1. Install the [Vulkan SDK](https://vulkan.lunarg.com/sdk/home#windows)

2. Ensure you have a valid shader compiler installed. The Vulkan SDK should come shipped with  a [glsl compiler](https://github.com/KhronosGroup/glslang) (`glslc`) if it was installed through the LunarG website. Alternatively, the [DirectX shader compiler](https://github.com/microsoft/DirectXShaderCompiler) (`dxc`) can be used to compile hlsl.

3. Clone the repo and it's submodules recursively
```
$ git clone --recursive git@github.com:jake-pauls/surf.git
```

4. Run the build scripts, this will generate the client CMake project
```
$ python ./scripts/surf.py
```

5. Open the Visual Studio solution
```
$ ./build/surf.sln
```

### Setup (Linux)
1. Install the [Vulkan SDK](https://vulkan.lunarg.com/sdk/home#windows). This may be shipped with your distro's package manager.
```
# Arch Linux
$ sudo pacman -S vulkan-devel
```

2. Ensure you have a valid shader compiler installed. Similar to Windows, either `glslc` or `dxc` can be used to compile glsl or hlsl respectively. 
```
# Arch Linux
$ sudo pacman -S shaderc                 # glslc
$ sudo pacman -S directx-shader-compiler # dxc
```

3. Clone the repo and it's submodules recursively
```
$ git clone --recursive git@github.com:jake-pauls/surf.git
```

4. Run the build scripts, this will generate the client CMake project
```
$ python ./scripts/surf.py debug # Can also pass 'release' for a release build
```

5. Make the project
```
$ make -C build
```

6. Run the client executable
```
$ ./build/client/Wave/DEBUG/wave
```