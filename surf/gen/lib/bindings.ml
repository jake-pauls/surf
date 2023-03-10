open Surf
open Ctypes

(** internal stubs exported to dll **)
module Stubs (I : Cstubs_inverted.INTERNAL) = struct
  let () = I.internal "interp" (string @-> returning string) Interpreter.interp_ret
end
