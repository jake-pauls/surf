<h3 align="center">🦞</h3>
<p align="center"><b>surf</b> communication layer.</p>

### Description
Interpreter API connecting to surf clients. Relies on codegen and injection from surf's 'gen' package in order to compile.

### Running
1. Create the shared library for **surf** (libsurf.dll), this contains c-stubs for all functions exported by the interpreter to C.
```
$ make dll
```

2. Compile source for target platform.
```
$ make win32
```

3. Run the executable to start the API, opens on port 3030
```
$ ./build/surf.exe
```

4. Use the client C library to test in a client app or use `netcat`, ex:
```
$ nc localhost 3030
```