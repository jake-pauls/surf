open Ast
open Env
open Errors

(** [typeof env e] the type of [e] in the environment [env] *)
let rec typeof env = function
  | Int _ -> STInt
  | Float _ -> STFloat
  | String _ | Reflect _ -> STString
  | Vec2 _ -> STVec2
  | Vec3 _ -> STVec3
  | Vec4 _ -> STVec4
  | Var x ->
    let _, styp = StaticEnvironment.lookup x env in
    styp
  | Let (x, t, e1) -> typeof_let env x t e1
  | Unop (uop, e1) -> typeof_unop env uop e1
  | Binop (bop, e1, e2) -> typeof_binop env bop e1 e2
  | (Put e | Spt e) -> typeof env e

(** [typeof_let env stype t e1] type checking for the expression [e1] in the [env] given a
    passed type [t], will infer a float or an int despite the expression if [t] specified
    as such *)
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

(** [typeof_unop env uop e1] evaluates the type of [uop] in the [env] against the
    expression [e1] *)
and typeof_unop env uop e1 =
  match uop, typeof env e1 with
  | `UMinus, STInt -> STInt
  | `UMinus, STFloat -> STFloat
  | `UMinus, STVec2 -> STVec2
  | `UMinus, STVec3 -> STVec3
  | `UMinus, STVec4 -> STVec4
  | `UMinus, STString -> raise (TypeError err_unop_mismatch)

(** [typeof_binop env bop e1 e2] evaluates the type of [bop] in the [env] against both
    expressions [e1] and [e2] *)
and typeof_binop env bop e1 e2 =
  let e1_t = typeof env e1 in
  let e2_t = typeof env e2 in
  match bop, e1_t, e2_t with
  (* Scalar Binops *)
  | (`Add | `Minus), (STInt | STFloat), (STInt | STFloat) ->
    if e1_t = STFloat || e2_t = STFloat then STFloat else STInt
  | (`Mult | `Div), (STInt | STFloat), (STInt | STFloat) ->
    if e1_t = STFloat || e2_t = STFloat then STFloat else STInt
  (* TODO: Vector math currently fails *)
  | (`Add | `Minus), (STVec2 | STVec3 | STVec4), (STVec2 | STVec3 | STVec4) ->
    raise (TypeError (Fmt.str "%s: %s" err_interp_unimplemented "vector arithmetic"))
  (* String Binops *)
  | `Add, STString, STString -> STString
  | (`Minus | `Mult | `Div), STString, STString ->
    raise (TypeError err_str_binop_mismatch)
  | `Add, STString, (STInt | STFloat | STVec2 | STVec3 | STVec4) -> STString
  | `Add, (STInt | STFloat | STVec2 | STVec3 | STVec4), STString -> STString
  | _ -> raise (TypeError err_vec_binop_mismatch)
;;

(** [typecheck] checks the type of [e] in [env], returns it if valid *)
let typecheck (env : 'a StaticEnvironment.t) (e : expr) : stype option =
  try Some (typeof env e) with
  | TypeError msg ->
    print_endline (Fmt.str "[type error ~> %s]" msg)
    ; None
;;
