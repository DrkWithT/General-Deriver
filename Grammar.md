# Expression Grammar

### Notes:
 - The grammar is written to be parseable by a recursive descent parser.
 - The grammar omits multiplication and division for now, as handling product and quotient functions would be upcoming, more involved obstacles for now.
    - NOTE 1: The AST still has multiplication and division support for use in resulting function instances only.
    - NOTE 2: Functions that form a larger composed function should have a special model.

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

### Syntax Samples:
```
(x - 1)^3
x - (x^2 + 1)
(x + 1)^2 - (x + 1)
```
