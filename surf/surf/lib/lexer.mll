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
  | "str" { STSTRING }
  | "vec2" { STVEC2 }
  | "vec3" { STVEC3 }
  | "vec4" { STVEC4 }
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
  | "," { COMMA }
  | "#" { read_comment lexbuf }
  | '"' { read_string (Buffer.create 17) lexbuf }
  | _ { raise (SyntaxError ("illegal character: " ^ "\"" ^ Lexing.lexeme lexbuf ^ "\""))}
  | eof { EOF }

and read_comment =
  parse
  | newline { read lexbuf }
  | eof { EOF }
  | _ { read_comment lexbuf; }

and read_string buf = parse
  | '"'       { STRING (Buffer.contents buf) }
  | '\\' '\\' { Buffer.add_char buf '\\'; read_string buf lexbuf }
  | '\\' 'n'  { Buffer.add_char buf '\n'; read_string buf lexbuf }
  | '\\' 't'  { Buffer.add_char buf '\t'; read_string buf lexbuf }
  | '\\' '''  { Buffer.add_char buf '\''; read_string buf lexbuf }
  | '\\' '"'  { Buffer.add_char buf '\"'; read_string buf lexbuf }
  | [^ '"' '\\']+
    { Buffer.add_string buf (Lexing.lexeme lexbuf);
      read_string buf lexbuf
    }
  | _ { raise (SyntaxError ("illegal string character " ^ "\"" ^ Lexing.lexeme lexbuf ^ "\"")) }
  | eof { raise (SyntaxError ("non-terminating string")) }