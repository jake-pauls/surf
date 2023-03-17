open Surf
open Test_helpers

(** [type_errors] suite of tests to test for type errors *)
let type_errors =
  [ ( "poor type annotation #1"
    , `Quick
    , fun _ ->
        check_interp_type_error
          "poor type annotation #1"
          Errors.err_poor_type_annotation
          "let x: str = 10;"
          empty )
  ; ( "poor type annotation #2"
    , `Quick
    , fun _ ->
        check_interp_type_error
          "poor type annotation #2"
          Errors.err_poor_type_annotation
          "let x: int = \"Test\";"
          empty )
  ]
;;

(** [parser_errors] suite of tests for common parser errors *)
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

(** [syntax_errors] suite of tests for common syntax errors *)
let syntax_errors =
  [ ( "using an unregistered symbol '^'"
    , `Quick
    , fun _ -> check_interp_syntax_error "using an unregistered symbol '^'" "^;" empty )
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

(** [runtime_errors] suite of tests for common runtime errors *)
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
