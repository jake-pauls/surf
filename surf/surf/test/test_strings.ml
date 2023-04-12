open Test_helpers

let escape_sequences =
  [ ( "new lines in strings '\n'"
    , `Quick
    , fun _ ->
        check_interp_str
          "new lines"
          "surf\nis\nreally\ncool"
          "put(\"surf\nis\nreally\ncool\");"
          empty )
  ; ( "adding slashes in strings '\\'"
    , `Quick
    , fun _ ->
        check_interp_str
          "slashes"
          "this\\is\\a\\slash"
          "put(\"this\\\\is\\\\a\\\\slash\");"
          empty )
  ; ( "tabs in strings '\t'"
    , `Quick
    , fun _ ->
        check_interp_str
          "tabs"
          "surf\tis\treally\tcool"
          "put(\"surf\tis\treally\tcool\");"
          empty )
  ; ( "single quotes in strings '\''"
    , `Quick
    , fun _ ->
        check_interp_str
          "single-quotes"
          "they said: \'surf is really cool\'"
          "put(\"they said: \\\'surf is really cool\\\'\");"
          empty )
  ; ( "double quotes in strings '\"'"
    , `Quick
    , fun _ ->
        check_interp_str
          "double-quotes"
          "they said: \"surf is really cool\""
          "put(\"they said: \\\"surf is really cool\\\"\");"
          empty )
  ]
;;
