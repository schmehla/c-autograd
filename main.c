#include "operations.h"
#include "optimizer.h"
#include <stdio.h>

int main(void) {
    char expr[] = "(x-2)*(x-3) + y*y";

    VarValue v[] = {{.name = "x", .value = 10}, {.name = "y", .value = 1}};
    VarValues var_values = {.arr = v, .len = 2};

    printf("\nmin: %f\n", optim(expr, &var_values));

    for (size_t i = 0; i < var_values.len; ++i) {
        printf("%s = %f\n", var_values.arr[i].name, var_values.arr[i].value);
    }

    // printf("derive by x: \n");
    // Node *d_parse_tree_dx = deriv(parse_tree, "x");
    // print(d_parse_tree_dx);
    // printf("\nevals to: %f\n", eval(d_parse_tree_dx, &var_values));
    // printf("\n");
    // free_parse_tree(d_parse_tree_dx);

    // printf("derive by y: \n");
    // Node *d_parse_tree_dy = deriv(parse_tree, "y");
    // print(d_parse_tree_dy);
    // printf("\nevals to: %f\n", eval(d_parse_tree_dy, &var_values));
    // printf("\n");
    // free_parse_tree(d_parse_tree_dy);

    // free_parse_tree(parse_tree);
    printf("\n");
    return 0;
}
