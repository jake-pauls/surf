open Core
open Surf

(** [main unit] usage: file_name file is instantly ran through the interpreter *)
let () =
  let args : string array = Sys.get_argv () in
  let read_file file = In_channel.read_lines file in
  let lines = read_file args.(1) in
  List.iter lines ~f:(fun s -> Interpreter.interp s)
