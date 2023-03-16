<h3 align="center">🦞</h3>
<p align="center"><b>surf</b> communication layer.</p>

### Description
Interpreter API connecting to surf clients. Relies on codegen and injection from surf's 'gen' package in order to compile.

#### Directories

* `client` - Clients should build/link against this library to communictate through surf's networking layer
* `server` - This should be ran on a server or a local network to open communication via the surf API

### Compiler ABI Warning
Note that this solution is improvised and hacky. Specifically, this is meant to hack around the inability to compile OCaml in an MSVC environment - as a result, please ensure that the environment you run OCaml with is used for compiling the **server** portion of this project. Inversely, the compiler being used to run the client should build the client library.

### Compilation
Compiling and running the server or building the client library have been abstracted to be fairly straightforward. Once completed, there should be a build folder containing both a `client` and a `server` folder with the required static and shared libraries to both run the server and link a potential client.

The server can be compiled using GNU Make. **Note:** This only has support for mingw/cygwin environments at the moment.
```
# mingw/cygwin or unix
$ make server
```

The client can be built in a similar environment, if the project is purely mingw/unix based. Or, the `api` directory can be added in CMake for linking with an MSVC project.
```
# mingw/cygwin or unix
$ make client

# OR... in your CMakeLists.txt
add_subdirectory(api)
target_link_libraries(${EXECUTABLE} surf::api)
```

Once completed successfully, the server can be run on the command-line or double-clicked. It runs on **port 3030** by default.
```
$ ./build/server/surf.exe
```

The server can be optionally connected to with `netcat`, ex:
```
$ nc localhost 3030
```