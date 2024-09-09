# Grammar

Inspired by [YouTube help](https://www.youtube.com/watch?v=SToUyjAsaFk).

### Abbrevations

-   E: Expression
-   T: Term
-   F: Factor

### Rules

-   E &rarr; T + E | T - E | T
-   for correct associativity: E &rarr; T (+|- T)+
-   T &rarr; F \* T | F / T | F
-   for correct associativity: T &rarr; F (\*|/ F)+
-   F &rarr; Var | Num | (E) | -F

### Tokens

-   Var: \w+
-   Num: \d+(\.\w+)?
-   and: +, -, \*, /, (, ),

### Example

-   input: (1.1 \* 0.2 / 10 + x)
-   E -> T -> F: 1.1 \* 0.2 / 10 + x, E
-   E -> T -> F
