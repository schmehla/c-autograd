#include "optimizer.h"
#include "node.h"
#include "node_list.h"
#include "parser.h"
#include "tree_iter.h"
#include <math.h>
#include <stdio.h>
#include <string.h>

const float LR = 10e-3; // learning rate
const float MAX_GRAD = 10e-4;
const size_t MAX_STEPS = 10e6;

float optim(char *expr) {
    NodeList *leaves = new_node_list();
    Node *root = parse(expr, leaves);
    bool done;
    for (size_t t = 0; t < MAX_STEPS; ++t) {
        // printf("iter: %zu, function value: %.2f", t,
        //        eval(parse_tree, var_values));
        done = true;
        full_eval(root);
        zero_grad(root);
        full_backprop(root);
        _Elem *curr = leaves->first;
        while (curr != NULL) {
            if (isnan(curr->data->grad) || isinf(curr->data->grad)) {
                done = false;
                break;
            }
            // printf(" (grad: %f)", grad);
            curr->data->val -= LR * curr->data->grad;
            done = (fabs(curr->data->grad) < MAX_GRAD) && done;
        }
        // printf("\n");
        if (done)
            break;
        curr = curr->next;
    }
    if (!done)
        printf("No minima found!\n");
    full_eval(root);
    float min = root->val;
    free_node(root);
    return min;
}
