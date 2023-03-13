open Helpers

(** [let_bindings] tests for let bindings *)
let let_bindings =
  [ ( "int let binding"
    , `Quick
    , fun _ -> check_interp_int "int let binding" 10 "let x: int = 10;" empty )
  ; ( "float let binding"
    , `Quick
    , fun _ ->
        check_interp_float "float let binding" 10.045 "let x: float = 10.045;" empty )
  ; ( "adding int and float"
    , `Quick
    , fun _ ->
        check_interp_float
          "implicit float cast (addition)"
          30.045
          "let x: float = 10 + 20.045;"
          empty )
  ; ( "subtracting int and float"
    , `Quick
    , fun _ ->
        check_interp_float
          "implicit float cast (subtraction)"
          7.9
          "let y: float = 10 - 2.1;"
          empty )
  ]
;;
