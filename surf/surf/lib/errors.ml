(* Type Errors *)

exception TypeError of string

let raise_tc_error e = raise (TypeError e)

(* Runtime Errors *)

exception RuntimeError of string

let raise_rt_error e = raise (RuntimeError e)

(* Error Messages *)

(** [string] error string representing an unbound variable at runtime *)
let err_unbound_var = "unbound variable"

(** [string] error string representing a mismatched unary operator at runtime *)
let err_unop_mismatch = "unary operator mismatch"

(** [string] error string representing a mismatched binary operator at runtime *)
let err_binop_mismatch = "binary operator mismatch"
