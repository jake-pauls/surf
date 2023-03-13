open Core
open Lexing

(** [print_error_position lexbuf] retrieves the line and position of a lexer error for
    printing *)
let print_error_position (lexbuf : Lexing.lexbuf) =
  let pos = lexbuf.lex_curr_p in
  Fmt.str "line: %d position: %d" pos.pos_lnum (pos.pos_cnum - pos.pos_bol + 1)
;;