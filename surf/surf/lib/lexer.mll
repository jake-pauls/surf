{
open Parser

exception SyntaxError of string

let line_num = ref 1
}

let white = [' ' '\t']+

let digit = ['0'-'9']
let letter = ['a'-'z' 'A'-'Z']
let id = (letter) (letter | digit | '_')*

let frac = '.' digit*
let exp = ['e' 'E'] ['-' '+']? digit+
let float = digit* frac? exp?

let cr = '\r'
let lf = '\n'
let newline = cr | lf | (cr lf)

let int = digit+

rule read =
  parse
  | white { read lexbuf }
  | newline { read lexbuf }
  | "let" { LET }
  | "int" { STINT }
  | "flt" { STFLOAT }
  | "str" { STSTRING }
  | "v2" { STVEC2 }
  | "v3" { STVEC3 }
  | "v4" { STVEC4 }
  | "put" { MPUT }
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