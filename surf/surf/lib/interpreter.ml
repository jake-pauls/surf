open Ast
open Lexing

(** [string_of_val e] converts [e] to a string. Requires: [e] is a value. *)
let string_of_val (e : expr) : string =
  match e with
  | Int i -> string_of_int i
  | Float f -> string_of_float f
  | Unop _ -> failwith "No string representation"
  | Binop _ -> failwith "No string representation"
;;

(** [is_value e] checks whether [e] is a value. *)
let is_value : expr -> bool = function
  | Int _ -> true
  | Float _ -> true
  | Unop _ -> false
  | Binop _ -> false
;;

(** [step e] takes a single step of evaluation of [e]. Int: If an 'Int' gets here, it's
    already been computed. *)
let rec step : expr -> expr = function
  | Int _ -> failwith "Does not step"
  | Float _ -> failwith "Does not step"
  | Unop (unop, e) when is_value e -> step_unop unop e
  | Unop (unop, e) -> Unop (unop, step e)
  | Binop (bop, e1, e2) when is_value e1 && is_value e2 -> step_bop bop e1 e2
  | Binop (bop, e1, e2) when is_value e1 -> Binop (bop, e1, step e2)
  | Binop (bop, e1, e2) -> Binop (bop, step e1, e2)

and step_unop unop v =
  match unop, v with
  | Minus, Int a -> Int (-a)
  | Minus, Float a -> Float (-.a)
  | _ -> failwith "precondition violated"

and step_bop bop v1 v2 =
  match bop, v1, v2 with
  | Add, Int a, Int b -> Int (a + b)
  | Add, Float a, Float b -> Float (a +. b)
  | Minus, Int a, Int b -> Int (a - b)
  | Minus, Float a, Float b -> Float (a -. b)
  | Mult, Int a, Int b -> Int (a * b)
  | Mult, Float a, Float b -> Float (a *. b)
  | Div, Int a, Int b -> Int (a / b)
  | Div, Float a, Float b -> Float (a /. b)
  | _ -> failwith "precondition violated"
;;

(** [eval e] fully evaluates [e] to a value [v]. Keeps calling itself and makes 'small
    steps' until it gets down to a value *)
let rec eval (e : expr) : expr = if is_value e then e else e |> step |> eval

(** [print_error_position lexbuf] retrieves the line and position of a lexer error for
    printing *)
let print_error_position lexbuf =
  let pos = lexbuf.lex_curr_p in
  Fmt.str "Line: %d Position: %d" pos.pos_lnum (pos.pos_cnum - pos.pos_bol + 1)
;;

(** [parse_token s] parses a single string expression [s] into an ast *)
let parse_token (s : string) : expr option =
  let lexbuf = Lexing.from_string s in
  try Parser.prog Lexer.read lexbuf with
  | Lexer.SyntaxError msg ->
    print_endline (Fmt.str "%s: %s@." (print_error_position lexbuf) msg)
    ; None
  | Parser.Error ->
    print_endline (Fmt.str "[Syntax Error ~> %s]" (print_error_position lexbuf))
    ; None
;;

(** [parse_and_print s] submits an ast for evaluation *)
let parse_and_print (s : string) : unit =
  match parse_token s with
  | Some value ->
    let e = eval value in
    print_endline (string_of_val e)
  | None -> ()
;;

(** [parse_and_ret s] submits an ast for evaluation and returns interpretation as string *)
let parse_and_ret (s : string) : string =
  match parse_token s with
  | Some value -> eval value |> string_of_val
  | None -> ""
;;

(** [interp s] interprets [s] by lexing, parsing, and evaluating it *)
let interp (s : string) : unit = s |> parse_and_print

(** [interp_ret] interprets [s] by lexing, parsing, evaluating it, and returning it as a
    string *)
let interp_ret (s : string) : string = s |> parse_and_ret
