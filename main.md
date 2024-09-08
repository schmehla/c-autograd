# Grammar

Inspired by [YouTube help](https://www.youtube.com/watch?v=SToUyjAsaFk).

### Abbrevations

-   E: Expression
-   T: Term
-   F: Factor

### Rules

-   E &rarr; T + E | T - E | T
-   or: E &rarr; T (+|- T)+
-   T &rarr; F \* T | F / T | F
-   F &rarr; Var | Num | (E) | -F

### Tokens

-   Var: \w+
-   Num: \d+(\.\w+)?
-   and: +, -, \*, /, (, ),
