open Test_arithmetic
open Test_bindings

(** [suite] main test suite for surf *)
let suite =
  [ "Test_arithmetic.int_basics", int_basics
  ; "Test_arithmetic.float_basics", float_basics
  ; "Test_arithmetic.bedmas", bedmas
  ; "Test_bindings.let_bindings", let_bindings
  ]
;;

let () = Alcotest.run "surf tests" suite
