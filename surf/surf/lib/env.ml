open Ast
open Errors
open Ctypes

module type ENVIRONMENT = sig
  type 'a t

  (** [empty unit] the empty state for the environment *)
  val empty : unit -> 'a t

  (** [lookup string t] looks up a [string] key in data structure [t], returns a [stype] *)
  val lookup : string -> 'a t -> 'a * stype

  (** [update string stype t] updates a [string] key in data structure [t] with a new
      [stype] *)
  val update : string -> 'a * stype -> 'a t -> unit
end

module StaticEnvironment : ENVIRONMENT = struct
  type 'a t = (string, 'a * stype) Hashtbl.t ref

  (** [empty unit] default hash table with 16 initial slots *)
  let empty () = ref (Hashtbl.create 16)

  (** [lookup k env] looks up a [k] in the [env] and returns a value throws a runtime
      error if the variable is not bound *)
  let lookup k env =
    try Hashtbl.find !env k with
    | Not_found -> raise (RuntimeError err_unbound_var)
  ;;

  (** [update k v env] updates a [k] in the [env] to be bound to [v] *)
  let update k v env = Hashtbl.replace !env k v
end

module StaticEnvironmentBindings = struct
  let senv_struct_name = "StaticEnvironment"

  (** [senv_struct] c-structure representation of the [StaticEnvironment] *)
  type 'a senv_struct = 'a StaticEnvironment.t structure

  (** [to_opaque ovalue] creates an opaque pointer from an [ovalue] representing a static
      environment *)
  let to_opaque (ovalue : 'a StaticEnvironment.t) : 'a senv_struct ptr =
    from_voidp (structure senv_struct_name) (Root.create ovalue)
  ;;

  (** [from_oaque opaque] converts a [opaque] static environment pointer to a static
      environment *)
  let from_opaque (opaque : 'a senv_struct ptr) : 'a StaticEnvironment.t =
    Root.get (to_voidp opaque)
  ;;

  (** [empty unit] wrapper for the initialization of a [StaticEnvironment] to a pointer to
      a c-structure *)
  let empty () = to_opaque (StaticEnvironment.empty ())
end
