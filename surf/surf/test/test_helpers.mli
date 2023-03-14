val empty : Surf.Ast.expr Surf.Env.StaticEnvironment.t

val check_interp_int
  :  string
  -> int
  -> string
  -> Surf.Ast.expr Surf.Env.StaticEnvironment.t
  -> unit

val check_interp_flt
  :  string
  -> float
  -> string
  -> Surf.Ast.expr Surf.Env.StaticEnvironment.t
  -> unit

val check_interp_str
  :  string
  -> string
  -> string
  -> Surf.Ast.expr Surf.Env.StaticEnvironment.t
  -> unit

val check_interp_v2f
  :  string
  -> float array
  -> string
  -> Surf.Ast.expr Surf.Env.StaticEnvironment.t
  -> unit

val check_interp_v2i
  :  string
  -> int array
  -> string
  -> Surf.Ast.expr Surf.Env.StaticEnvironment.t
  -> unit

val check_interp_v3i
  :  string
  -> int array
  -> string
  -> Surf.Ast.expr Surf.Env.StaticEnvironment.t
  -> unit

val check_interp_v3f
  :  string
  -> float array
  -> string
  -> Surf.Ast.expr Surf.Env.StaticEnvironment.t
  -> unit

val check_interp_v4i
  :  string
  -> int array
  -> string
  -> Surf.Ast.expr Surf.Env.StaticEnvironment.t
  -> unit

val check_interp_v4f
  :  string
  -> float array
  -> string
  -> Surf.Ast.expr Surf.Env.StaticEnvironment.t
  -> unit
