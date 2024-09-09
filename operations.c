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
    float eval_ = eval(n->node_data->un_node.n, var_values, n_var_values);
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

Node *copy(Node *n) { return NULL; }

Node *derivative(Node *n) {
  if (n->node_type == NUM_NODE)
    return copy(n);
  if (n->node_type == VAR_NODE)
    return copy(n);
  Node *deriv = malloc(sizeof(Node));
  union NodeData *nd = malloc(sizeof(union NodeData));
  if (n->node_type == BIN_NODE) {
    deriv->node_type = BIN_NODE;
    n->node_type = BIN_NODE;
    nd->bin_node.lhs = derivative(n->node_data->bin_node.lhs);
    nd->bin_node.rhs = derivative(n->node_data->bin_node.rhs);
    if (n->node_data->bin_node.op == ADD)
      nd->bin_node.op = ADD;
    if (n->node_data->bin_node.op == SUB)
      nd->bin_node.op = SUB;
    if (n->node_data->bin_node.op == MUL) {
      // d (u(x) * v(x)) / dx = du(x)/dx * v(x) + u(x) * dv(x)/dx
      // lhs: u
      // rhs: v
      nd->bin_node.op = ADD;

      Node *lhs = malloc(sizeof(Node));
      union NodeData *lhs_nd = malloc(sizeof(union NodeData));
      lhs->node_type = BIN_NODE;
      lhs_nd->bin_node.op = MUL;
      lhs_nd->bin_node.lhs = derivative(n->node_data->bin_node.lhs);
      lhs_nd->bin_node.rhs = copy(n->node_data->bin_node.rhs);
      lhs->node_data = lhs_nd;

      Node *rhs = malloc(sizeof(Node));
      union NodeData *rhs_nd = malloc(sizeof(union NodeData));
      rhs->node_type = BIN_NODE;
      rhs_nd->bin_node.op = MUL;
      rhs_nd->bin_node.lhs = copy(n->node_data->bin_node.lhs);
      rhs_nd->bin_node.rhs = derivative(n->node_data->bin_node.rhs);
      rhs->node_data = rhs_nd;
    }
    if (n->node_data->bin_node.op == DIV) {
      assert(false);
    }
    deriv->node_data = nd;
    return deriv;
  }
  // if (n->node_type == UN_NODE) {
  //   float eval_ = eval(n->node_data->un_node.n, var_values, n_var_values);
  //   if (n->node_data->un_node.op == NEG)
  //     return -eval_;
  // }
  assert(false);
}
