open Core
open Lexing

(** [print_error_position lexbuf] retrieves the line and position of a lexer error for
    printing *)
let print_error_position (lexbuf : Lexing.lexbuf) =
  let pos = lexbuf.lex_curr_p in
  Fmt.str "line: %d position: %d" pos.pos_lnum (pos.pos_cnum - pos.pos_bol + 1)
;;

(** [contains_subtr s1 s2] checks if [s2] contains [s1] as a substring *)
let contains_substr s1 s2 =
  let re = Str.regexp_string s2 in
  try
    ignore (Str.search_forward re s1 0)
    ; true
  with
  | Not_found_s _ -> false
;;
