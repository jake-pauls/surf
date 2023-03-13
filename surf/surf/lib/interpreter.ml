open Ast
open Env
open Errors
open Utils

(** [is_value e] checks whether [e] is a value *)
let is_value : expr -> bool = function
  | Int _ | Float _ | String _ -> true
  (* TODO: Vectors shouldn't be values explicitly (they can be stepped if they have
     variables inside them at declaration time) *)
  | Vec2 _ | Vec3 _ | Vec4 _ -> true
  | Var _ | Let _ | Binop _ | Unop _ -> false
  | Put _ -> false
;;

(** [vec2_string_of_val e1 e2] retrieves the string representation of a vec2 with value expressions *)
let vec2_string_of_val e1 e2 =
  match e1, e2 with
  | Int i1, Int i2 -> Fmt.str "(%d, %d)" i1 i2
  | Float f1, Float f2 -> Fmt.str "(%f, %f)" f1 f2
  | _ -> raise (RuntimeError err_vec_type_mismatch)
;;

(** [vec3_string_of_val e1 e2 e3] retrieves the string representation of a vec3 with value expressions *)
let vec3_string_of_val e1 e2 e3 =
  match e1, e2, e3 with
  | Int i1, Int i2, Int i3 -> Fmt.str "(%d, %d, %d)" i1 i2 i3
  | Float f1, Float f2, Float f3 -> Fmt.str "(%f, %f, %f)" f1 f2 f3
  | _ -> raise (RuntimeError err_vec_type_mismatch)
;;

(** [vec4_string_of_val e1 e2 e3] retrieves the string representation of a vec4 with value expressions *)
let vec4_string_of_val e1 e2 e3 e4 =
  match e1, e2, e3, e4 with
  | Int i1, Int i2, Int i3, Int i4 -> Fmt.str "(%d, %d, %d, %d)" i1 i2 i3 i4
  | Float f1, Float f2, Float f3, Float f4 -> Fmt.str "(%f, %f, %f, %f)" f1 f2 f3 f4
  | _ -> raise (RuntimeError err_vec_type_mismatch)
;;

(** [typed_string_of_val t e] converts [e] to a string, implicitly casts it to type [t] if
    casting is possible *)
