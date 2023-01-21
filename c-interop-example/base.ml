(** Code taken from sample on https://v2.ocaml.org/manual/intfc.html *)

let rec fib n = if n < 2 then 1 else fib (n - 1) + fib (n - 1)
let fmt_res n = Printf.sprintf "Result is: %d\n" n

let _ = Callback.register "fib" fib
let _ = Callback.register "fmt_res" fmt_res
