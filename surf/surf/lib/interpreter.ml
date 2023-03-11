open Ast
open Env
open Errors
open Lexing

(** [string_of_val e] converts [e] to a string. Requires: [e] is a value. *)
let string_of_val = function
  | Int i -> string_of_int i
  | Float f -> string_of_float f
  | Var _ | Let _ | Binop _ | Unop _ -> failwith "no string representation"
;;

(** [expr_of_vp (value, typ)] creates an ocaml type from a [value * styp] pair retrieved
    from the StaticEnvironment *)
let expr_of_vp ((value, styp) : string * stype) : expr =
  match styp with
  | STInt -> Int (int_of_string value)
  | STFloat -> Float (float_of_string value)
;;

(** [is_value e] checks whether [e] is a value. *)
let is_value : expr -> bool = function
  | Int _ | Float _ -> true
  | Var _ | Let _ | Binop _ | Unop _ -> false
;;

(** [step e] takes a single step of evaluation of [e]. Int: If an 'Int' gets here, it's
    already been computed. *)
let rec step env = function
  | Int _ | Float _ -> failwith "does not step"
  | Var x ->
    let vp = StaticEnvironment.lookup x env in
    expr_of_vp vp
  | Let (x, t, e) when is_value e ->
    let s = string_of_val e in
    StaticEnvironment.update x (s, t) env
    ; e
  | Let (x, t, e) -> Let (x, t, step env e)
  | Unop (unop, e) when is_value e -> step_unop unop e
  | Unop (unop, e) -> Unop (unop, step env e)
  | Binop (bop, e1, e2) when is_value e1 && is_value e2 -> step_bop bop e1 e2
  | Binop (bop, e1, e2) when is_value e1 -> Binop (bop, e1, step env e2)
  | Binop (bop, e1, e2) -> Binop (bop, step env e1, e2)

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

(** [typeof env e] the type of [e] in the environment [env] *)
let rec typeof env = function
  | Int _ -> STInt
  | Float _ -> STFloat
  | Var x ->
    let _, styp = StaticEnvironment.lookup x env in
    styp
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

(** [typecheck] checks the type of [e] in [env], returns it if valid *)
let typecheck env e =
  ignore (typeof env e)
  ; e
;;

(** [eval e] fully evaluates [e] to a value [v]. Keeps calling itself and makes 'small
    steps' until it gets down to a value *)
let rec eval env e : expr = if is_value e then e else step env e |> eval env

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

(** [parse_and_print env s] submits an ast for typing and evaluation in the [env] *)
let parse_and_print env s : unit =
  match parse_token s with
  | Some e ->
    let e' = typecheck env e in
    eval env e' |> string_of_val |> print_endline
  | None -> ()
;;

(** [parse_and_ret env s] submits an ast for evaluation in the [env] and returns
    interpretation as string *)
let parse_and_ret env s : string =
  match parse_token s with
  | Some e ->
    let e' = typecheck env e in
    eval env e' |> string_of_val
  | None -> ""
;;

(** [interp env s] interprets [s] in the [env] *)
let interp env s : unit = parse_and_print env s

(** [interp_ret env s] interprets [s] in [env] and returns it as a string *)
let interp_ret env s : string = parse_and_ret env s
