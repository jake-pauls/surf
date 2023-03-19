(* Type Errors *)

exception TypeError of string

(* Runtime Errors *)

exception RuntimeError of string
exception SyntaxError of string * Lexing.lexbuf
exception ParserError of Lexing.lexbuf

(* Error Formatting *)

(** [fmt_type_error msg] formats the error [msg] into a type error string *)
let fmt_type_error msg = Fmt.str "[type error ~> %s]" msg

(** [fmt_runtime_error msg] formats the error [msg] into a runtime error string *)
let fmt_runtime_error msg = Fmt.str "[runtime error ~> %s]" msg

(** [fmt_syntax_error msg] formats the error [msg] into a syntax error string *)
let fmt_syntax_error pos msg = Fmt.str "[syntax error ~> %s (%s)]" pos msg

(** [fmt_parser_error msg] formats the error [msg] into a parser error string *)
let fmt_parser_error pos = Fmt.str "[parser error ~> %s]" pos

(** [print_and_ret_err err] prints [err] and returns it *)
let print_and_ret_err err =
  print_endline err
  ; err
;;

(* Error Messages *)

(** [err_interp_unimplemented] error string representing an unimplemented operation in the
    interpreter*)
let err_interp_unimplemented = "unimplemented operation (this is probably a todo)"

(** [err_interp_failure] error string representing an interpreter failure, if these occur
    something wrong probably happened. *)
let err_interp_failure = "interpreter failure (this is probably a bug)"

(** [err_poor_type_annotation] error string representing a poor type annotation *)
let err_poor_type_annotation = "poor type annotation"

(** [err_unbound_var] error string representing an unbound variable *)
let err_unbound_var = "unbound variable"

(** [err_unop_mismatch] error string representing a mismatched unary operator at runtime *)
let err_unop_mismatch = "unary operator mismatch"

(** [err_binop_mismatch] error string representing a mismatched binary operator at runtime *)
let err_binop_mismatch = "binary operator mismatch"

(** [err_vec_type_mismatch] error string representing a vector initialized with
    inconsistent number types *)
let err_vec_type_mismatch =
  "vectors should be initialized with consistent types (int, int, int) or (float, float, \
   float)"
;;

(** [err_vec_binop_mismatch] error string representing an invalid binary operation being
    performed on a vector *)
let err_vec_binop_mismatch = "attempting to perform an invalid vector operation"

(** [err_str_binop_mismatch] error string representing an invalid binary operation being
    performed on a string *)
let err_str_binop_mismatch = "attempting to perform invalid string operation"

(** [err_no_str_rep] error string representing the retrieval of an expression without a
    valid string representation *)
let err_no_str_rep = "no string representation"
