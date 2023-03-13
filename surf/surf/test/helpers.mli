val empty : Surf.Ast.expr Surf.Env.StaticEnvironment.t

val check_interp_int
  :  string
  -> int
  -> string
  -> Surf.Ast.expr Surf.Env.StaticEnvironment.t
  -> unit

val check_interp_float
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

val check_interp_vec2f
  :  string
  -> float array
  -> string
  -> Surf.Ast.expr Surf.Env.StaticEnvironment.t
  -> unit

val check_interp_vec2i
  :  string
  -> int array
  -> string
  -> Surf.Ast.expr Surf.Env.StaticEnvironment.t
  -> unit

val check_interp_vec3i
  :  string
  -> int array
  -> string
  -> Surf.Ast.expr Surf.Env.StaticEnvironment.t
  -> unit

val check_interp_vec3f
  :  string
  -> float array
  -> string
  -> Surf.Ast.expr Surf.Env.StaticEnvironment.t
  -> unit

val check_interp_vec4i
  :  string
  -> int array
  -> string
  -> Surf.Ast.expr Surf.Env.StaticEnvironment.t
  -> unit

val check_interp_vec4f
  :  string
  -> float array
  -> string
  -> Surf.Ast.expr Surf.Env.StaticEnvironment.t
  -> unit
