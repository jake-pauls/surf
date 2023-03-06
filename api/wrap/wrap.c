#include <stdio.h>
#include <string.h>

#include <caml/mlvalues.h>
#include <caml/callback.h>

int fib(int n)
{
    static const value* fibClosure = NULL;
    if (fibClosure == NULL) fibClosure = caml_named_value("fib");
    return Int_val(caml_callback(*fibClosure, Val_int(n)));
}

char* fmt_res(int n)
{
    static const value* fmtResClosure = NULL;
    if (fmtResClosure == NULL) fmtResClosure = caml_named_value("fmt_res");
    return strdup(String_val(caml_callback(*fmtResClosure, Val_int(n))));
}