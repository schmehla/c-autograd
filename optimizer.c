#include "optimizer.h"
#include "operations.h"
#include "parser.h"
#include <math.h>
#include <stdio.h>

const float LR = 10e-3; // learning rate
const float MAX_GRAD = 10e-4;
const size_t MAX_STEPS = 100000;

float optim(char *expr, VarValues *var_values) {
    Node *parse_tree = get_parse_tree(expr);
    Node *deriv_trees[var_values->len];
    for (size_t i = 0; i < var_values->len; ++i) {
        deriv_trees[i] = deriv(parse_tree, var_values->arr[i].name);
    }
    bool less_than_max_grad;
    for (size_t t = 0; t < MAX_STEPS; ++t) {
        // printf("iter: %zu, function value: %.2f", t,
        //        eval(parse_tree, var_values));
        less_than_max_grad = true;
        for (size_t i = 0; i < var_values->len; ++i) {
            // printf(", %s = %.2f ", var_values->arr[i].name,
            //        var_values->arr[i].value);
            float grad = eval(deriv_trees[i], var_values);
            if (isnan(grad) || isinf(grad)) {
                less_than_max_grad = false;
                break;
            }
            // printf(" (grad: %f)", grad);
            var_values->arr[i].value -= LR * grad;
            less_than_max_grad = (fabs(grad) < MAX_GRAD) && less_than_max_grad;
        }
        // printf("\n");
        if (less_than_max_grad)
            break;
    }
    if (!less_than_max_grad)
        printf("No minima found!\n");
    float min = eval(parse_tree, var_values);
    free_parse_tree(parse_tree);
    for (size_t i = 0; i < var_values->len; ++i) {
        free_parse_tree(deriv_trees[i]);
    }
    return min;
}
