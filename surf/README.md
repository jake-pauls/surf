<h3 align="center">🏄</h3>
<p align="center">the <b>surf</b> interpreter.</p>

### Description
The interpreter for the surf language. Can be run with either a provided script or in an interactive repl environment.

### Running
1. Use `opam` to install the project dependencies

```
$ opam install --deps-only .
```

2. Use `dune` to build the project
```
$ dune build
```

3. Run surf in interactive mode, or execute a script
```
$ dune exec surf                # interactive repl
$ dune exec surf script.surf    # run script
```

### Dependencies
* [dune](https://dune.readthedocs.io/en/stable/) 
* [core](https://opensource.janestreet.com/core/) 
* [fmt](https://github.com/dbuenzli/fmt) 
* [alcotest](https://github.com/mirage/alcotest) 
* [menhir](http://cambium.inria.fr/~fpottier/menhir/) 
* [ctypes](https://github.com/yallop/ocaml-ctypes)
  * libffi
* [phantom-algebra](https://ocaml.org/p/phantom-algebra/latest)

### Note On Environments
OCaml is incredibly straightforward for Unix environments, and the exact opposite on Windows or Cygwin-based environments.

If this is being run in a Windows environment, I highly recommend installing from [the supported windows installer](https://fdopen.github.io/opam-repository-mingw/). While this only runs OCaml in a mingw environment (as opposed to MSVC), it's much less of a struggle to setup. 

Additionally, installing packages using [depext](https://fdopen.github.io/opam-repository-mingw/depext-cygwin/) will also ensure that additional packages (especially in Cygwin) are installed automatically, reducing the amount of time spent scowering the Cygwin repos for a dependency.