open Surf

let check msg x = Alcotest.(check bool) msg true x
let test_logic () = check "logic" (Base.four == 4)
let test_reason () = check "reason" (String.equal Base.name "surf")

let def_suite =
  [
    ( "logic and reason",
      [ ("logic", `Quick, test_logic); ("reason", `Quick, test_reason) ] );
  ]

let () = Alcotest.run "Default test suite, should pass" def_suite
