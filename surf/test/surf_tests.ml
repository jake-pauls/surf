let test_lowercase () =
  Alcotest.(check string) "lowercase" "surf" (String.lowercase_ascii "SURF")

let () =
  Alcotest.run "surf"
    [ ("tbd", [ Alcotest.test_case "Lower case" `Quick test_lowercase ]) ]
