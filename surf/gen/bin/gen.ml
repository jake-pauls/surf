(** [path dir_name file_name] concatenates a file path string given a directory and a
    filename will create the directory if it does not exist *)
let path dir_name file_name =
  if not (Sys.file_exists dir_name) then Sys.mkdir dir_name 0o755
  ; Filename.concat dir_name file_name
;;

(** [gen dir_name ml_file c_file h_file] generates the ml, c, and h bindings for functions
    exported via ctypes to the provided file paths *)
let gen dir_name ml_file c_file h_file =
  let prefix = "gen" in
  let ml_file_oc = open_out (path dir_name ml_file) in
  let c_file_oc = open_out (path dir_name c_file) in
  let h_file_oc = open_out (path dir_name h_file) in
  let stubs = (module Bindings.Stubs : Cstubs_inverted.BINDINGS) in
  (* Generate the ML module that links in the generated C *)
  Cstubs_inverted.write_ml (Format.formatter_of_out_channel ml_file_oc) ~prefix stubs
  ; (* Generate the C source file that exports OCaml functions *)
    Format.fprintf
      (Format.formatter_of_out_channel c_file_oc)
      "#include \"gen.h\"@\n%a"
      (Cstubs_inverted.write_c ~prefix)
      stubs
  ; (* Generate the C header file that exports OCaml functions *)
    Cstubs_inverted.write_c_header
      (Format.formatter_of_out_channel h_file_oc)
      ~prefix
      stubs
  ; close_out h_file_oc
  ; close_out c_file_oc
  ; close_out ml_file_oc
;;

(** [main unit] usage: out_dir ml_file c_file h_file *)
let () = gen Sys.argv.(1) Sys.argv.(2) Sys.argv.(3) Sys.argv.(4)
