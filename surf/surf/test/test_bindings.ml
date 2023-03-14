open Test_helpers

(** [let_bindings] general tests for let bindings *)
let let_bindings =
  [ ( "int let binding"
    , `Quick
    , fun _ -> check_interp_int "int let binding" 10 "let x: int = 10;" empty )
  ; ( "flt let binding"
    , `Quick
    , fun _ -> check_interp_flt "float let binding" 10.045 "let x: flt = 10.045;" empty )
  ; ( "str let binding"
    , `Quick
    , fun _ ->
        check_interp_str
          "str let binding"
          "surf is really cool"
          "let copium: str = \"surf is really cool\";"
          empty )
  ; ( "v2 int let binding"
    , `Quick
    , fun _ ->
        check_interp_v2i "vec2 int let binding" [| 1; 2 |] "let x: v2 = (1, 2);" empty )
  ; ( "v2 flt let binding"
    , `Quick
    , fun _ ->
        check_interp_v2f
          "v2 flt let binding"
          [| 1.0; 2.0 |]
          "let x: v2 = (1.0, 2.0);"
          empty )
  ; ( "v3 int let binding"
    , `Quick
    , fun _ ->
        check_interp_v3i "v3 int let binding" [| 1; 2; 3 |] "let x: v3 = (1, 2, 3);" empty
    )
  ; ( "v3 flt let binding"
    , `Quick
    , fun _ ->
        check_interp_v3f
          "v3 flt let binding"
          [| 1.0; 2.0; 3.0 |]
          "let x: v3 = (1.0, 2.0, 3.0);"
          empty )
  ; ( "v4 int let binding"
    , `Quick
    , fun _ ->
        check_interp_v4i
          "v4 int let binding"
          [| 1; 2; 3; 4 |]
          "let x: v4 = (1, 2, 3, 4);"
          empty )
  ; ( "v4 flt let binding"
    , `Quick
    , fun _ ->
        check_interp_v4f
          "v4 flt let binding"
          [| 1.0; 2.0; 3.0; 4.0 |]
          "let x: v4 = (1.0, 2.0, 3.0, 4.0);"
          empty )
  ; ( "adding int and flt"
    , `Quick
    , fun _ ->
        check_interp_flt
          "implicit flt cast (addition)"
          30.045
          "let x: flt = 10 + 20.045;"
          empty )
  ; ( "subtracting int and flt"
    , `Quick
    , fun _ ->
        check_interp_flt
          "implicit flt cast (subtraction)"
          7.9
          "let y: flt = 10 - 2.1;"
          empty )
  ; ( "multiplying int and flt"
    , `Quick
    , fun _ ->
        check_interp_flt
          "implicit flt cast (multiplication)"
          21.0
          "let y: flt = 10 * 2.1;"
          empty )
  ; ( "dividing int and flt"
    , `Quick
    , fun _ ->
        check_interp_flt "implicit flt cast (division)" 5.0 "let y: flt = 10 / 2.0;" empty
    )
  ]
;;
