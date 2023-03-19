open Test_helpers

let strings =
  [ ( "concatenating strings"
    , `Quick
    , fun _ ->
        check_interp_str
          "string concatenation"
          "surf is really cool"
          "let x: str = \"surf is \" + \"really cool\";"
          empty )
  ; ( "new lines in strings '\n'"
    , `Quick
    , fun _ ->
        check_interp_str
          "new lines"
          "surf\nis\nreally\ncool"
          "let x: str = \"surf\nis\nreally\ncool\";"
          empty )
  ; ( "adding slashes in strings '\\'"
    , `Quick
    , fun _ ->
        check_interp_str
          "slashes"
          "this\\is\\a\\slash"
          "let x: str = \"this\\\\is\\\\a\\\\slash\";"
          empty )
  ; ( "tabs in strings '\t'"
    , `Quick
    , fun _ ->
        check_interp_str
          "tabs"
          "surf\tis\treally\tcool"
          "let x: str = \"surf\tis\treally\tcool\";"
          empty )
  ; ( "single quotes in strings '\''"
    , `Quick
    , fun _ ->
        check_interp_str
          "single-quotes"
          "they said: \'surf is really cool\'"
          "let x: str = \"they said: \\\'surf is really cool\\\'\";"
          empty )
  ; ( "double quotes in strings '\"'"
    , `Quick
    , fun _ ->
        check_interp_str
          "double-quotes"
          "they said: \"surf is really cool\""
          "let x: str = \"they said: \\\"surf is really cool\\\"\";"
          empty )
  ; ( "concatenating strings with int"
    , `Quick
    , fun _ ->
        check_interp_str
          "string concatenation with int"
          "int is 1"
          "let x: str = \"int is \" + 1;"
          empty )
  ; ( "concatenating strings with flt"
    , `Quick
    , fun _ ->
        check_interp_str
          "string concatenation with flt"
          "flt is 1."
          "let x: str = \"flt is \" + 1.0;"
          empty )
  ; ( "concatenating strings with flt (decimals)"
    , `Quick
    , fun _ ->
        check_interp_str
          "string concatenation with flt (decimals)"
          "flt is 1.23"
          "let x: str = \"flt is \" + 1.23;"
          empty )
  ; ( "concatenating strings with v2"
    , `Quick
    , fun _ ->
        check_interp_str
          "string concatenation with v2"
          "v2 is (1,2)"
          "let x: str = \"v2 is \" + (1,2);"
          empty )
  ; ( "concatenating strings with v3"
    , `Quick
    , fun _ ->
        check_interp_str
          "string concatenation with v3"
          "v3 is (1,2,3)"
          "let x: str = \"v3 is \" + (1,2,3);"
          empty )
  ; ( "concatenating strings with v4"
    , `Quick
    , fun _ ->
        check_interp_str
          "string concatenation with v4"
          "v4 is (1,2,3,4)"
          "let x: str = \"v4 is \" + (1,2,3,4);"
          empty )
  ]
;;
