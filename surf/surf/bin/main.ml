open Core
open Surf

(** [interp_file file_name] *)
let interp_file (file_name : string) : unit =
  let read_file file = In_channel.read_lines file in
  let lines = read_file file_name in
  List.iter lines ~f:(fun s -> Interpreter.interp s)
;;

(** [repl stdin] enters a repl environment via stdin *)
let repl stdin : unit =
  let rec rrepl stdin : unit =
    match stdin with
    | Some s ->
      Interpreter.interp s
      ; rrepl (In_channel.input_line In_channel.stdin)
    | None -> ()
  in
  rrepl stdin
;;

(** [main unit] usage: file_name file is instantly ran through the interpreter *)
let () =
  let args : string array = Sys.get_argv () in
  let len = Array.length args in
  if len > 1
  then interp_file args.(1)
  else (
    print_endline "<><>- surf repl. -<><>"
    ; repl (In_channel.input_line In_channel.stdin)
    ; print_endline "<><>- bye. -<><>")
;;
