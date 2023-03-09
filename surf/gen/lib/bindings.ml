open Surf
open Ctypes

let rec fib n = if n < 2 then 1 else fib (n - 1) + fib (n - 1)
let fmt_res res = Printf.sprintf "Result is: %d\n" res

module Stubs (I : Cstubs_inverted.INTERNAL) = struct
  let () = I.internal "fib" (int @-> returning int) fib
  let () = I.internal "fmt_res" (int @-> returning string) fmt_res
  let () = I.internal "interp" (string @-> returning string) Interpreter.interp_ret
end
