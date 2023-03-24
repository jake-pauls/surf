<h3 align="center">🦞</h3>
<p align="center"><b>surf</b> communication layer.</p>

### Description
Interpreter API connecting to surf clients. Relies on codegen and injection from surf's 'gen' package in order to compile.

#### Directories

* `client` - Clients should build/link against this library to communictate through surf's networking layer
* `server` - This should be ran on a server or a local network to open communication via the surf API

### Compiler ABI Warning
Note that this solution is improvised and hacky. Specifically, this is meant to hack around my inability to compile OCaml in an MSVC environment - as a result, please ensure that the environment you run OCaml with is used for compiling the **server** portion of this project. Inversely, the compiler being used to run the client interfacing with surf should also build the API's client library.

### Compilation
Compiling and running the server or building the client library have been abstracted to be fairly straightforward in the right environment. Once completed, there should be a build folder containing both a `client` and a `server` folder with the required static and shared libraries to both run the server and link a potential client.

#### Server
The server can be compiled using cygwin and Make. **Note:** This only has support for mingw/cygwin environments at the moment.
```
# mingw/cygwin in 'api/server'
$ make server
```

Once completed successfully, the server can be run on the command-line or double-clicked. It runs on **port 3030** by default.
```
$ ./build/server/winsurf.exe
```

The server can be optionally connected to and tested with `netcat`, ex:
```
$ nc localhost 3030
```

#### Client
The client can be built/linked with CMake.
```
# mingw/cygwin in 'api/client'
$ mkdir build && cmake -Bbuild .

# OR... in your CMakeLists.txt (MSVC)
add_subdirectory(api)
target_link_libraries(${EXECUTABLE} surf::api)
```

### Dependencies
* [check](https://libcheck.github.io/check/)
* [minitrace](https://github.com/hrydgard/minitrace)
