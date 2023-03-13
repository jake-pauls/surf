let check msg x = Alcotest.(check bool) msg true x
let test_logic () = check "logic" (4 == 4)
let test_reason () = check "reason" (String.equal "surf" "surf")

let suite =
  [ "logic and reason", [ "logic", `Quick, test_logic; "reason", `Quick, test_reason ] ]
;;

let () = Alcotest.run "Default test suite, should pass" suite
