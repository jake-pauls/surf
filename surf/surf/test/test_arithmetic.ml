open Test_helpers

(** [int_basics] default integer operations *)
let int_basics =
  [ ( "integer evaluation"
    , `Quick
    , fun _ -> check_interp_int "integer evaluation" 2 "2" empty )
  ; ( "integer addition"
    , `Quick
    , fun _ -> check_interp_int "integer addition" 5 "2+3" empty )
  ; ( "integer subtraction"
    , `Quick
    , fun _ -> check_interp_int "integer subtraction" 4 "6-2" empty )
  ; ( "integer multiplication"
    , `Quick
    , fun _ -> check_interp_int "integer multiplication" 144 "12*12" empty )
  ; ( "integer division"
    , `Quick
    , fun _ -> check_interp_int "integer division" 5 "10 / 2" empty )
  ]
;;

(** [flt_basics] default flt operations *)
let flt_basics =
  [ ( "float evaluation"
    , `Quick
    , fun _ -> check_interp_flt "float evaluation" 2.0 "2.0" empty )
  ; ( "float evaluation with decimals"
    , `Quick
    , fun _ -> check_interp_flt "float evaluation with decimals" 2.234785 "2.234785" empty
    )
  ; ( "float addition"
    , `Quick
    , fun _ -> check_interp_flt "float addition" 5.4 "2.2+3.2" empty )
  ; ( "float subtraction"
    , `Quick
    , fun _ -> check_interp_flt "float subtraction" 4.0 "6.1-2.1" empty )
  ; ( "float multiplication"
    , `Quick
    , fun _ -> check_interp_flt "float multiplication" 125.44 "12.25*10.24" empty )
  ; ( "float division"
    , `Quick
    , fun _ -> check_interp_flt "float division" 5.0 "10.0 / 2.0" empty )
  ]
;;

let unary_minus_basics =
  [ ( "adding a negative number"
    , `Quick
    , fun _ -> check_interp_int "adding a negative number" (-10) "5 + -15" empty )
  ; ( "subtracting a negative number"
    , `Quick
    , fun _ -> check_interp_int "subtracting a negative number" 20 "19 - -1" empty )
  ; ( "multiplying a negative number"
    , `Quick
    , fun _ -> check_interp_int "multiplying a negative number" (-8) "-4 * 2" empty )
  ; ( "dividing a negative number"
    , `Quick
    , fun _ -> check_interp_int "dividing a negative number" (-2) "-4 / 2" empty )
  ]
;;

(** [int_flt_basics] asserts the combination of int and flt types *)
let int_flt_basics =
  [ ( "float addition with integer"
    , `Quick
    , fun _ -> check_interp_flt "float addition with integer" 12.3 "2.3 + 10" empty )
  ; ( "float subtraction with integer"
    , `Quick
    , fun _ -> check_interp_flt "float subtraction with integer" 10.5 "12.5 - 2" empty )
  ; ( "float multiplication with integer"
    , `Quick
    , fun _ -> check_interp_flt "float multiplication with integer" 11.5 "2.3 * 5" empty
    )
  ; ( "float division with integer"
    , `Quick
    , fun _ -> check_interp_flt "float division with integer" 10.1 "20.2 / 2" empty )
  ; ( "integer addition with float"
    , `Quick
    , fun _ -> check_interp_flt "integer addition with float" 12.3 "10 + 2.3" empty )
  ; ( "integer subtraction with float"
    , `Quick
    , fun _ -> check_interp_flt "integer subtraction with float" (-10.5) "2 - 12.5" empty
    )
  ; ( "integer multiplication with float"
    , `Quick
    , fun _ -> check_interp_flt "integer multiplication with float" 11.5 "5 * 2.3" empty
    )
  ; ( "integer division with float"
    , `Quick
    , fun _ -> check_interp_flt "integer division with float" 10.0 "20 / 2.0" empty )
  ]
;;

(** [bedmas] testing order of operations and operand associativity with integers, should
    also be valid for flts *)
let bedmas =
  [ ( "minus on left of addition"
    , `Quick
    , fun _ -> check_interp_int "minus on left of addition" 2 "6-2+2" empty )
  ; ( "mult on left of addition"
    , `Quick
    , fun _ -> check_interp_int "mult on left of addition" 22 "2+2*10" empty )
  ; ( "mult on right of addition"
    , `Quick
    , fun _ -> check_interp_int "mult on right of addition" 14 "2*2+10" empty )
  ; ( "nested addition with parentheses"
    , `Quick
    , fun _ ->
        check_interp_int "nested addition with parentheses" 18 "(10 + 5) + (1 + 2)" empty
    )
  ; ( "division on left of add"
    , `Quick
    , fun _ -> check_interp_int "division on left of add" 15 "40 / 4 + 5" empty )
  ; ( "division on right of add"
    , `Quick
    , fun _ -> check_interp_int "division on right of add" 15 "5 + 40 / 4" empty )
  ; ( "nested division with parentheses"
    , `Quick
    , fun _ -> check_interp_int "nested division with parentheses" 11 "44 / (2 + 2)" empty
    )
  ]
;;
