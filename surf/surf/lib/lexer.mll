{
open Parser
}

(** allow whitespace *)
let white = [' ' '\t']+

(** digits can be any single number from 0 to 9 *)
let digit = ['0'-'9']

(** in the calculator, integers can be one or more sequential digits (hence the '+') *)
let int = digit+

rule read =
    parse
    | white { read lexbuf }
    | "+" { PLUS }
    | "-" { MINUS }
    | "*" { MULT }
    | "/" { DIV }
    | "(" { LPAREN }
    | ")" { RPAREN }
    | int { INT (int_of_string (Lexing.lexeme lexbuf)) }
    | eof { EOF }
