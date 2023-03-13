open Surf

(** [empty unit] an empty static environment for testing *)
let empty : Ast.expr Env.StaticEnvironment.t = Env.StaticEnvironment.empty ()

(** [check_interp_int name i s env] sets up an alcotest case for the interp function with
    name [name] evaluating an integer [i] against the interpreted [s] *)
let check_interp_int name i s env =
  Alcotest.(check bool)
    name
    true
    (String.equal (string_of_int i) (Interpreter.interp_ret env s))
;;

(** [check_interp_float name f s env] sets up an alcotest case for the interp function
    with name [name] evaluating a float [f] against the interpreted [s] *)
let check_interp_float name f s env =
  Alcotest.(check bool)
    name
    true
    (String.equal (string_of_float f) (Interpreter.interp_ret env s))
;;

(** [check_interp_str name str s env] sets up an alcotest case for the interp function
    with name [name] evaluating a string [str] against the interpreted [s] *)
let check_interp_str name str s env =
  Alcotest.(check bool) name true (String.equal str (Interpreter.interp_ret env s))
;;

(** [check_interp_vec2f name v s env] sets up an alcotest case for the interp function
    with name [name] evaluating a float vec2 [v] against the interpreted [s] *)
let check_interp_vec2f name v s env =
  let float_arr_str = Fmt.str "(%f, %f)" v.(0) v.(1) in
  Alcotest.(check bool)
    name
    true
    (String.equal float_arr_str (Interpreter.interp_ret env s))
;;

(** [check_interp_vec2i name v s env] sets up an alcotest case for the interp function
    with name [name] evaluating an integer vec2 [v] against the interpreted [s] *)
let check_interp_vec2i name v s env =
  let float_arr_str = Fmt.str "(%d, %d)" v.(0) v.(1) in
  Alcotest.(check bool)
    name
    true
    (String.equal float_arr_str (Interpreter.interp_ret env s))
;;

(** [check_interp_vec3i name v s env] sets up an alcotest case for the interp function
    with name [name] evaluating an integer vec3 [v] against the interpreted [s] *)
let check_interp_vec3i name v s env =
  let float_arr_str = Fmt.str "(%d, %d, %d)" v.(0) v.(1) v.(2) in
  Alcotest.(check bool)
    name
    true
    (String.equal float_arr_str (Interpreter.interp_ret env s))
;;

(** [check_interp_vec3f name v s env] sets up an alcotest case for the interp function
    with name [name] evaluating a float vec3 [v] against the interpreted [s] *)
let check_interp_vec3f name v s env =
  let float_arr_str = Fmt.str "(%f, %f, %f)" v.(0) v.(1) v.(2) in
  Alcotest.(check bool)
    name
    true
    (String.equal float_arr_str (Interpreter.interp_ret env s))
;;

(** [check_interp_vec4i name v s env] sets up an alcotest case for the interp function
    with name [name] evaluating an integer vec4 [v] against the interpreted [s] *)
let check_interp_vec4i name v s env =
  let float_arr_str = Fmt.str "(%d, %d, %d, %d)" v.(0) v.(1) v.(2) v.(3) in
  Alcotest.(check bool)
    name
    true
    (String.equal float_arr_str (Interpreter.interp_ret env s))
;;

(** [check_interp_vec3f name v s env] sets up an alcotest case for the interp function
    with name [name] evaluating a float vec4 [v] against the interpreted [s] *)
let check_interp_vec4f name v s env =
  let float_arr_str = Fmt.str "(%f, %f, %f, %f)" v.(0) v.(1) v.(2) v.(3) in
  Alcotest.(check bool)
    name
    true
    (String.equal float_arr_str (Interpreter.interp_ret env s))
;;
