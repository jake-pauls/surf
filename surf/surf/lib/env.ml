open Ast
open Errors

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
  type t = (string, string * stype) Hashtbl.t

  (** [empty unit] default hash table with 16 initial slots *)
  let empty () = Hashtbl.create 16

  (** [lookup k env] looks up a [k] in the [env] and returns a value throws a runtime
      error if the variable is not bound *)
  let lookup k env =
    try Hashtbl.find env k with
    | Not_found -> raise_rt_error err_unbound_var
  ;;

  (** [update k v env] updates a [k] in the [env] to be bound to [v] *)
  let update k v env = Hashtbl.replace env k v
end
