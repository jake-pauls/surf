#include "wrap.h"
#include "surf/Bridge.h"

#include <stdio.h>
#include <caml/callback.h>

void surf_BridgeTest()
{
    printf("surf. Bridge Test.\n");
}

int surf_BridgeTestReturn()
{
    return 1;
}

void surf_Init()
{
    // Initialize OCaml
    char* argv[] = { "surf", NULL };
    caml_startup(argv);

    // Interop
    printf("Calling OCaml methods...\n");
    int x = fib(10);
    fmt_res(x);
    printf("Returned fib result %d", x);
}