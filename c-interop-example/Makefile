##
## OCaml/C interop example from https://v2.ocaml.org/manual/intfc.html#s%3Ac-callback
## Compiled on Windows with Cygwin
##

.PHONY: lib compile clean-all clean-int

lib:
	mkdir -p build 

	# Compile the OCaml
	ocamlc -custom -output-obj -o build/modcaml.o base.ml

	# Build the C wrapper
	gcc -I`ocamlc -where` -o build/modwrap.o -c modwrap.c

	# Statically add it to the OCaml runtime
	cp `ocamlc -where`/libcamlrun.a build/mod.a && chmod +w build/mod.a
	ar r build/mod.a build/modcaml.o build/modwrap.o

compile:
	gcc -c main.c -o build/main.o -I`ocamlc -where`

	# Ensure the flexlink has other intermediate files in this dir or assigned to the FLEXDIR environment variable
	flexlink -chain mingw64 -exe -o surf.exe \
		build/main.o build/mod.a -lversion -lws2_32

clean-all:
	rm -rf build
	rm -f surf.exe
	$(MAKE) clean-int

clean-int:
	rm -f *.cm*
	rm -f *.o