%{
open Ast
%}

%token <int> INT
%token <float> FLOAT
%token <string> ID
%token PLUS MINUS MULT DIV
%token LPAREN RPAREN
%token COLON
%token SEMICOLON
%token LET
%token EQUALS
%token STINT STFLOAT
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
    | x = ID { Var (x) }

    /* Expression Types */

    | e1 = expr; PLUS; e2 = expr { Binop (Add, e1, e2) }
    | e1 = expr; MINUS; e2 = expr { Binop (Minus, e1, e2) }
    | e1 = expr; MULT; e2 = expr { Binop (Mult, e1, e2) }
    | e1 = expr; DIV; e2 = expr { Binop (Div, e1, e2) }
    | MINUS; e = expr; %prec UMINUS { Unop (UMinus, e) }

    /* Assignment Rules */

    | LET; x = ID; COLON; t = stype; EQUALS; e = expr; SEMICOLON { Let (x, t, e) }
    ;

    /* Static Types */

stype:
    | STINT { STInt } 
    | STFLOAT { STFloat }
    ;