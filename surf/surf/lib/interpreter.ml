open Ast
open Errors
open Lexing

(** [string_of_val e] converts [e] to a string. Requires: [e] is a value. *)
let string_of_val (e : expr) : string =
  match e with
  | Int i -> string_of_int i
  | Float f -> string_of_float f
  | Var _ | Let _ | Binop _ | Unop _ -> failwith "no string representation"
;;

(** [is_value e] checks whether [e] is a value. *)
let is_value : expr -> bool = function
  | Int _ | Float _ -> true
  | Var _ | Let _ | Binop _ | Unop _ -> false
;;

(** [step e] takes a single step of evaluation of [e]. Int: If an 'Int' gets here, it's
    already been computed. *)
let rec step : expr -> expr = function
  | Int _ | Float _ -> failwith "does not step"
  | Var _ -> raise_rt_error err_unbound_var
  | Let (_, _, e) when is_value e -> e
  | Let (x, t, e) -> Let (x, t, step e)
  | Unop (unop, e) when is_value e -> step_unop unop e
  | Unop (unop, e) -> Unop (unop, step e)
  | Binop (bop, e1, e2) when is_value e1 && is_value e2 -> step_bop bop e1 e2
  | Binop (bop, e1, e2) when is_value e1 -> Binop (bop, e1, step e2)
  | Binop (bop, e1, e2) -> Binop (bop, step e1, e2)

and step_unop unop v =
  match unop, v with
  | UMinus, Int a -> Int (-a)
  | UMinus, Float a -> Float (-.a)
  | _ -> raise_rt_error err_unop_mismatch

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
  | _ -> raise_rt_error err_binop_mismatch
;;

(** [lookup_type env e] lookups up the type of [e] in the environment [env] *)
let lookup_type env e =
  match List.assoc_opt e env with
  | Some t -> t
  | None -> raise_tc_error err_unbound_var
;;

(** [typeof env e] the type of [e] in the environment [env] *)
let rec typeof env = function
  | Int _ -> STInt
  | Float _ -> STFloat
  | Var x -> lookup_type env x
  | Let (x, t, e1) -> typeof_let env x t e1
  | Unop (uop, e1) -> typeof_unop env uop e1
  | Binop (bop, e1, e2) -> typeof_binop env bop e1 e2

and typeof_let env _ t e1 =
  let t' = typeof env e1 in
  if t != t' then raise_tc_error err_poor_type_annotation else t'

and typeof_unop env uop e1 =
  match uop, typeof env e1 with
  | UMinus, STInt -> STInt
  | UMinus, STFloat -> STFloat

and typeof_binop env bop e1 e2 =
  match bop, typeof env e1, typeof env e2 with
  | Add, STInt, STInt -> STInt
  | Add, STFloat, STFloat -> STFloat
  | Minus, STInt, STInt -> STInt
  | Minus, STFloat, STFloat -> STFloat
  | Mult, STInt, STInt -> STInt
  | Mult, STFloat, STFloat -> STFloat
  | Div, STInt, STInt -> STInt
  | Div, STFloat, STFloat -> STFloat
  | _ -> raise_tc_error err_binop_mismatch
;;

(** [typecheck] checks the type of [e], returns it if valid *)
let typecheck e =
  ignore (typeof [] e)
  ; e
;;

(** [eval e] fully evaluates [e] to a value [v]. Keeps calling itself and makes 'small
    steps' until it gets down to a value *)
let rec eval (e : expr) : expr = if is_value e then e else e |> step |> eval

(** [print_error_position lexbuf] retrieves the line and position of a lexer error for
    printing *)
let print_error_position lexbuf =
  let pos = lexbuf.lex_curr_p in
  Fmt.str "line: %d position: %d" pos.pos_lnum (pos.pos_cnum - pos.pos_bol + 1)
;;

(** [parse_token s] parses a single string expression [s] into an ast *)
let parse_token (s : string) : expr option =
  let lexbuf = Lexing.from_string s in
  try Parser.prog Lexer.read lexbuf with
  | Lexer.SyntaxError msg ->
    print_endline (Fmt.str "%s: %s@." (print_error_position lexbuf) msg)
    ; None
  | Parser.Error ->
    print_endline (Fmt.str "[syntax error ~> %s]" (print_error_position lexbuf))
    ; None
;;

(** [parse_and_print s] submits an ast for evaluation *)
let parse_and_print (s : string) : unit =
  match parse_token s with
  | Some e -> typecheck e |> eval |> string_of_val |> print_endline
  | None -> ()
;;

(** [parse_and_ret s] submits an ast for evaluation and returns interpretation as string *)
let parse_and_ret (s : string) : string =
  match parse_token s with
  | Some e -> typecheck e |> eval |> string_of_val
  | None -> ""
;;

(** [interp s] interprets [s] by lexing, parsing, and evaluating it *)
let interp (s : string) : unit = s |> parse_and_print

(** [interp_ret] interprets [s] by lexing, parsing, evaluating it, and returning it as a
    string *)
let interp_ret (s : string) : string = s |> parse_and_ret
