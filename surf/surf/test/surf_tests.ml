open Test_arithmetic
open Test_bindings
open Test_builtins
open Test_strings
open Test_errors

(** [suite] main test suite for surf *)
let suite =
  [ "Test_arithmetic.int_basics", int_basics
  ; "Test_arithmetic.flt_basics", flt_basics
  ; "Test_arithmetic.unary_minus_basics", unary_minus_basics
  ; "Test_arithmetic.int_flt_basics", int_flt_basics
  ; "Test_arithmetic.bedmas", bedmas
  ; "Test_bindings.let_bindings", let_bindings
  ; "Test_bindings.let_bindings_with_cast", let_bindings_with_cast
  ; "Test_builtins.put", put
  ; "Test_builtins.spt", spt
  ; "Test_builtins.ref", ref
  ; "Test_strings.escape_sequences", escape_sequences
  ; "Test_errors.type_errors", type_errors
  ; "Test_errors.parser_errors", parser_errors
  ; "Test_errors.syntax_errors", syntax_errors
  ; "Test_errors.runtime_errors", runtime_errors
  ]
;;

let () = Alcotest.run "surf tests" suite