let typed_string_of_val (t : stype) = function
  | Int i when t = STFloat -> string_of_float (float_of_int i)
  | Int i -> string_of_int i
  | Float f when t = STInt -> string_of_int (int_of_float f)
  | Float f -> string_of_float f
  | String s -> s
  (* TODO: Vec's with non-value types will break *)
  | Vec2 (e1, e2) when is_value e1 && is_value e2 -> vec2_string_of_val e1 e2
  | Vec3 (e1, e2, e3) when is_value e1 && is_value e2 && is_value e3 ->
    vec3_string_of_val e1 e2 e3
  | Vec4 (e1, e2, e3, e4) when is_value e1 && is_value e2 && is_value e3 && is_value e4 ->
    vec4_string_of_val e1 e2 e3 e4
  | _ -> raise (TypeError err_no_str_rep)
;;

(** [expr_of_vp (value, typ)] creates an ocaml type from a [value * styp] pair retrieved
    from the StaticEnvironment *)
let expr_of_vp ((expr, styp) : Ast.expr * stype) : expr =
  match styp with
  | STInt | STFloat | STString | STVec2 | STVec3 | STVec4 -> expr
;;

(** [step e] takes a single step of evaluation of [e]. Int: If an 'Int' gets here, it's
    already been computed *)
let rec step env = function
  | Int _ | Float _ | String _ -> raise (RuntimeError (Fmt.str "%s: %s" err_interp_failure "does not step"))
  (* TODO: as mentioned above, vectors should probably step to properly insert variable
     values *)
  | Vec2 _ | Vec3 _ | Vec4 _ -> raise (RuntimeError (Fmt.str "%s: %s" err_interp_failure "vectors cannot step yet"))
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
  (* String (Concatenation) *)
  | `Add, String s1, String s2 -> String (s1 ^ s2)
  (* TODO: Vector math currently fails *)
  | _ -> raise (RuntimeError err_binop_mismatch)
;;

(** [typeof env e] the type of [e] in the environment [env] *)
let rec typeof env = function
  | Int _ -> STInt
  | Float _ -> STFloat
  | String _ -> STString
  | Vec2 _ -> STVec2
  | Vec3 _ -> STVec3
  | Vec4 _ -> STVec4
  | Var x ->
    let _, styp = StaticEnvironment.lookup x env in
    styp
  | Let (x, t, e1) -> typeof_let env x t e1
  | Unop (uop, e1) -> typeof_unop env uop e1
  | Binop (bop, e1, e2) -> typeof_binop env bop e1 e2
  | Put e -> typeof env e

(** [typeof_let env stype t e1] type checking for the expression [e1] in the [env] given a passed type [t], will infer a float or an int despite the expression if [t] specified as such *)
and typeof_let env _ t e1 =
  let t' =
    typeof env e1
    |> function
    | STInt when t = STFloat -> STFloat
    | STInt -> STInt
    | STFloat when t = STInt -> STInt
    | STFloat -> STFloat
    | STString -> STString
    | STVec2 -> STVec2
    | STVec3 -> STVec3
    | STVec4 -> STVec4
  in
  if t != t' then raise (TypeError err_poor_type_annotation) else t'

(** [typeof_unop env uop e1] evaluates the type of [uop] in the [env] against the expression [e1] *)
and typeof_unop env uop e1 =
  match uop, typeof env e1 with
  | `UMinus, STInt -> STInt
  | `UMinus, STFloat -> STFloat
  | `UMinus, STVec2 -> STVec2
  | `UMinus, STVec3 -> STVec3
  | `UMinus, STVec4 -> STVec4
  | `UMinus, STString -> raise (TypeError err_unop_mismatch)

(** [typeof_binop env bop e1 e2] evaluates the type of [bop] in the [env] against both expressions [e1] and [e2] *)
and typeof_binop env bop e1 e2 =
  let e1_t = typeof env e1 in
  let e2_t = typeof env e2 in
  match bop, e1_t, e2_t with
  (* Scalar Binops *)
  | (`Add | `Minus), (STInt | STFloat), (STInt | STFloat) ->
    if e1_t = STFloat || e2_t = STFloat then STFloat else STInt
  | (`Mult | `Div), (STInt | STFloat), (STInt | STFloat) ->
    if e1_t = STFloat || e2_t = STFloat then STFloat else STInt
  (* String Ops *)
  | `Add, STString, STString -> STString
  | (`Minus | `Mult | `Div), STString, STString ->
    raise (TypeError err_str_binop_mismatch)
  (* TODO: Vector math currently fails *)
  | _ -> raise (TypeError err_vec_binop_mismatch)
;;

(** [typecheck] checks the type of [e] in [env], returns it if valid *)
let typecheck (env : 'a StaticEnvironment.t) (e : expr) : stype option =
  try Some (typeof env e) with
  | TypeError msg ->
    print_endline (Fmt.str "[type error ~> %s]" msg)
    ; None
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
       let str = typed_string_of_val t e' in
       (* Put statements should immediately print *)
       (match e with
        | Put _ -> print_endline str
        | _ -> ())
       ; str
     | None -> raise (TypeError err_poor_type_annotation))
  | None -> ""
;;

(** [interp env s] interprets [s] in the [env] - currently used in the interactive repl and when surf is parsing a file *)
let interp env s : unit = ignore (parse_and_ret env s)

(** [interp_ret env s] interprets [s] in [env] and returns it as a string - currently used in the surf test suites *)
let interp_ret env s : string = parse_and_ret env s

(** [c_interp_ret env s] entrypoint for the c-api, requires an [env] pointer to the static
    environment as a basis for recursion, interprets a string [s] and returns any results *)
let c_interp_ret (env : 'a StaticEnvironmentBindings.senv_struct Ctypes.ptr) (s : string)
  : string
  =
  let senv = StaticEnvironmentBindings.from_opaque env in
  parse_and_ret senv s
;;
