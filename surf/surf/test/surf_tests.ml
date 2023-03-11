open Helpers
open Surf

let empty = Env.StaticEnvironment.empty ()

let int_basics =
  [ ("int", `Quick, fun _ -> check_interp "int" 2 "2" empty)
  ; ("add", `Quick, fun _ -> check_interp "add" 5 "2+3" empty)
  ; ("minus", `Quick, fun _ -> check_interp "minus" 4 "6-2" empty)
  ; ("mult", `Quick, fun _ -> check_interp "mult" 144 "12*12" empty)
  ; ("division", `Quick, fun _ -> check_interp "division" 5 "10 / 2" empty)
  ]
;;

let bedmas =
  [ ("minus on left of add", `Quick, fun _ -> check_interp "minus" 2 "6-2+2" empty)
  ; ("mult of mult", `Quick, fun _ -> check_interp "mult of mult" 24 "1*2*3*4" empty)
  ; ( "mult on left of add"
    , `Quick
    , fun _ -> check_interp "mult on left of add" 22 "2+2*10" empty )
  ; ( "mult on right of add"
    , `Quick
    , fun _ -> check_interp "mult on right of add" 14 "2*2+10" empty )
  ; ( "nested addition"
    , `Quick
    , fun _ -> check_interp "nested addition" 18 "(10 + 5) + (1 + 2)" empty )
  ; ( "division of division"
    , `Quick
    , fun _ -> check_interp "division of division" 5 "40 / 4 / 2" empty )
  ; ( "division on left of add"
    , `Quick
    , fun _ -> check_interp "division on left of add" 15 "40 / 4 + 5" empty )
  ; ( "division on right of add"
    , `Quick
    , fun _ -> check_interp "division on right of add" 15 "5 + 40 / 4" empty )
  ; ( "nested division"
    , `Quick
    , fun _ -> check_interp "nested division" 11 "44 / (2 + 2)" empty )
  ; ( "adding a negative"
    , `Quick
    , fun _ -> check_interp "adding a negative" (-10) "5 + -15" empty )
  ; ( "subtracting a negative"
    , `Quick
    , fun _ -> check_interp "subtracting a negative" 20 "19 - -1" empty )
  ; ( "multiplying a negative"
    , `Quick
    , fun _ -> check_interp "multiplying a negative" (-8) "-4 * 2" empty )
  ; ( "dividing a negative"
    , `Quick
    , fun _ -> check_interp "dividing a negative" (-2) "-4 / 2" empty )
  ]
;;

let suite = [ "basics", int_basics; "bedmas", bedmas ]
let () = Alcotest.run "Default" suite
