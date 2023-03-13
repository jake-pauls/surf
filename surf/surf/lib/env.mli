module type ENVIRONMENT = sig
  type 'a t

  val empty : unit -> 'a t
  val lookup : string -> 'a t -> 'a * Ast.stype
  val update : string -> 'a * Ast.stype -> 'a t -> unit
end

module StaticEnvironment : ENVIRONMENT

module StaticEnvironmentBindings : sig
  val senv_struct_name : string

  type 'a senv_struct = 'a StaticEnvironment.t Ctypes.structure

  val to_opaque : 'a StaticEnvironment.t -> 'a senv_struct Ctypes.ptr
  val from_opaque : 'a senv_struct Ctypes.ptr -> 'a StaticEnvironment.t
  val empty : unit -> 'a senv_struct Ctypes.ptr
end
