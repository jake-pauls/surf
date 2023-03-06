/// CODEGEN
#include "gen.h" 
/// CODEGEN

#include "surf/Bridge.h"

#include <stdio.h>
#include <caml/callback.h>

void surf_Init()
{
    // Testing Interop
    printf("Calling OCaml methods...\n");
    int x = fib(10);
    fmt_res(x);
    printf("Returned fib result %d", x);

    // Interpreting!
    char* eq = "5*5";
    char* res = interp(eq);
    printf("\n%s=%s", eq, res);
}