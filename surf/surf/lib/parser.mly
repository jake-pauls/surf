%{
open Ast
%}

%token <int> INT
%token <float> FLOAT
%token <string> ID
%token <string> STRING
%token PLUS MINUS MULT DIV
%token LPAREN RPAREN
%token COLON
%token SEMICOLON
%token LET
%token EQUALS
%token COMMA
%token STINT STFLOAT STSTRING
%token STVEC2 STVEC3 STVEC4
%token MPUT
%token EOF

%nonassoc UMINUS
%left MINUS
%left PLUS 
%left MULT 
%left DIV

%start <Ast.expr option> prog

%%

prog:
    | EOF { None }
    | e = expr; EOF { Some e }
    ;

expr:
    | LPAREN; e = expr; RPAREN { e }
    | i = INT { Int i }
    | f = FLOAT { Float f }
    | s = STRING { String s }
    | x = ID { Var x }

    /* Expression Types */

    | e1 = expr; PLUS; e2 = expr { Binop (`Add, e1, e2) }
    | e1 = expr; MINUS; e2 = expr { Binop (`Minus, e1, e2) }
    | e1 = expr; MULT; e2 = expr { Binop (`Mult, e1, e2) }
    | e1 = expr; DIV; e2 = expr { Binop (`Div, e1, e2) }
    | MINUS; e = expr; %prec UMINUS { Unop (`UMinus, e) }
    | LPAREN; e1 = expr; COMMA; e2 = expr; RPAREN { Vec2 (e1, e2) }
    | LPAREN; e1 = expr; COMMA; e2 = expr; COMMA; e3 = expr; RPAREN { Vec3 (e1, e2, e3) } 
    | LPAREN; e1 = expr; COMMA; e2 = expr; COMMA; e3 = expr; COMMA; e4 = expr; RPAREN { Vec4 (e1, e2, e3, e4) } 

    /* Assignment Rules */

    | LET; x = ID; COLON; t = stype; EQUALS; e = expr; SEMICOLON { Let (x, t, e) }

    /* Built-ins */

    | MPUT; LPAREN; e = expr; RPAREN; SEMICOLON { Put (e) }
    ;

    /* Static Types */

stype:
    | STINT { STInt } 
    | STFLOAT { STFloat }
    | STSTRING { STString }
    | STVEC2 { STVec2 }
    | STVEC3 { STVec3 }
    | STVEC4 { STVec4 }
    ;