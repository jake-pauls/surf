#include <caml/callback.h>

/// Used by codegen to inject the OCaml runtime into the API
/// Initializes the OCaml runtime when the shared library is loaded

__attribute__ ((__constructor__))
void Init()
{
    char* argv[] = { "surf api.", NULL };
    caml_startup(argv);
}