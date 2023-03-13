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

(** [string] error string representing a vector initialized with inconsistent number types *)
let err_vec_type_mismatch = "vectors should be initialized with consistent types (int, int, int) or (float, float, float)"

(** [string] error string representing an invalid binary operation being performed on a vector *)
let err_vec_binop_mismatch = "attempting to perform an invalid binary operation on a vector"