open Surf

(** [empty unit] an empty static environment for testing *)
let empty = Env.StaticEnvironment.empty ()

(** [check_interp name i s env] sets up an alcotest case for the interp function with name
    [name] evaluating an integer [i] against the interpreted [s] *)
let check_interp_int name i s env =
  Alcotest.(check bool)
    name
    true
    (String.equal (string_of_int i) (Interpreter.interp_ret env s))
;;

(** [check_interp name f s env] sets up an alcotest case for the interp function with name
    [name] evaluating a float [f] against the interpreted [s] *)
let check_interp_float name f s env =
  Alcotest.(check bool)
    name
    true
    (String.equal (string_of_float f) (Interpreter.interp_ret env s))
;;
