open Test_helpers

(** [int_basics] default integer operations *)
let int_basics =
  [ ("int", `Quick, fun _ -> check_interp_int "int" 2 "2" empty)
  ; ("add", `Quick, fun _ -> check_interp_int "add" 5 "2+3" empty)
  ; ("minus", `Quick, fun _ -> check_interp_int "minus" 4 "6-2" empty)
  ; ("mult", `Quick, fun _ -> check_interp_int "mult" 144 "12*12" empty)
  ; ("division", `Quick, fun _ -> check_interp_int "division" 5 "10 / 2" empty)
  ]
;;

(** [flt_basics] default flt operations *)
let flt_basics =
  [ ("flt", `Quick, fun _ -> check_interp_flt "flt" 2.0 "2.0" empty)
  ; ("add", `Quick, fun _ -> check_interp_flt "add" 5.4 "2.2+3.2" empty)
  ; ("minus", `Quick, fun _ -> check_interp_flt "minus" 4.0 "6.1-2.1" empty)
  ; ("mult", `Quick, fun _ -> check_interp_flt "mult" 125.44 "12.25*10.24" empty)
  ; ("division", `Quick, fun _ -> check_interp_flt "division" 5.0 "10.0 / 2.0" empty)
  ]
;;

(** [bedmas] testing order of operations and operand associativity with integers, should
    also be valid for flts *)
let bedmas =
  [ ("minus on left of add", `Quick, fun _ -> check_interp_int "minus" 2 "6-2+2" empty)
  ; ("mult of mult", `Quick, fun _ -> check_interp_int "mult of mult" 24 "1*2*3*4" empty)
  ; ( "mult on left of add"
    , `Quick
    , fun _ -> check_interp_int "mult on left of add" 22 "2+2*10" empty )
  ; ( "mult on right of add"
    , `Quick
    , fun _ -> check_interp_int "mult on right of add" 14 "2*2+10" empty )
  ; ( "nested addition"
    , `Quick
    , fun _ -> check_interp_int "nested addition" 18 "(10 + 5) + (1 + 2)" empty )
  ; ( "division of division"
    , `Quick
    , fun _ -> check_interp_int "division of division" 5 "40 / 4 / 2" empty )
  ; ( "division on left of add"
    , `Quick
    , fun _ -> check_interp_int "division on left of add" 15 "40 / 4 + 5" empty )
  ; ( "division on right of add"
    , `Quick
    , fun _ -> check_interp_int "division on right of add" 15 "5 + 40 / 4" empty )
  ; ( "nested division"
    , `Quick
    , fun _ -> check_interp_int "nested division" 11 "44 / (2 + 2)" empty )
  ; ( "adding a negative"
    , `Quick
    , fun _ -> check_interp_int "adding a negative" (-10) "5 + -15" empty )
  ; ( "subtracting a negative"
    , `Quick
    , fun _ -> check_interp_int "subtracting a negative" 20 "19 - -1" empty )
  ; ( "multiplying a negative"
    , `Quick
    , fun _ -> check_interp_int "multiplying a negative" (-8) "-4 * 2" empty )
  ; ( "dividing a negative"
    , `Quick
    , fun _ -> check_interp_int "dividing a negative" (-2) "-4 / 2" empty )
  ]
;;
