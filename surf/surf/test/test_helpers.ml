open Surf
open Errors

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

(** [check_interp_flt name f s env] sets up an alcotest case for the interp function with
    name [name] evaluating a float [f] against the interpreted [s] *)
let check_interp_flt name f s env =
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

(** [check_interp_v2f name v s env] sets up an alcotest case for the interp function with
    name [name] evaluating a float v2 [v] against the interpreted [s] *)
let check_interp_v2f name v s env =
  let float_arr_str = Fmt.str "(%f, %f)" v.(0) v.(1) in
  Alcotest.(check bool)
    name
    true
    (String.equal float_arr_str (Interpreter.interp_ret env s))
;;

(** [check_interp_v2i name v s env] sets up an alcotest case for the interp function with
    name [name] evaluating an integer v2 [v] against the interpreted [s] *)
let check_interp_v2i name v s env =
  let float_arr_str = Fmt.str "(%d, %d)" v.(0) v.(1) in
  Alcotest.(check bool)
    name
    true
    (String.equal float_arr_str (Interpreter.interp_ret env s))
;;

(** [check_interp_v3i name v s env] sets up an alcotest case for the interp function with
    name [name] evaluating an integer v3 [v] against the interpreted [s] *)
let check_interp_v3i name v s env =
  let float_arr_str = Fmt.str "(%d, %d, %d)" v.(0) v.(1) v.(2) in
  Alcotest.(check bool)
    name
    true
    (String.equal float_arr_str (Interpreter.interp_ret env s))
;;

(** [check_interp_v3f name v s env] sets up an alcotest case for the interp function with
    name [name] evaluating a float v3 [v] against the interpreted [s] *)
let check_interp_v3f name v s env =
  let float_arr_str = Fmt.str "(%f, %f, %f)" v.(0) v.(1) v.(2) in
  Alcotest.(check bool)
    name
    true
    (String.equal float_arr_str (Interpreter.interp_ret env s))
;;

(** [check_interp_v4i name v s env] sets up an alcotest case for the interp function with
    name [name] evaluating an integer v4 [v] against the interpreted [s] *)
let check_interp_v4i name v s env =
  let float_arr_str = Fmt.str "(%d, %d, %d, %d)" v.(0) v.(1) v.(2) v.(3) in
  Alcotest.(check bool)
    name
    true
    (String.equal float_arr_str (Interpreter.interp_ret env s))
;;

(** [check_interp_v4f name v s env] sets up an alcotest case for the interp function with
    name [name] evaluating a float v4 [v] against the interpreted [s] *)
let check_interp_v4f name v s env =
  let float_arr_str = Fmt.str "(%f, %f, %f, %f)" v.(0) v.(1) v.(2) v.(3) in
  Alcotest.(check bool)
    name
    true
    (String.equal float_arr_str (Interpreter.interp_ret env s))
;;

(** [check_interp_type_err name err s env] sets up an alcotest case for the interp
    function with name [name] checking for the type error message [err] against the
    interpreted [s] *)
let check_interp_type_error name err s env =
  Alcotest.(check bool)
    name
    true
    (String.equal (fmt_type_error err) (Interpreter.interp_ret env s))
;;

(** [check_interp_runtime_err name err s env] sets up an alcotest case for the interp
    function with name [name] checking for the runtime error message [err] against the
    interpreted [s] *)
let check_interp_runtime_error name err s env =
  let e = fmt_runtime_error err in
  print_endline e
  ; let a = Interpreter.interp_ret env s in
    print_endline a
    ; Alcotest.(check bool)
        name
        true
        (String.equal (fmt_runtime_error err) (Interpreter.interp_ret env s))
;;

(** [check_interp_parser_err name s env] sets up an alcotest case for the interp function
    with name [name] checking for a parser error against the interpreted [s] *)
let check_interp_parser_error name s env =
  Alcotest.(check bool)
    name
    true
    (* fmt_parser_error requires a lexbuf, so just check if the right error is thrown
       instead *)
    (Utils.contains_substr (Interpreter.interp_ret env s) "parser error ~>")
;;

(** [check_interp_syntax_err name s env] sets up an alcotest case for the interp function
    with name [name] checking for a syntax error against the interpreted [s] *)
let check_interp_syntax_error name s env =
  Alcotest.(check bool)
    name
    true
    (* fmt_syntax_error requires a lexbuf, so just check if the right error is thrown
       instead *)
    (Utils.contains_substr (Interpreter.interp_ret env s) "syntax error ~>")
;;
