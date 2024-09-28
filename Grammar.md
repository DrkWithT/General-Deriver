# Expression Grammar

### Grammar Rules:
```
whitespace = " " | "\t" | "\r" | "\n"
number = DIGIT+ "."? DIGIT*
variable = "x"

literal = number | variable | "(" expr ")"
unary = "-"? literal
power = unary ("^" number)?
<!-- factor = power (("*" | "/") power)* -->
<!-- term = factor (("+" | "-") factor)* -->
term = power (("+" | "-") power)*
expr = term
```
