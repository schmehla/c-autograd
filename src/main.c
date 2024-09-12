#include "operations.h"
#include "optimizer.h"
// #include "parser.h"
#include <stdio.h>

int main(void) {
    char expr[] = "(x-2)*(x-3)*x + (y-10)*y + x+y";

    Var v[] = {{.name = "x", .value = -3}, {.name = "y", .value = 3}};
    VarTable var_values = {.arr = v, .len = 2};

    // ParseTree *pt = get_parse_tree(expr);
    // eval(pt->root, &var_values);
    // printf("eval: %f\n", pt->root->val);
    // backprop(pt->root);

    // for (NodeList *cur = *pt->vars; cur != NULL; cur = cur->next) {
    //     printf("%s: %f\n", cur->data->data->var_node.name, cur->data->grad);
    // }
    // free_parse_tree(pt);

    float min = optim(expr, &var_values);
    for (size_t i = 0; i < var_values.len; ++i) {
        printf("%s = %f\n", var_values.arr[i].name, var_values.arr[i].value);
    }
    printf("min: %f\n", min);
    printf("\n");
    return 0;
}
