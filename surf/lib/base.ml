let four : int = 2 + 2
let name : string = "surf"

let rec fib n = if n < 2 then 1 else fib (n - 1) + fib (n - 1)
let fmt_res res = Printf.sprintf "Result is: %d\n" res

let () = Callback.register "fib" fib
let () = Callback.register "fmt_res" fmt_res