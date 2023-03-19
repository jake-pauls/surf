exception TypeError of string
exception RuntimeError of string
exception SyntaxError of string * Lexing.lexbuf
exception ParserError of Lexing.lexbuf

val print_and_ret_err : string -> string
val fmt_type_error : string -> string
val fmt_runtime_error : string -> string
val fmt_syntax_error : string -> string -> string
val fmt_parser_error : string -> string
val err_interp_unimplemented : string
val err_interp_failure : string
val err_poor_type_annotation : string
val err_unbound_var : string
val err_unop_mismatch : string
val err_binop_mismatch : string
val err_vec_type_mismatch : string
val err_vec_binop_mismatch : string
val err_str_binop_mismatch : string
val err_no_str_rep : string
