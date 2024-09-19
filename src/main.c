// #include "optimizer.h"
#include "node.h"
#include "node_list.h"
#include "parser.h"
#include "tree_iter.h"
#include <stdio.h>

int main(void) {
    char expr[] = "(x-2)*(x-3)*x*y + (y-10)*y + x+y";
    // char expr[] = "2 * (x+3) + 4*x + y";

    NodeList *leaves = new_node_list();
    Node *parse_tree = parse(expr, leaves);
    full_eval(parse_tree);
    printf("eval: %f\n", parse_tree->val);
    set_topo_lvl(parse_tree, 0);
    zero_grad(parse_tree);
    full_backprop(parse_tree);

    _Elem *curr = leaves->first;
    while (curr != NULL) {
        printf("leaf: name: %s val: %f grad: %f\n", curr->data->name,
               curr->data->val, curr->data->grad);
        curr = curr->next;
    }

    // Node *x = find_by_name(parse_tree, "x");
    // printf("x: name: %s val: %f grad: %f topolvl: %zu\n", x->name, x->val,
    // x->grad, x->topo_lvl);

    // for (NodeList *cur = *pt->vars; cur != NULL; cur = cur->next) {
    //     printf("%s: %f\n", cur->data->data->var_node.name, cur->data->grad);
    // }
    free_node(parse_tree);
    free_node_list(leaves);

    // float min = optim(expr, &var_values);
    // for (size_t i = 0; i < var_values.len; ++i) {
    //     printf("%s = %f\n", var_values.arr[i].name, var_values.arr[i].value);
    // }
    // printf("min: %f\n", min);
    printf("\n");
    return 0;
}
