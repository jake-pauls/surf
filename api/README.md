<h3 align="center">🦞</h3>
<p align="center"><b>surf</b> communication layer.</p>

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