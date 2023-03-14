open Errors

(** [binop] binary operations *)
type binop =
  [ `Add
  | `Minus
  | `Mult
  | `Div
  ]

(** [unop] unary operations *)
type unop = [ `UMinus ]

(** [stype] static types *)
type stype =
  | STInt
  | STFloat
  | STString
  | STVec2
  | STVec3
  | STVec4

(** [expr] expression types *)
type expr =
  | Int of int
  | Float of float
  | String of string
  | Var of string
  | Let of string * stype * expr
  (* Operators *)
  | Binop of binop * expr * expr
  | Unop of unop * expr
  (* Vectors *)
  | Vec2 of expr * expr
  | Vec3 of expr * expr * expr
  | Vec4 of expr * expr * expr * expr
  (* Built-ins *)
  | Put of expr

(** [is_value e] checks whether [e] is a value, vectors containing all value types are
    considered values *)
let rec is_value : expr -> bool = function
  | Int _ | Float _ | String _ -> true
  | Vec2 (e1, e2) when is_value e1 && is_value e2 -> true
  | Vec3 (e1, e2, e3) when is_value e1 && is_value e2 && is_value e3 -> true
  | Vec4 (e1, e2, e3, e4) when is_value e1 && is_value e2 && is_value e3 && is_value e4 ->
    true
  | Var _ | Let _ | Binop _ | Unop _ | Vec2 _ | Vec3 _ | Vec4 _ -> false
  | Put _ -> false
;;

(** [vec2_string_of_val e1 e2] retrieves the string representation of a vec2 with value
    expressions *)
let vec2_string_of_val e1 e2 =
  match e1, e2 with
  | Int i1, Int i2 -> Fmt.str "(%d, %d)" i1 i2
  | Float f1, Float f2 -> Fmt.str "(%f, %f)" f1 f2
  | _ -> raise (RuntimeError err_vec_type_mismatch)
;;

(** [vec3_string_of_val e1 e2 e3] retrieves the string representation of a vec3 with value
    expressions *)
let vec3_string_of_val e1 e2 e3 =
  match e1, e2, e3 with
  | Int i1, Int i2, Int i3 -> Fmt.str "(%d, %d, %d)" i1 i2 i3
  | Float f1, Float f2, Float f3 -> Fmt.str "(%f, %f, %f)" f1 f2 f3
  | _ -> raise (RuntimeError err_vec_type_mismatch)
;;

(** [vec4_string_of_val e1 e2 e3] retrieves the string representation of a vec4 with value
    expressions *)
let vec4_string_of_val e1 e2 e3 e4 =
  match e1, e2, e3, e4 with
  | Int i1, Int i2, Int i3, Int i4 -> Fmt.str "(%d, %d, %d, %d)" i1 i2 i3 i4
  | Float f1, Float f2, Float f3, Float f4 -> Fmt.str "(%f, %f, %f, %f)" f1 f2 f3 f4
  | _ -> raise (RuntimeError err_vec_type_mismatch)
;;

(** [typed_string_of_val t e] converts [e] to a string, implicitly casts it to type [t] if
    casting is possible *)
let typed_string_of_val (t : stype) (e : expr) =
  match e with
  | Int i when t = STFloat -> string_of_float (float_of_int i)
  | Int i -> string_of_int i
  | Float f when t = STInt -> string_of_int (int_of_float f)
  | Float f -> string_of_float f
  | String s -> s
  (* TODO: Vec's with non-value types will break *)
  | Vec2 (e1, e2) when is_value e -> vec2_string_of_val e1 e2
  | Vec3 (e1, e2, e3) when is_value e -> vec3_string_of_val e1 e2 e3
  | Vec4 (e1, e2, e3, e4) when is_value e -> vec4_string_of_val e1 e2 e3 e4
  | _ -> raise (TypeError err_no_str_rep)
;;

(** [inf_string_of_val e] converts [e] to a string, infers it's internal type *)
let inf_typed_string_of_val (e : expr) =
  match e with
  | Int _ -> typed_string_of_val STInt e
  | Float _ -> typed_string_of_val STFloat e
  | String _ -> typed_string_of_val STString e
  | Vec2 _ -> typed_string_of_val STVec2 e
  | Vec3 _ -> typed_string_of_val STVec3 e
  | Vec4 _ -> typed_string_of_val STVec4 e
  | _ -> raise (TypeError err_no_str_rep)
;;
