open Helpers

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
  ]
;;
