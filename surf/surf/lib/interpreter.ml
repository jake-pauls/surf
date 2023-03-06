open Ast

(** [parse s] parses [s] into an AST *)
let parse (s : string) : expr =
  let lexbuf = Lexing.from_string s in
  let ast_expr = Parser.prog Lexer.read lexbuf in
  ast_expr

(** [string_of_val e] converts [e] to a string. Requires: [e] is a value. *)
let string_of_val (e : expr) : string =
  match e with
  | Int i -> string_of_int i
  | Unop _ -> failwith "No string representation"
  | Binop _ -> failwith "No string representation"

(** [is_value e] checks whether [e] is a value. *)
let is_value : expr -> bool = function
  | Int _ -> true
  | Unop _ -> false
  | Binop _ -> false

(** [step e] takes a single step of evaluation of [e]. Int: If an 'Int' gets
    here, it's already been computed. *)
let rec step : expr -> expr = function
  | Int _ -> failwith "Does not step"
  | Unop (unop, e) when is_value e -> step_unop unop e
  | Unop (unop, e) -> Unop (unop, step e)
  | Binop (bop, e1, e2) when is_value e1 && is_value e2 -> step_bop bop e1 e2
  | Binop (bop, e1, e2) when is_value e1 -> Binop (bop, e1, step e2)
  | Binop (bop, e1, e2) -> Binop (bop, step e1, e2)

and step_unop unop v =
  match (unop, v) with
  | Minus, Int a -> Int (-a)
  | _ -> failwith "precondition violated"

and step_bop bop v1 v2 =
  match (bop, v1, v2) with
  | Add, Int a, Int b -> Int (a + b)
  | Minus, Int a, Int b -> Int (a - b)
  | Mult, Int a, Int b -> Int (a * b)
  | Div, Int a, Int b -> Int (a / b)
  | _ -> failwith "precondition violated"

(** [eval e] fully evaluates [e] to a value [v]. Keeps calling itself and makes
    'small steps' until it gets down to a value *)
let rec eval (e : expr) : expr = if is_value e then e else e |> step |> eval

(** [interp s] interprets [s] by lexing and parsing it, evaluating it, and
    convering the result to a string. *)
let interp (s : string) : string = s |> parse |> eval |> string_of_val
