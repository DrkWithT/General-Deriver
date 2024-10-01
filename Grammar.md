# Expression Grammar

### Notes:
 - The grammar is written to be parseable by a recursive descent parser.
 - The grammar omits multiplication and division for now, as handling the product, quotient, and chain rules would be upcoming, more involved obstacles for now.

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
