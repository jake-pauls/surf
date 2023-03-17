open Ast
open Env
open Errors
open Typechecker

(** [expr_of_vp (value, typ)] creates an ocaml type from a [value * styp] pair retrieved
    from the StaticEnvironment *)
let expr_of_vp ((expr, styp) : Ast.expr * stype) : expr =
  match styp with
  | STInt | STFloat | STString | STVec2 | STVec3 | STVec4 -> expr
;;

(** [step e] takes a single step of evaluation of [e]. Int: If an 'Int' gets here, it's
    already been computed *)
let rec step env exp =
  match exp with
  | Int _ | Float _ | String _ | Reflect _ ->
    raise (RuntimeError (Fmt.str "%s: %s" err_interp_failure "does not step"))
  | Var x -> StaticEnvironment.lookup x env |> expr_of_vp
  | Let (x, t, e) when is_value e ->
    StaticEnvironment.update x (e, t) env
    ; e
  | Let (x, t, e) -> Let (x, t, step env e)
  | Unop (unop, e) when is_value e -> step_unop unop e
  | Unop (unop, e) -> Unop (unop, step env e)
  | Binop (bop, e1, e2) when is_value e1 && is_value e2 -> step_bop bop e1 e2
  | Binop (bop, e1, e2) when is_value e1 -> Binop (bop, e1, step env e2)
  | Binop (bop, e1, e2) -> Binop (bop, step env e1, e2)
  (* Vectors should step values to insert variables *)
  | Vec2 _ when is_value exp -> exp
  | Vec2 (e1, e2) when is_value e1 -> Vec2 (e1, step env e2)
  | Vec2 (e1, e2) -> Vec2 (step env e1, e2)
  | Vec3 _ when is_value exp -> exp
  | Vec3 (e1, e2, e3) when is_value e1 && is_value e2 -> Vec3 (e1, e2, step env e3)
  | Vec3 (e1, e2, e3) when is_value e1 -> Vec3 (e1, step env e2, e3)
  | Vec3 (e1, e2, e3) -> Vec3 (step env e1, e2, e3)
  | Vec4 _ when is_value exp -> exp
  | Vec4 (e1, e2, e3, e4) when is_value e1 && is_value e2 && is_value e3 ->
    Vec4 (e1, e2, e3, step env e4)
  | Vec4 (e1, e2, e3, e4) when is_value e1 && is_value e2 -> Vec4 (e1, e2, step env e3, e4)
  | Vec4 (e1, e2, e3, e4) when is_value e1 -> Vec4 (e1, step env e2, e3, e4)
  | Vec4 (e1, e2, e3, e4) -> Vec4 (step env e1, e2, e3, e4)
  (* Put statements can immediately return their expressions *)
  | Put e when is_value e -> e
  | Put e -> step env e

(** [step_unop uop v] takes a single step to perform a unary operation *)
and step_unop uop v =
  match uop, v with
  | `UMinus, Int a -> Int (-a)
  | `UMinus, Float a -> Float (-.a)
  | `UMinus, String _ -> raise (TypeError err_unop_mismatch)
  | _ -> raise (TypeError err_unop_mismatch)

(** [step_bop bop v1 v2] takes a single step to perform a binary operation *)
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
  (* TODO: Vector math currently fails *)
  | (`Add | `Minus), (Vec2 _ | Vec3 _ | Vec4 _), (Vec2 _ | Vec3 _ | Vec4 _) ->
    raise (RuntimeError (Fmt.str "%s: %s" err_interp_unimplemented "vector arithmetic"))
  (* String (Concatenation) *)
  | `Add, String s1, String s2 -> String (s1 ^ s2)
  | `Add, String s1, (Int _ | Float _ | Vec2 _ | Vec3 _ | Vec4 _) ->
    String (s1 ^ inf_typed_string_of_val v2)
  | `Add, (Int _ | Float _ | Vec2 _ | Vec3 _ | Vec4 _), String s1 ->
    String (inf_typed_string_of_val v1 ^ s1)
  | _ -> raise (RuntimeError err_binop_mismatch)
;;

(** [eval e] fully evaluates [e] to a value [v]. Keeps calling itself and makes 'small
    steps' until it gets down to a value *)
let rec eval env e : expr =
  if is_value e
  then e
  else (
    let stepped_e = step env e in
    eval env stepped_e)
;;

(** [parse_token s] parses a single string expression [s] into an ast *)
let parse_token (s : string) : expr option =
  let lexbuf = Lexing.from_string s in
  try Parser.prog Lexer.read lexbuf with
  | RuntimeError msg -> raise (RuntimeError msg)
  | TypeError msg -> raise (TypeError msg)
  | Lexer.SyntaxError msg -> raise (SyntaxError (msg, lexbuf))
  | Parser.Error -> raise (ParserError lexbuf)
;;

(** [parse_and_ret env s] submits an ast for evaluation in the [env] and returns
    interpretation as string *)
let parse_and_ret env s : string =
  try
    match parse_token s with
    | Some e ->
      (match typecheck env e with
       | Some t ->
         let e' = eval env e in
         let str = typed_string_of_val t e' in
         (* Put statements should immediately print *)
         (match e with
          | Put _ -> print_endline str
          | _ -> ())
         ; str
       | None -> raise (TypeError err_poor_type_annotation))
    | None -> ""
  with
  | RuntimeError msg ->
    print_endline ("do " ^ msg ^ " end")
    ; print_and_ret_err (fmt_runtime_error msg)
  | TypeError msg -> print_and_ret_err (fmt_type_error msg)
  | SyntaxError (msg, lexbuf) ->
    print_and_ret_err (fmt_syntax_error (Utils.print_error_position lexbuf) msg)
  | ParserError lexbuf ->
    print_and_ret_err (fmt_parser_error (Utils.print_error_position lexbuf))
;;

(** [interp env s] interprets [s] in the [env] - currently used in the interactive repl
    and when surf is parsing a file *)
let interp env s : unit = ignore (parse_and_ret env s)

(** [interp_ret env s] interprets [s] in [env] and returns it as a string - currently used
    in the surf test suites *)
let interp_ret env s : string = parse_and_ret env s

(** [c_interp_ret env s] entrypoint for the c-api, requires an [env] pointer to the static
    environment as a basis for recursion, interprets a string [s] and returns any results *)
let c_interp_ret (env : 'a StaticEnvironmentBindings.senv_struct Ctypes.ptr) (s : string)
  : string
  =
  let senv = StaticEnvironmentBindings.from_opaque env in
  parse_and_ret senv s
;;
