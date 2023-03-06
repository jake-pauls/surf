(* open Interpreter let () = Callback.register "interp" interp *)

(** [fib n] sample api method to calculate the fibonacci value of a number *)
let rec fib n = if n < 2 then 1 else fib (n - 1) + fib (n - 1)

let _ = Callback.register "fib" fib

(** [fmt_res res] sample api method to print the result of an integer *)
let fmt_res res = Printf.sprintf "Result is: %d\n" res

let _ = Callback.register "fmt_res" fmt_res
