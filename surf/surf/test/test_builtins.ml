open Test_helpers

(** [put] general tests for the 'put' builtin *)
let put =
  [ ("put int", `Quick, fun _ -> check_interp_str "put int" "1" "put(1);" empty)
  ; ("put flt", `Quick, fun _ -> check_interp_str "put float" "1." "put(1.0);" empty)
  ; ( "put flt with decimals"
    , `Quick
    , fun _ -> check_interp_str "put flt with decimals" "1.23" "put(1.23);" empty )
  ; ("put v2i", `Quick, fun _ -> check_interp_str "put v2i" "(1, 2)" "put((1, 2));" empty)
  ; ( "put v2f"
    , `Quick
    , fun _ -> check_interp_str "put v2f" "(1.000000, 2.000000)" "put((1.0, 2.0));" empty
    )
  ; ( "put v3i"
    , `Quick
    , fun _ -> check_interp_str "put v3i" "(1, 2, 3)" "put((1, 2, 3));" empty )
  ; ( "put v3f"
    , `Quick
    , fun _ ->
        check_interp_str
          "put v3f"
          "(1.000000, 2.000000, 3.000000)"
          "put((1.0, 2.0, 3.0));"
          empty )
  ; ( "put v4i"
    , `Quick
    , fun _ -> check_interp_str "put v4i" "(1, 2, 3, 4)" "put((1, 2, 3, 4));" empty )
  ; ( "put v4f"
    , `Quick
    , fun _ ->
        check_interp_str
          "put v4f"
          "(1.000000, 2.000000, 3.000000, 4.000000)"
          "put((1.0, 2.0, 3.0, 4.0));"
          empty )
  ; ( "put string"
    , `Quick
    , fun _ -> check_interp_str "put string" "test" "put(\"test\");" empty )
  ; ( "put concat strings"
    , `Quick
    , fun _ ->
        check_interp_str "put concat strings" "testing" "put(\"test\" + \"ing\");" empty
    )
  ; ( "put concat string with int"
    , `Quick
    , fun _ ->
        check_interp_str "put concat string with int" "int: 1" "put(\"int: \" + 1);" empty
    )
  ; ( "put concat int with string"
    , `Quick
    , fun _ ->
        check_interp_str
          "put concat int with string"
          "1 is an int"
          "put(1 + \" is an int\");"
          empty )
  ; ( "put concat string with flt"
    , `Quick
    , fun _ ->
        check_interp_str
          "put concat string with flt"
          "flt: 1."
          "put(\"flt: \" + 1.0);"
          empty )
  ; ( "put concat flt with string"
    , `Quick
    , fun _ ->
        check_interp_str
          "put concat flt with string"
          "1. is a flt"
          "put(1.0 + \" is a flt\");"
          empty )
  ; ( "put concat string with flt (decimals)"
    , `Quick
    , fun _ ->
        check_interp_str
          "put concat string with flt (decimals)"
          "flt: 1.23"
          "put(\"flt: \" + 1.23);"
          empty )
  ; ( "put concat flt (decimals) with string"
    , `Quick
    , fun _ ->
        check_interp_str
          "put concat flt (decimals) with string"
          "1.23 is a flt"
          "put(1.23 + \" is a flt\");"
          empty )
  ; ( "put concat string with v2"
    , `Quick
    , fun _ ->
        check_interp_str
          "put concat string with v2"
          "v2: (1, 2)"
          "put(\"v2: \" + (1, 2));"
          empty )
  ; ( "put concat v2 with string"
    , `Quick
    , fun _ ->
        check_interp_str
          "put concat v2 with string"
          "(1, 2) is a v2"
          "put((1, 2) + \" is a v2\");"
          empty )
  ; ( "put concat string with v3"
    , `Quick
    , fun _ ->
        check_interp_str
          "put concat string with v3"
          "v3: (1, 2, 3)"
          "put(\"v3: \" + (1, 2, 3));"
          empty )
  ; ( "put concat v3 with string"
    , `Quick
    , fun _ ->
        check_interp_str
          "put concat v3 with string"
          "(1, 2, 3) is a v3"
          "put((1, 2, 3) + \" is a v3\");"
          empty )
  ; ( "put concat string with v4"
    , `Quick
    , fun _ ->
        check_interp_str
          "put concat string with v4"
          "v4: (1, 2, 3, 4)"
          "put(\"v4: \" + (1, 2, 3, 4));"
          empty )
  ; ( "put concat v4 with string"
    , `Quick
    , fun _ ->
        check_interp_str
          "put concat v4 with string"
          "(1, 2, 3, 4) is a v4"
          "put((1, 2, 3, 4) + \" is a v4\");"
          empty )
  ]
;;
