#include "operations.h"
#include "parser.h"

#include <assert.h>
#include <string.h>

float eval(Node *n, VarValues *var_values) {
    if (n->type == BIN_NODE) {
        BinNode bn = n->data->bin_node;
        if (bn.op == ADD)
            return eval(bn.lhs, var_values) + eval(bn.rhs, var_values);
        if (bn.op == SUB)
            return eval(bn.lhs, var_values) - eval(bn.rhs, var_values);
        if (bn.op == MUL)
            return eval(bn.lhs, var_values) * eval(bn.rhs, var_values);
        if (bn.op == DIV)
            return eval(bn.lhs, var_values) / eval(bn.rhs, var_values);
    }
    if (n->type == UN_NODE) {
        UnNode un = n->data->un_node;
        if (n->data->un_node.op == NEG)
            return -eval(un.child, var_values);
        assert(false);
    }
    if (n->type == VAR_NODE) {
        VarNode vn = n->data->var_node;
        for (size_t i = 0; i < var_values->len; ++i) {
            if (strcmp(vn.name, var_values->arr[i].name) == 0)
                return var_values->arr[i].value;
        }
        assert(false);
    }
    if (n->type == NUM_NODE) {
        NumNode nn = n->data->num_node;
        return nn.num;
    }
    assert(false);
}

// void simpl(Node *n, Node *parent) {
//   if (n->type == BIN_NODE) {
//     BinNode bn = n->data->bin_node;
//     simpl(bn.lhs, n);
//     simpl(bn.rhs, n);
//     if (bn.lhs->type == NUM_NODE && bn.rhs->type == NUM_NODE) {
//       free(n); // TODO this does not work without resetting parents pointer
//       n = create_num_node(bn.lhs->data->num_node.num +
//                           bn.rhs->data->num_node.num);
//     }
//   }
//   if (n->type == UN_NODE) {
//     UnNode un = n->data->un_node;
//     simpl(un.child, n);
//     if (un.child->type == NUM_NODE) {
//       free(n);
//       n = create_num_node(-un.child->data->num_node.num);
//     }
//   }
// if (n->node_type == VAR_NODE) {
//   VarNode vn = n->node_data->var_node;
// }
// if (n->node_type == NUM_NODE) {
//   NumNode nn = n->node_data->num_node;
// }
// assert(false);
// }

Node *copy(Node *n) {
    if (n->type == BIN_NODE) {
        BinNode bn = n->data->bin_node;
        return create_bin_node(bn.op, copy(bn.lhs), copy(bn.rhs));
    }
    if (n->type == UN_NODE) {
        UnNode un = n->data->un_node;
        return create_un_node(un.op, copy(un.child));
    }
    if (n->type == VAR_NODE) {
        VarNode vn = n->data->var_node;
        return create_var_node(vn.name);
    }
    if (n->type == NUM_NODE) {
        NumNode nn = n->data->num_node;
        return create_num_node(nn.num);
    }
    assert(false);
}

// TODO derive by what?
Node *deriv(Node *n, char *by) {
    if (n->type == NUM_NODE)
        return create_num_node(0);
    if (n->type == VAR_NODE) {
        if (strcmp(n->data->var_node.name, by) == 0)
            return create_num_node(1);
        return create_num_node(0);
    }
    if (n->type == BIN_NODE) {
        // Node *lhs_deriv = derivative(n->node_data->bin_node.lhs);
        // Node *rhs_deriv = derivative(n->node_data->bin_node.rhs);
        // Node *lhs_copy = copy(n->node_data->bin_node.lhs);
        // Node *rhs_copy = copy(n->node_data->bin_node.rhs);
        Node *lhs = n->data->bin_node.lhs;
        Node *rhs = n->data->bin_node.rhs;
        if (n->data->bin_node.op == ADD)
            return create_bin_node(ADD, deriv(lhs, by), deriv(rhs, by));
        if (n->data->bin_node.op == SUB)
            return create_bin_node(SUB, deriv(lhs, by), deriv(rhs, by));
        if (n->data->bin_node.op == MUL) {
            // d (u(x) * v(x)) / dx = du(x)/dx * v(x) + u(x) * dv(x)/dx
            // lhs: u
            // rhs: v
            return create_bin_node(
                ADD, create_bin_node(MUL, deriv(lhs, by), copy(rhs)),
                create_bin_node(MUL, copy(lhs), deriv(rhs, by)));
        }
        if (n->data->bin_node.op == DIV) {
            // d (u(x) / v(x)) / dx = v(x)² * [du(x)/dx * v(x) - u(x) *
            // dv(x)/dx] lhs: u rhs: v
            Node *lhs_sq = create_bin_node(MUL, copy(lhs), copy(lhs));
            Node *n = create_bin_node(
                ADD, create_bin_node(MUL, deriv(lhs, by), copy(rhs)),
                create_bin_node(MUL, copy(lhs), deriv(rhs, by)));
            return create_bin_node(MUL, lhs_sq, n);
        }
    }
    if (n->type == UN_NODE) {
        if (n->data->un_node.op == NEG)
            return create_un_node(NEG, copy(n));
        assert(false);
    }
    assert(false);
}
