open Surf
open Test_helpers

(** [type_errors] test case for type errors *)
let type_errors =
  [ ( "string poor type annotation"
    , `Quick
    , fun _ ->
        check_interp_type_error
          "poor type annotation #1"
          Errors.err_poor_type_annotation
          "let x: str = 10;"
          empty )
  ; ( "integer poor type annotation"
    , `Quick
    , fun _ ->
        check_interp_type_error
          "integer poor type annotation"
          Errors.err_poor_type_annotation
          "let x: int = \"Test\";"
          empty )
  ; ( "float poor type annotation"
    , `Quick
    , fun _ ->
        check_interp_type_error
          "float poor type annotation"
          Errors.err_poor_type_annotation
          "let x: flt = \"Test\";"
          empty )
  ; ( "v2 poor type annotation"
    , `Quick
    , fun _ ->
        check_interp_type_error
          "v2 poor type annotation"
          Errors.err_poor_type_annotation
          "let x: v2 = (1,2,3);"
          empty )
  ; ( "v3 poor type annotation"
    , `Quick
    , fun _ ->
        check_interp_type_error
          "v3 poor type annotation"
          Errors.err_poor_type_annotation
          "let x: v3 = (1,2);"
          empty )
  ; ( "v4 poor type annotation"
    , `Quick
    , fun _ ->
        check_interp_type_error
          "v4 poor type annotation"
          Errors.err_poor_type_annotation
          "let x: v4 = (1,2,3);"
          empty )
  ]
;;

(** [parser_errors] test case for common parser errors *)
let parser_errors =
  [ ( "c-style declaration"
    , `Quick
    , fun _ -> check_interp_parser_error "c-style declaration" "int x = 10;" empty )
  ; ( "let binding without a semicolon"
    , `Quick
    , fun _ ->
        check_interp_parser_error
          "let binding without a type semicolon"
          "let x = \"Test\""
          empty )
  ; ( "let binding without a type annotation"
    , `Quick
    , fun _ ->
        check_interp_parser_error
          "let binding without a type annotation"
          "let x = \"Test\";"
          empty )
  ; ( "using a reserved keyword as a variable name (int)"
    , `Quick
    , fun _ ->
        check_interp_parser_error
          "using a reserved keyword as a variable name (int)"
          "let int: int = 10;"
          empty )
  ; ( "using a reserved keyword as a variable name (flt)"
    , `Quick
    , fun _ ->
        check_interp_parser_error
          "using a reserved keyword as a variable name (flt)"
          "let flt: flt = 10.02;"
          empty )
  ; ( "using a reserved keyword as a variable name (str)"
    , `Quick
    , fun _ ->
        check_interp_parser_error
          "using a reserved keyword as a variable name (str)"
          "let str: str = \"Test\";"
          empty )
  ]
;;

(** [syntax_errors] test case for common syntax errors *)
let syntax_errors =
  [ ( "using an unregistered symbol '^'"
    , `Quick
    , fun _ -> check_interp_syntax_error "using an unregistered symbol '^'" "let x: flt = 2^3;" empty )
  ; ( "using an unregistered symbol '[]'"
    , `Quick
    , fun _ ->
        check_interp_syntax_error
          "using an unregistered symbol '[]'"
          "let x: int[] = [10; 20];"
          empty )
  ; ( "using an unregistered symbol '{}'"
    , `Quick
    , fun _ ->
        check_interp_syntax_error
          "using an unregistered symbol '{}'"
          "let x: int = {10; 20};"
          empty )
  ]
;;

(** [runtime_errors] test case for common runtime errors *)
let runtime_errors =
  [ ( "using an unbound variable"
    , `Quick
    , fun _ ->
        check_interp_runtime_error
          "using an unbound variable"
          Errors.err_unbound_var
          "let x: int = variable_name;"
          empty )
  ]
;;
