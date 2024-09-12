#include "optimizer.h"
#include "node.h"
#include "operations.h"
#include "parser.h"
#include <math.h>
#include <stdio.h>
#include <string.h>

const float LR = 10e-3; // learning rate
const float MAX_GRAD = 10e-4;
const size_t MAX_STEPS = 10e6;

float optim(char *expr, VarTable *var_table) {
    ParseTree *pt = get_parse_tree(expr);
    bool done;
    for (size_t t = 0; t < MAX_STEPS; ++t) {
        // printf("iter: %zu, function value: %.2f", t,
        //        eval(parse_tree, var_values));
        done = true;
        eval(pt->root, var_table);
        backprop(pt->root);
        for (NodeList *curr = *pt->vars; curr != NULL; curr = curr->next) {
            // printf(", %s = %.2f ", var_values->arr[i].name,
            //        var_values->arr[i].value);
            float grad = curr->data->grad;
            if (isnan(grad) || isinf(grad)) {
                done = false;
                break;
            }
            // printf(" (grad: %f)", grad);

            for (size_t i = 0; i < var_table->len; ++i) {
                if (strcmp(curr->data->data->var_node.name,
                           var_table->arr[i].name) == 0)
                    var_table->arr[i].value -= LR * grad;
            }
            done = (fabs(grad) < MAX_GRAD) && done;
        }
        // printf("\n");
        if (done)
            break;
    }
    if (!done)
        printf("No minima found!\n");
    float min = pt->root->val;
    eval(pt->root, var_table);
    free_parse_tree(pt);
    return min;
}
