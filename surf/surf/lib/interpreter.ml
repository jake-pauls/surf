open Ast
open Env
open Errors
open Lexing

(** [string_of_val t e] converts [e] to a string, implicitly casts it to type [t] if
    casting is possible. *)
let string_of_val (t : stype) = function
  | Int i when t = STFloat -> string_of_float (float_of_int i)
  | Int i -> string_of_int i
  | Float f when t = STInt -> string_of_int (int_of_float f)
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
    let s = string_of_val t e in
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
  | `UMinus, Int a -> Int (-a)
  | `UMinus, Float a -> Float (-.a)
  | _ -> raise (TypeError err_unop_mismatch)

and step_bop bop v1 v2 =
  match bop, v1, v2 with
  (* Add *)
  | `Add, Int i1, Int i2 -> Int (i1 + i2)
  | `Add, Float f1, Float f2 -> Float (f1 +. f2)
  | `Add, Int i, Float f | `Add, Float f, Int i -> Float (float_of_int i +. f)
  (* Minus *)
  | `Minus, Int i1, Int i2 -> Int (i1 - i2)
  | `Minus, Float f1, Float f2 -> Float (f1 -. f2)
  | `Minus, Int i, Float f -> Float (float_of_int i -. f)
  | `Minus, Float f, Int i -> Float (f -. float_of_int i)
  (* Mult *)
  | `Mult, Int i1, Int i2 -> Int (i1 * i2)
  | `Mult, Float f1, Float f2 -> Float (f1 *. f2)
  | `Mult, Int i, Float f | `Mult, Float f, Int i -> Float (float_of_int i *. f)
  (* Div *)
  | `Div, Int i1, Int i2 -> Int (i1 / i2)
  | `Div, Float f1, Float f2 -> Float (f1 /. f2)
  | `Div, Int i, Float f -> Float (float_of_int i /. f)
  | `Div, Float f, Int i -> Float (f /. float_of_int i)
  | _ -> raise (RuntimeError err_binop_mismatch)
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
  let t' =
    typeof env e1
    |> function
    | STInt when t = STFloat -> STFloat
    | STInt -> STInt
    | STFloat when t = STInt -> STInt
    | STFloat -> STFloat
  in
  if t != t' then raise (TypeError err_poor_type_annotation) else t'

and typeof_unop env uop e1 =
  match uop, typeof env e1 with
  | `UMinus, STInt -> STInt
  | `UMinus, STFloat -> STFloat

and typeof_binop env (bop : binop) e1 e2 =
  let e1_t = typeof env e1 in
  let e2_t = typeof env e2 in
  match bop, e1_t, e2_t with
  | (`Add | `Minus), (STInt | STFloat), (STInt | STFloat) ->
    if e1_t = STFloat || e2_t = STFloat then STFloat else STInt
  | (`Mult | `Div), (STInt | STFloat), (STInt | STFloat) ->
    if e1_t = STFloat || e2_t = STFloat then STFloat else STInt
;;

(** [typecheck] checks the type of [e] in [env], returns it if valid *)
let typecheck (env : StaticEnvironment.t) (e : expr) : stype option =
  try Some (typeof env e) with
  | TypeError msg ->
    print_endline (Fmt.str "[type error ~> %s]" msg)
    ; None
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
    print_endline (Fmt.str "[lexer error ~> %s (%s)]" (print_error_position lexbuf) msg)
    ; None
  | Parser.Error ->
    print_endline (Fmt.str "[syntax error ~> %s]" (print_error_position lexbuf))
    ; None
;;

(** [parse_and_ret env s] submits an ast for evaluation in the [env] and returns
    interpretation as string *)
let parse_and_ret env s : string =
  match parse_token s with
  | Some e ->
    (match typecheck env e with
     | Some t ->
       let e' = eval env e in
       string_of_val t e'
     | None -> raise (TypeError err_poor_type_annotation))
  | None -> ""
;;

(** [parse_and_print env s] submits an ast for typing and evaluation in the [env] *)
let parse_and_print env s : unit = parse_and_ret env s |> print_endline

(** [interp env s] interprets [s] in the [env] *)
let interp env s : unit = parse_and_print env s

(** [interp_ret env s] interprets [s] in [env] and returns it as a string *)
let interp_ret env s : string = parse_and_ret env s

(** [c_interp_ret env s] entrypoint for the c-api, requires an [env] pointer to the static
    environment as a basis for recursion, interprets a string [s] and returns any results *)
let c_interp_ret (env : StaticEnvironmentBindings.senv_struct Ctypes.ptr) (s : string)
  : string
  =
  let senv = StaticEnvironmentBindings.from_opaque env in
  parse_and_ret senv s
;;
