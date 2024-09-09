#include "operations.h"
#include "parser.h"

#include <assert.h>
#include <string.h>

float eval(Node *n, VarValue *var_values, size_t n_var_values) {
  if (n->node_type == BIN_NODE) {
    float lhs_eval = eval(n->node_data->bin_node.lhs, var_values, n_var_values);
    float rhs_eval = eval(n->node_data->bin_node.rhs, var_values, n_var_values);
    if (n->node_data->bin_node.op == ADD)
      return lhs_eval + rhs_eval;
    if (n->node_data->bin_node.op == SUB)
      return lhs_eval - rhs_eval;
    if (n->node_data->bin_node.op == MUL)
      return lhs_eval * rhs_eval;
    if (n->node_data->bin_node.op == DIV)
      return lhs_eval / rhs_eval;
  }
  if (n->node_type == UN_NODE) {
    float eval_ = eval(n->node_data->un_node.child, var_values, n_var_values);
    if (n->node_data->un_node.op == NEG)
      return -eval_;
  }
  if (n->node_type == VAR_NODE) {
    char *name = n->node_data->var_node.name;
    for (size_t i = 0; i < n_var_values; ++i) {
      if (strcmp(name, var_values[i].name) == 0)
        return var_values[i].value;
    }
    assert(false);
  }
  if (n->node_type == NUM_NODE) {
    return n->node_data->num_node.num;
  }
  assert(false);
}

Node *copy(Node *n) {
  if (n->node_type == BIN_NODE) {
    BinNode bn = n->node_data->bin_node;
    return create_bin_node(bn.op, bn.lhs, bn.rhs);
  }
  if (n->node_type == UN_NODE) {
    UnNode un = n->node_data->un_node;
    return create_un_node(un.op, un.child);
  }
  if (n->node_type == VAR_NODE) {
    VarNode vn = n->node_data->var_node;
    return create_var_node(vn.name);
  }
  if (n->node_type == NUM_NODE) {
    NumNode nn = n->node_data->num_node;
    return create_num_node(nn.num);
  }
  assert(false);
}

// TODO derive by what?
Node *deriv(Node *n) {
  if (n->node_type == NUM_NODE)
    return create_num_node(0);
  if (n->node_type == VAR_NODE)
    return create_num_node(1);
  if (n->node_type == BIN_NODE) {
    // Node *lhs_deriv = derivative(n->node_data->bin_node.lhs);
    // Node *rhs_deriv = derivative(n->node_data->bin_node.rhs);
    // Node *lhs_copy = copy(n->node_data->bin_node.lhs);
    // Node *rhs_copy = copy(n->node_data->bin_node.rhs);
    Node *lhs = n->node_data->bin_node.lhs;
    Node *rhs = n->node_data->bin_node.rhs;
    if (n->node_data->bin_node.op == ADD)
      return create_bin_node(ADD, deriv(lhs), deriv(rhs));
    if (n->node_data->bin_node.op == SUB)
      return create_bin_node(SUB, deriv(lhs), deriv(rhs));
    if (n->node_data->bin_node.op == MUL) {
      // d (u(x) * v(x)) / dx = du(x)/dx * v(x) + u(x) * dv(x)/dx
      // lhs: u
      // rhs: v
      return create_bin_node(ADD, create_bin_node(MUL, deriv(lhs), copy(rhs)),
                             create_bin_node(MUL, copy(lhs), deriv(rhs)));
    }
    if (n->node_data->bin_node.op == DIV) {
      // d (u(x) / v(x)) / dx = v(x)² * [du(x)/dx * v(x) - u(x) * dv(x)/dx]
      // lhs: u
      // rhs: v
      Node *lhs_sq = create_bin_node(MUL, copy(lhs), copy(lhs));
      Node *n =
          create_bin_node(ADD, create_bin_node(MUL, deriv(lhs), copy(rhs)),
                          create_bin_node(MUL, copy(lhs), deriv(rhs)));
      return create_bin_node(MUL, lhs_sq, n);
    }
  }
  if (n->node_type == UN_NODE) {
    if (n->node_data->un_node.op == NEG)
      return create_un_node(NEG, copy(n));
    assert(false);
  }
  assert(false);
}
