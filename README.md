<h3 align="center">🌊</h3>
<p align="center"><strike>Scripting</strike> <b>Surfing</b> the graphics pipeline.</p>

### Installation (Windows)
Surfing is (kinda) easy.

#### Development Setup
1. Install the [Vulkan SDK](https://vulkan.lunarg.com/sdk/home#windows)

2. Ensure you have a valid shader compiler installed. The Vulkan SDK should come shipped with  a [glsl compiler](https://github.com/KhronosGroup/glslang) (`glslc`) if it was installed through the SDK. Alternatively, the [DirectX shader compiler](https://github.com/microsoft/DirectXShaderCompiler) (`dxc`) can be used to compile hlsl.

```
# or... on Arch!
$ sudo pacman -S shaderc
$ sudo pacman -S directx-shader-compiler
```

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
