#include "operations.h"
#include "node.h"
// #include "parser.h"
#include <assert.h>
#include <stdbool.h>
#include <string.h>

void eval(Node *node, VarTable *var_table) {
    if (node->type == BIN_NODE) {
        BinNode bn = node->data->bin_node;
        eval(bn.lhs, var_table);
        eval(bn.rhs, var_table);
        if (bn.op == ADD)
            node->val = bn.lhs->val + bn.rhs->val;
        if (bn.op == SUB)
            node->val = bn.lhs->val - bn.rhs->val;
        if (bn.op == MUL)
            node->val = bn.lhs->val * bn.rhs->val;
        if (bn.op == DIV)
            node->val = bn.lhs->val / bn.rhs->val;
        return;
    }
    if (node->type == UN_NODE) {
        UnNode un = node->data->un_node;
        eval(un.child, var_table);
        if (un.op == NEG)
            node->val = -un.child->val;
        return;
    }
    if (node->type == VAR_NODE) {
        VarNode vn = node->data->var_node;
        for (size_t i = 0; i < var_table->len; ++i) {
            if (strcmp(vn.name, var_table->arr[i].name) == 0) {
                node->val = var_table->arr[i].value;
                return;
            }
        }
        assert(false);
    }
    if (node->type == NUM_NODE) {
        NumNode nn = node->data->num_node;
        node->val = nn.num;
        return;
    }
    assert(false);
}

void _backprop(Node *node) {
    if (node->type == BIN_NODE) {
        BinNode bn = node->data->bin_node;
        if (bn.op == ADD) {
            bn.lhs->grad += node->grad;
            _backprop(bn.lhs);
            bn.rhs->grad += node->grad;
            _backprop(bn.rhs);
        }
        if (bn.op == SUB) {
            bn.lhs->grad += node->grad;
            _backprop(bn.lhs);
            bn.rhs->grad -= node->grad;
            _backprop(bn.rhs);
        }
        if (bn.op == MUL) {
            bn.lhs->grad += bn.rhs->val * node->grad;
            _backprop(bn.lhs);
            bn.rhs->grad += bn.lhs->val * node->grad;
            _backprop(bn.rhs);
        }
        if (bn.op == DIV) {
            bn.lhs->grad += 1 / bn.rhs->val * node->grad;
            _backprop(bn.lhs);
            bn.rhs->grad +=
                -bn.lhs->val / (bn.rhs->val * bn.rhs->val) * node->grad;
            _backprop(bn.rhs);
        }
        return;
    }
    if (node->type == UN_NODE) {
        UnNode un = node->data->un_node;
        if (un.op == NEG) {
            un.child->grad += -node->grad;
            _backprop(un.child);
        }
        return;
    }
    if (node->type == VAR_NODE) {
        return;
    }
    if (node->type == NUM_NODE) {
        return;
    }
    assert(false);
}

void backprop(Node *node) {
    zero_grad(node);
    node->grad = 1;
    _backprop(node);
}
