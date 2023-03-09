{
open Parser

exception SyntaxError of string
}

let white = [' ' '\t']+
let digit = ['0'-'9']
let letter = ['a'-'z' 'A'-'Z']
let id = letter+

let frac = '.' digit*
let exp = ['e' 'E'] ['-' '+']? digit+
let float = digit* frac? exp?

let newline = '\r' | '\n' | "\r\n" 

let int = digit+

rule read =
  parse
  | white { read lexbuf }
  | newline { read lexbuf }
  | "let" { LET }
  | "int" { STINT }
  | "float" { STFLOAT }
  | int { INT (int_of_string (Lexing.lexeme lexbuf)) }
  | float { FLOAT (float_of_string (Lexing.lexeme lexbuf)) }
  | id { ID (Lexing.lexeme lexbuf) }
  | "+" { PLUS }
  | "-" { MINUS }
  | "*" { MULT }
  | "/" { DIV }
  | "(" { LPAREN }
  | ")" { RPAREN }
  | "=" { EQUALS }
  | ":" { COLON }
  | ";" { SEMICOLON }
  | "#" { read_comment lexbuf }
  | _ { raise (SyntaxError ("Illegal character: " ^ Lexing.lexeme lexbuf))}
  | eof { EOF }

and read_comment =
  parse
  | newline { read lexbuf }
  | eof { EOF }
  | _ { read_comment lexbuf; }
