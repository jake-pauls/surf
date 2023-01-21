/* Code taken from sample on https://v2.ocaml.org/manual/intfc.html */

#include <stdio.h>
#include <caml/callback.h>

extern int fib(int n);
extern char* fmt_res(int n);

int main(int argc, char** argv)
{
    // Initialize OCaml
    caml_startup(argv);

    // Interop
    printf("Calling OCaml methods...\n");
    int x = fib(10);
    fmt_res(x);
    printf("Returned fib result %d", x);
}