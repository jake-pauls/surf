val typeof : 'a Env.StaticEnvironment.t -> Ast.expr -> Ast.stype

val typeof_let
  :  'a Env.StaticEnvironment.t
  -> string
  -> Ast.stype
  -> Ast.expr
  -> Ast.stype

val typeof_unop : 'a Env.StaticEnvironment.t -> Ast.unop -> Ast.expr -> Ast.stype

val typeof_binop
  :  'a Env.StaticEnvironment.t
  -> Ast.binop
  -> Ast.expr
  -> Ast.expr
  -> Ast.stype

val typecheck : 'a Env.StaticEnvironment.t -> Ast.expr -> Ast.stype option
