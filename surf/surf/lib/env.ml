open Ast
open Errors
open Ctypes

module type ENVIRONMENT = sig
  type t

  (** [empty unit] the empty state for the environment *)
  val empty : unit -> t

  (** [lookup string t] looks up a [string] key in data structure [t], returns a [stype] *)
  val lookup : string -> t -> string * stype

  (** [update string stype t] updates a [string] key in data structure [t] with a new
      [stype] *)
  val update : string -> string * stype -> t -> unit
end

module StaticEnvironment : ENVIRONMENT = struct
  type t = (string, string * stype) Hashtbl.t ref

  (** [empty unit] default hash table with 16 initial slots *)
  let empty () = ref (Hashtbl.create 16)

  (** [lookup k env] looks up a [k] in the [env] and returns a value throws a runtime
      error if the variable is not bound *)
  let lookup k env =
    try Hashtbl.find !env k with
    | Not_found -> raise_rt_error err_unbound_var
  ;;

  (** [update k v env] updates a [k] in the [env] to be bound to [v] *)
  let update k v env = Hashtbl.replace !env k v
end

module StaticEnvironmentBindings = struct
  let senv_struct_name = "StaticEnvironment"

  (** [senv_struct] c-structure representation of the [StaticEnvironment] *)
  type senv_struct = StaticEnvironment.t structure

  (** [to_opaque ovalue] creates an opaque pointer from an [ovalue] representing a static
      environment *)
  let to_opaque (ovalue : StaticEnvironment.t) : senv_struct ptr =
    from_voidp (structure senv_struct_name) (Root.create ovalue)
  ;;

  (** [from_oaque opaque] converts a [opaque] static environment pointer to a static
      environment *)
  let from_opaque (opaque : senv_struct ptr) : StaticEnvironment.t =
    Root.get (to_voidp opaque)
  ;;

  (** [empty unit] wrapper for the initialization of a [StaticEnvironment] to a pointer to
      a c-structure *)
  let empty () = to_opaque (StaticEnvironment.empty ())
end
