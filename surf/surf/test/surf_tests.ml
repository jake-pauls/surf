open Test_arithmetic
open Test_bindings
open Test_builtins
open Test_strings

(** [suite] main test suite for surf *)
let suite =
  [ "Test_arithmetic.int_basics", int_basics
  ; "Test_arithmetic.flt_basics", flt_basics
  ; "Test_arithmetic.bedmas", bedmas
  ; "Test_bindings.let_bindings", let_bindings
  ; "Test_builtins.put", put
  ; "Test_strings.strings", strings
  ]
;;

let () = Alcotest.run "surf tests" suite
