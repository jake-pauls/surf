open Ctypes
open Surf

(** internal stubs exported to dll **)
module Stubs (I : Cstubs_inverted.INTERNAL) = struct
  open Env

  let senv_t : 'a StaticEnvironmentBindings.senv_struct typ =
    structure StaticEnvironmentBindings.senv_struct_name
  ;;

  let () = I.typedef senv_t StaticEnvironmentBindings.senv_struct_name

  let () =
    I.internal
      "surfgen_InitRuntime"
      (void @-> returning (ptr senv_t))
      StaticEnvironmentBindings.empty
  ;;

  let () =
    I.internal
      "surfgen_Interp"
      (ptr senv_t @-> string @-> returning string)
      Interpreter.c_interp_ret
  ;;
end
