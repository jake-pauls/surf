<h3 align="center">🌊</h3>
<p align="center"><strike>Scripting</strike> <b>Surfing</b> the graphics pipeline.</p>

### Installation (Windows)
Surfing is (kinda) easy.

#### Development Setup
1. Install the [Vulkan SDK](https://vulkan.lunarg.com/sdk/home#windows)

2. Clone the repo and it's submodules recursively
```
$ git clone --recursive git@github.com:jake-pauls/surf.git
```

3. Run the build scripts, this will generate the client CMake project
```
$ python ./scripts/surf.py
```

4. Open the Visual Studio solution
```
$ ./build/surf.sln
```