module type ENVIRONMENT = sig
  type t

  val empty : unit -> t
  val lookup : string -> t -> string * Ast.stype
  val update : string -> string * Ast.stype -> t -> unit
end

module StaticEnvironment : ENVIRONMENT

module StaticEnvironmentBindings : sig
  val senv_struct_name : string

  type senv_struct = StaticEnvironment.t Ctypes.structure

  val to_opaque : StaticEnvironment.t -> senv_struct Ctypes.ptr
  val from_opaque : senv_struct Ctypes.ptr -> StaticEnvironment.t
  val empty : unit -> senv_struct Ctypes.ptr
end
