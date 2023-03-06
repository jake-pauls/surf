(** [bop] binary operations *)
type bop = Add | Minus | Mult | Div

(** [unop] unary operations *)
type unop = Minus

(** [expr] expression types *)
type expr = Int of int | Binop of bop * expr * expr | Unop of unop * expr
