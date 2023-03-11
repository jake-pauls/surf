module type ENVIRONMENT = sig
  type t

  val empty : unit -> t
  val lookup : string -> t -> string * Ast.stype
  val update : string -> string * Ast.stype -> t -> unit
end

module StaticEnvironment : ENVIRONMENT
