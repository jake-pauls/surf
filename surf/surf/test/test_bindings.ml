open Test_helpers

(** [let_bindings] general tests for let bindings, variables are bound in an [empty]
    static environment *)
let let_bindings =
  [ ( "integer let binding"
    , `Quick
    , fun _ -> check_interp_int "integer let binding" 10 "let x: int = 10;" empty )
  ; ( "float let binding"
    , `Quick
    , fun _ -> check_interp_flt "float let binding" 10.045 "let x: flt = 10.045;" empty )
  ; ( "string let binding"
    , `Quick
    , fun _ ->
        check_interp_str "string let binding" "test" "let test: str = \"test\";" empty )
  ; ( "v2 integer let binding"
    , `Quick
    , fun _ ->
        check_interp_v2i "v2 integer let binding" [| 1; 2 |] "let x: v2 = (1, 2);" empty
    )
  ; ( "v2 float let binding"
    , `Quick
    , fun _ ->
        check_interp_v2f
          "v2 float let binding"
          [| 1.0; 2.0 |]
          "let x: v2 = (1.0, 2.0);"
          empty )
  ; ( "v3 integer let binding"
    , `Quick
    , fun _ ->
        check_interp_v3i
          "v3 integer let binding"
          [| 1; 2; 3 |]
          "let x: v3 = (1, 2, 3);"
          empty )
  ; ( "v3 float let binding"
    , `Quick
    , fun _ ->
        check_interp_v3f
          "v3 float let binding"
          [| 1.0; 2.0; 3.0 |]
          "let x: v3 = (1.0, 2.0, 3.0);"
          empty )
  ; ( "v4 integer let binding"
    , `Quick
    , fun _ ->
        check_interp_v4i
          "v4 integer let binding"
          [| 1; 2; 3; 4 |]
          "let x: v4 = (1, 2, 3, 4);"
          empty )
  ; ( "v4 float let binding"
    , `Quick
    , fun _ ->
        check_interp_v4f
          "v4 float let binding"
          [| 1.0; 2.0; 3.0; 4.0 |]
          "let x: v4 = (1.0, 2.0, 3.0, 4.0);"
          empty )
  ]
;;

let let_bindings_with_cast =
  [ ( "adding integer and float"
    , `Quick
    , fun _ ->
        check_interp_flt
          "implicit flt cast (addition)"
          30.045
          "let x: flt = 10 + 20.045;"
          empty )
  ; ( "subtracting integer and float"
    , `Quick
    , fun _ ->
        check_interp_flt
          "implicit flt cast (subtraction)"
          7.9
          "let y: flt = 10 - 2.1;"
          empty )
  ; ( "multiplying integer and float"
    , `Quick
    , fun _ ->
        check_interp_flt
          "implicit flt cast (multiplication)"
          21.0
          "let y: flt = 10 * 2.1;"
          empty )
  ; ( "dividing integer and float"
    , `Quick
    , fun _ ->
        check_interp_flt "implicit flt cast (division)" 5.0 "let y: flt = 10 / 2.0;" empty
    )
  ]
;;
