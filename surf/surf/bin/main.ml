open Core
open Surf
open Env

(** [interp_file file_name] interprets passed file one line at a time *)
let interp_file env file_name : unit =
  let read_file file = In_channel.read_lines file in
  let lines = read_file file_name in
  List.iter lines ~f:(fun s -> Interpreter.interp env s)
;;

(** [repl stdin] enters a repl environment via stdin *)
let rec repl env stdin : unit =
  match stdin with
  | Some s ->
    Interpreter.interp env s
    ; repl env (In_channel.input_line In_channel.stdin)
  | None -> ()
;;

(** [main unit] usage: file_name file is instantly ran through the interpreter *)
let () =
  let env : StaticEnvironment.t = StaticEnvironment.empty () in
  let args : string array = Sys.get_argv () in
  let len = Array.length args in
  if len > 1
  then interp_file env args.(1)
  else (
    print_endline "<><>- surf repl. -<><>"
    ; let stdin = In_channel.input_line In_channel.stdin in
      repl env stdin
      ; print_endline "<><>- bye. -<><>")
;;
