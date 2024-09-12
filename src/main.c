#include "operations.h"
// #include "optimizer.h"
#include "parser.h"
#include <stdio.h>

int main(void) {
    char expr[] = "(x-2)*(x-3)*x + (y-10)*y + x+y";

    Var v[] = {{.name = "x", .value = 1}, {.name = "y", .value = 1}};
    VarTable var_values = {.arr = v, .len = 2};

    ParseTree *pt = get_parse_tree(expr);
    printf("eval: %f", eval(pt->root, &var_values));
    free_parse_tree(pt);

    // float min = optim(expr, &var_values);
    // for (size_t i = 0; i < var_values.len; ++i) {
    //     printf("%s = %f\n", var_values.arr[i].name, var_values.arr[i].value);
    // }
    // printf("min: %f\n", min);
    printf("\n");
    return 0;
}
