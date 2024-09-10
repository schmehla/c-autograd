#include "optimizer.h"
#include "operations.h"
#include "parser.h"
#include <math.h>
#include <stdio.h>

const float LR = 10e-3; // learning rate
const float B_1 = 0.9;
const float B_2 = 0.999;
const size_t N_STEPS = 10000;

float optim(char *expr, VarValues *var_values) {
    Node *parse_tree = get_parse_tree(expr);
    Node *deriv_trees[var_values->len];
    float m[var_values->len];
    float v[var_values->len];
    for (size_t i = 0; i < var_values->len; ++i) {
        deriv_trees[i] = deriv(parse_tree, var_values->arr[i].name);
        m[i] = 0;
        v[i] = 0;
    }
    for (size_t t = 0; t < N_STEPS; ++t) {
        printf("function value: %.2f", eval(parse_tree, var_values));
        for (size_t i = 0; i < var_values->len; ++i) {
            printf(", %s = %.2f ", var_values->arr[i].name,
                   var_values->arr[i].value);
            printf(" (gradient: %.2f)", eval(deriv_trees[i], var_values));
            float grad = eval(deriv_trees[i], var_values);
            m[i] = B_1 * m[i] + (1 - B_1) * grad;
            v[i] = B_2 * v[i] + (1 - B_2) * grad * grad;
            float m_hat = m[i] / (1 - pow(B_1, t + 1));
            float v_hat = v[i] / (1 - pow(B_2, t + 1));
            var_values->arr[i].value -= LR * m_hat / (sqrt(v_hat) + 10e-6);
        }
        printf("\n");
    }
    float min = eval(parse_tree, var_values);
    free_parse_tree(parse_tree);
    for (size_t i = 0; i < var_values->len; ++i) {
        free_parse_tree(deriv_trees[i]);
    }
    return min;
}
