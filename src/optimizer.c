#include "optimizer.h"
#include "node.h"
#include "node_list.h"
#include <math.h>
#include <stdio.h>
#include <string.h>

const float LR = 10e-3; // learning rate
const float MAX_GRAD = 10e-4;
const size_t MAX_STEPS = 10e6;

float optim(Node *node, NodeList *numeric_vars) {
    bool done;
    set_topo_lvl(node, 0);
    for (size_t t = 0; t < MAX_STEPS; ++t) {
        eval(node);
        zero_grad(node);
        backprop(node);
        done = true;
        _Elem *curr = numeric_vars->first; // TODO _Elem is private!
        while (curr != NULL) {
            if (isnan(curr->data->grad) || isinf(curr->data->grad)) {
                done = false;
                break;
            }
            curr->data->val -= LR * curr->data->grad;
            done = (fabs(curr->data->grad) < MAX_GRAD) && done;
            curr = curr->next;
        }
        if (done)
            break;
    }
    if (!done)
        printf("No minima found!\n");
    eval(node);
    float min = node->val;
    return min;
}
