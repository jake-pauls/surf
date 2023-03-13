open Helpers

(** [let_bindings] general tests for let bindings *)
let let_bindings =
  [ ( "int let binding"
    , `Quick
    , fun _ -> check_interp_int "int let binding" 10 "let x: int = 10;" empty )
  ; ( "float let binding"
    , `Quick
    , fun _ ->
        check_interp_float "float let binding" 10.045 "let x: float = 10.045;" empty )
  ; ( "str let binding"
    , `Quick
    , fun _ ->
        check_interp_str
          "str let binding"
          "surf is really cool"
          "let copium: str = \"surf is really cool\";"
          empty )
  ; ( "vec2 int let binding"
    , `Quick
    , fun _ ->
        check_interp_vec2i "vec2 int let binding" [| 1; 2 |] "let x: vec2 = (1, 2);" empty
    )
  ; ( "vec2 float let binding"
    , `Quick
    , fun _ ->
        check_interp_vec2f
          "vec2 float let binding"
          [| 1.0; 2.0 |]
          "let x: vec2 = (1.0, 2.0);"
          empty )
  ; ( "vec3 int let binding"
    , `Quick
    , fun _ ->
        check_interp_vec3i
          "vec3 int let binding"
          [| 1; 2; 3 |]
          "let x: vec3 = (1, 2, 3);"
          empty )
  ; ( "vec3 float let binding"
    , `Quick
    , fun _ ->
        check_interp_vec3f
          "vec3 float let binding"
          [| 1.0; 2.0; 3.0 |]
          "let x: vec3 = (1.0, 2.0, 3.0);"
          empty )
  ; ( "vec4 int let binding"
    , `Quick
    , fun _ ->
        check_interp_vec4i
          "vec4 int let binding"
          [| 1; 2; 3; 4 |]
          "let x: vec4 = (1, 2, 3, 4);"
          empty )
  ; ( "vec4 float let binding"
    , `Quick
    , fun _ ->
        check_interp_vec4f
          "vec4 float let binding"
          [| 1.0; 2.0; 3.0; 4.0 |]
          "let x: vec4 = (1.0, 2.0, 3.0, 4.0);"
          empty )
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
  ; ( "multiplying int and float"
    , `Quick
    , fun _ ->
        check_interp_float
          "implicit float cast (multiplication)"
          21.0
          "let y: float = 10 * 2.1;"
          empty )
  ; ( "dividing int and float"
    , `Quick
    , fun _ ->
        check_interp_float
          "implicit float cast (division)"
          5.0
          "let y: float = 10 / 2.0;"
          empty )
  ]
;;
