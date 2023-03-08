{
open Parser

exception SyntaxError of string
}

let white = [' ' '\t']+
let digit = ['0'-'9']

let frac = '.' digit*
let exp = ['e' 'E'] ['-' '+']? digit+
let float = digit* frac? exp?

let newline = '\r' | '\n' | "\r\n" 

let int = digit+

rule read =
  parse
  | white { read lexbuf }
  | newline { read lexbuf }
  | int { INT (int_of_string (Lexing.lexeme lexbuf)) }
  | float { FLOAT (float_of_string (Lexing.lexeme lexbuf)) }
  | "+" { PLUS }
  | "-" { MINUS }
  | "*" { MULT }
  | "/" { DIV }
  | "(" { LPAREN }
  | ")" { RPAREN }
  | "#" { read_comment lexbuf }
  | _ { raise (SyntaxError ("Illegal character: " ^ Lexing.lexeme lexbuf))}
  | eof { EOF }

and read_comment =
  parse
  | newline { read lexbuf }
  | eof { EOF }
  | _ { read_comment lexbuf; }
