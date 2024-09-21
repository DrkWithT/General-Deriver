# Expression Grammar

### Grammar Rules:
```
whitespace = " " | "\t" | "\r" | "\n"
number = DIGIT+ "."? DIGIT*
variable = "x"

literal = number | variable | "(" expr ")"
unary = "-"? literal
factor = unary (("*" | "/") unary)*
term = factor (("+" | "-") factor)*
expr = term
```
