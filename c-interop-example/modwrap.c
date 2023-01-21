/* Code taken from sample on https://v2.ocaml.org/manual/intfc.html */

#include <stdio.h>
#include <string.h>
#include <caml/mlvalues.h>
#include <caml/callback.h>

int fib(int n)
{
    static const value* fib_closure = NULL;
    if (fib_closure == NULL)
        fib_closure = caml_named_value("fib");

    return Int_val(caml_callback(*fib_closure, Val_int(n)));
}

char* fmt_res(int n)
{
    static const value* fmt_res_closure = NULL;
    if (fmt_res_closure == NULL)
        fmt_res_closure = caml_named_value("fmt_res");

    return strdup(String_val(caml_callback(*fmt_res_closure, Val_int(n))));
    /* We copy the C string returned by String_val to the C heap
     so that it remains valid after garbage collection. */
}