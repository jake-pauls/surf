<p align="center">
    <img src=".github/assets/surf.png" alt="surf">
</p>
<p align="center">
    <b>Surfing</b> the graphics pipeline.
</p>

## Setup
Setting this project up manually is quite dense. I've tried to abstract as much of it away as possible but some caveats and hiccups will remain.

The setup below uses a simple set of custom python scripts to run the build commands I've set out for each package (interpreter, api, and client app). For package-specific compilation/execution steps not explicitly covered by those scripts, please refer to each folder's respective README.

Note that at the moment there is only support for running this in a **Windows** environment, with the server running on **MinGW** in **Cygwin** and the client running through **MSVC**.

### Setup (Windows)
1. Install the [Vulkan SDK](https://vulkan.lunarg.com/sdk/home#windows).

2. Ensure you have a valid shader compiler installed. The Vulkan SDK should come shipped with  a [glsl compiler](https://github.com/KhronosGroup/glslang) (`glslc`) if it was installed through the LunarG website. Alternatively, the [DirectX shader compiler](https://github.com/microsoft/DirectXShaderCompiler) (`dxc`) can be used to compile hlsl.

3. Install [OCaml](https://fdopen.github.io/opam-repository-mingw/installation/), for more information on the intricacies and caveats of this process, refer to the interpreter's [README](https://github.com/jake-pauls/surf/blob/main/surf/README.md).

4. Clone the repo and it's submodules recursively.
```
$ git clone --recursive git@github.com:jake-pauls/surf.git
```

5. In Cygwin, run the build scripts for the interpreter (requires OCaml/opam/dune).
```
# mingw/cygwin
$ python ./scripts/surf.py surf
```   

6. In Cygwin, run the build scripts for the API (requires OCaml/dune/flexlink). This builds the **server** side of the API.
```
# mingw/cygwin
$ python ./scripts/surf.py api
```

7. In Powershell, run the build scripts for the client CMake project. This step also builds the surf API's **client** side of the library.
```
# powershell - debug
$ python ./scripts/surf.py wave

# powershell - client with profiling
$ python ./scripts/surf.py wave profile
```

8. In Cygwin, start the server.
```
# mingw/cygwin
$ ./api/build/server/surf.exe
```

9. Open the Visual Studio solution.
```
$ ./build/surf.sln
```