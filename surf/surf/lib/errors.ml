(* Type Errors *)

exception TypeError of string

(* Runtime Errors *)

exception RuntimeError of string

(* Error Messages *)

(** [string] error string representing a poor type annotation *)
let err_poor_type_annotation = "poor type annotation"

(** [string] error string representing an unbound variable *)
let err_unbound_var = "unbound variable"

(** [string] error string representing a mismatched unary operator at runtime *)
let err_unop_mismatch = "unary operator mismatch"

(** [string] error string representing a mismatched binary operator at runtime *)
let err_binop_mismatch = "binary operator mismatch"
